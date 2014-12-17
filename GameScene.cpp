//
// Lucky Resistor's Lucky World Game
// ---------------------------------------------------------------------------
// (c)2014 by Lucky Resistor. See LICENSE for details.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "GameScene.h"


#include "GameState.h"
#include "LevelManager.h"
#include "Sounds.h"
#include "DynamicGameObject.h"

#include <LRMeggyJr.h>


namespace lr {

    
const uint8_t kGameSceneCanvasWidth = 32;
const uint8_t kGameSceneCanvasHeight = 16;
const uint8_t kGameSceneCanvasMaxObjects = 32;
    
    
GameScene::GameScene(const uint8_t* level, GameState *gameState)
:
    Scene(gameState),
    _levelPtr(level),
    _canvas(kGameSceneCanvasWidth, kGameSceneCanvasHeight),
    _canvasObjects(kGameSceneCanvasMaxObjects),
    _firstDynamicGameObject(0),
    _viewX(0),
    _viewY(0),
    _levelWidth(256),
    _endReason(NoEnd)
{
    // Read the initial game data.
    readLevelHeader();
    readLevelObjectsUpTo(_canvas.getWidth()-1);
    // Render this initial data in full onto the canvs.
    _canvas.drawInitialStaticObjects(_canvasObjects);
    // Remove already obsolete game objects
    removeVanishedStaticObjects(_canvas.getLastObjectPositionX());
    // Assign the canvas to allow player checks.
    _player.setGameCanvas(&_canvas);
    // Reset the last uncovered level
    gameState->setLastUncoveredLevel(NoLevel);
}

    
GameScene::~GameScene()
{
    DynamicGameObject *currentObject = _firstDynamicGameObject;
    while (currentObject != 0) {
        DynamicGameObject *nextObject = currentObject->getNextObject();
        delete currentObject;
        currentObject = nextObject;
    }
    _firstDynamicGameObject = 0;
}

    
void GameScene::running(const uint32_t &frame)
{
    runningPlayer(frame);
    if (_endReason == NoEnd) {
        runningDynamicObjectLogic(frame);
    }
    if (_endReason == NoEnd) {
        runningCollisionCheck(frame);
    }
    runningView(frame);
    runningRender(frame);
}

    
void GameScene::runningPlayer(const uint32_t &frame)
{
    // Player logic (buttion checks and movements)
    if (!_player.logic(frame, getGameState())) {
        meg.playSound(kSoundDeathFallDown, 2);
        endGame(EndDeathBottom, frame);
        return;
    }
}
    
    
void GameScene::runningView(const uint32_t &frame)
{
    // Calculate the best view, start with the X view:
    const uint8_t playerX = _player.getLevelX();
    if (playerX < 2) {
        _viewX = 0;
    } else {
        _viewX = playerX-2;
    }
    // limits for the view
    if (_viewX < _canvas.getLeftX()) {
        _viewX = _canvas.getLeftX();
    }
    const uint8_t maxViewX = _levelWidth-meg.getScreenWidth();
    if (_viewX > maxViewX) {
        _viewX = maxViewX;
    }
    
    // check if we should add more level
    const uint8_t canvasX = _canvas.getLeftX();
    // The optimal canvas offset is almost at the end of the canvas to allow backward scroll.
    // But is limited to even positions, because we only can scroll by 2.
    // If we reach the end of the canvas, it is scrolled by 2 positions to the left and new
    // level objects are loaded if necessary.
    const uint8_t optimalCanvasOffset = (_canvas.getWidth() - meg.getScreenWidth() - 2);
    if (canvasX < _canvas.getLastX() && (_viewX - canvasX) > optimalCanvasOffset) {
        // We need more level data, scroll the canvas
        _canvas.scroll2Left();
        // remove any vanished objects. They will never be rendered again.
        removeVanishedStaticObjects(_canvas.getLastObjectPositionX());
        removeVanishedDynamicObjects(_canvas.getLeftX());
        // read more objects from the level data
        readLevelObjectsUpTo(_canvas.getRightX());
        // rerender the canvas from the objects.
        _canvas.drawStaticObjects(_canvasObjects);
    }
    
    // Calculate the best Y view
    const uint8_t playerY = _player.getLevelY();
    uint8_t viewYGoal = 0;
    if (playerY < 4) {
        viewYGoal = 0;
    } else {
        viewYGoal = playerY - 4;
    }
    const uint8_t maxViewY = (_canvas.getHeight()-meg.getScreenHeight());
    if (viewYGoal > maxViewY) {
        viewYGoal = maxViewY;
    }
    // Slow move
    if (_viewY < viewYGoal && (frame & 7) == 0) {
        ++_viewY;
    } else if (_viewY > viewYGoal && (frame & 3) == 0) {
        --_viewY;
    }
}

    
void GameScene::runningDynamicObjectLogic(const uint32_t &frame)
{
    // Iterate trough the linked dynamic object list.
    DynamicGameObject *currentObject = _firstDynamicGameObject;
    DynamicGameObject *previousObject = 0; // using previous object pointer for removal.
    while (currentObject != 0) {
        const LogicResult result = currentObject->logic(_canvas, _player, frame);
        if (result == LogicResult::Remove) {
            // Remove the current object from the list.
            if (previousObject == 0) {
                // no previous object, manipulate the first pointer.
                _firstDynamicGameObject = currentObject->getNextObject();
                delete currentObject;
                currentObject = _firstDynamicGameObject;
            } else {
                // Point the previous object to the next one.
                previousObject->setNextObject(currentObject->getNextObject());
                delete currentObject;
                currentObject = previousObject->getNextObject();
            }
        } else if (result == LogicResult::KilledPlayer) {
            // The player was killed, stop everything and end the game.
            meg.playSound(kSoundDeathDynObject, 2);
            endGame(EndDeathDynObject, frame);
            return;
        } else if (result == LogicResult::AddObject) {
            // We have to add a new object (we insert it at the begin of the list).
            DynamicGameObject *newObject = result.getGameObject();
            newObject->setNextObject(_firstDynamicGameObject);
            _firstDynamicGameObject = newObject;
            // and continue with the iteration.
            previousObject = currentObject;
            currentObject = currentObject->getNextObject();
        } else if (result == LogicResult::NextLevel) {
            meg.playSound(kSoundFinishLevel, 2);
            const uint8_t nextLevelIndex = result.getNextLevelIndex();
            if (nextLevelIndex == 31) { // game end
                getGameState()->setLevelFinished(getGameState()->getLevel());
                getGameState()->saveToStorage();
                endGame(EndFinal, frame);
                return;
            } else if (nextLevelIndex == 30) { // No next level.
                getGameState()->setLevelFinished(getGameState()->getLevel());
                getGameState()->saveToStorage();
                endGame(EndSuccess, frame);
                return;
            } else {
                const LevelIdentifier nextLevelIdentifier = getGameState()->getLevelManager()->getLevelForIndex(nextLevelIndex);
                getGameState()->setLevelFinished(getGameState()->getLevel());
                getGameState()->setLevelVisible(nextLevelIdentifier);
                getGameState()->setLastUncoveredLevel(nextLevelIdentifier);
                getGameState()->saveToStorage();
                endGame(EndSuccess, frame);
                return;
            }
        } else {
            // Just continue in the iteration.
            previousObject = currentObject;
            currentObject = currentObject->getNextObject();
        }
    }
}


void GameScene::runningCollisionCheck(const uint32_t &frame)
{
    // Get the block under the player.
    const GameCanvas::Block blockUnderPlayer = _canvas.getBlockAt(_player.getLevelX(), _player.getLevelY());
    GameState* const gameState = getGameState();
    
    // Stop further checks if there is empty space.
    if (blockUnderPlayer == GameCanvas::Empty) {
        return;
    }
    
    // A coin.
    if (blockUnderPlayer == GameCanvas::Coin) {
        // Collect the coin.
        gameState->setCoins(gameState->getCoins()+1);
        if (gameState->getCoins() >= 64) {
            // increase the lives, maximum lives are 8
            if (gameState->getLives() < 8) {
                meg.playSound(kSoundNewLive, 1); // play with higher priority.
                gameState->setLives(gameState->getLives()+1);
            } else {
                // simple reset without new life.
                meg.playSound(kSoundCoin);
            }
            gameState->setCoins(0);
        } else {
            meg.playSound(kSoundCoin);
        }
        // remove the coin from the level.
        _canvas.setBlockAt(_player.getLevelX(), _player.getLevelY(), GameCanvas::Empty);
    } else if (blockUnderPlayer == GameCanvas::Lava) {
        meg.playSound(kSoundDeathDynObject, 2);
        endGame(EndDeathDynObject, frame);
    } else if (blockUnderPlayer == GameCanvas::Water) {
        meg.playSound(kSoundDeathDynObject, 2);
        endGame(EndDeathDynObject, frame);
    }
}

    
void GameScene::runningRender(const uint32_t &frame)
{
    // Render the dynamic objects to the canvas.
    DynamicGameObject *currentObject = _firstDynamicGameObject;
    while (currentObject != 0) {
        currentObject->render(_canvas, frame);
        currentObject = currentObject->getNextObject();
    }
    // Render the canvas to the display.
    _canvas.renderCanvas(_viewX, _viewY, frame);
    // Render special object directory to the display
    currentObject = _firstDynamicGameObject;
    while (currentObject != 0) {
        if (currentObject->doesRenderToDisplay()) {
            // Check if the object is visible at all
            if (currentObject->getMaxX() >= _viewX && currentObject->getX() <= (_viewX+meg.getScreenWidth()-1) &&
                currentObject->getMaxY() >= _viewY && currentObject->getY() < (_viewY+meg.getScreenHeight())) {
                currentObject->renderToDisplay(_viewX, _viewY, frame);
            }
        }
        currentObject = currentObject->getNextObject();
    }
    // render the player object (always on top)
    _player.renderPlayer(_viewX, _viewY, frame);
}
    
    
void GameScene::ending(const uint32_t &frame)
{
    const uint32_t endFrame = frame - _endStartFrame;
    if (_endReason == EndSuccess || _endReason == EndFinal) {
        if ((endFrame & 0x7) == 0) {
            meg.scrollPixel(MeggyJr::ScrollLeft);
            for (uint8_t y = 0; y < meg.getScreenHeight(); ++y) {
                meg.setPixel(meg.getScreenWidth()-1, y, Color::black());
            }
        }
        if (endFrame >= 64) {
            if (_endReason == EndFinal) {
                setNextScene(FinalId);
            } else {
                setNextScene(LevelSelectId);
            }
            setState(SceneSwitch);
        }
    } else if (_endReason == EndDeathBottom) {
        // the player fell out of the bottom.
        // scroll up, fade player
        const uint32_t scrollSpeed = 4;
        if (endFrame == 1) {
            // in the first frame, render the last static level without the player.
            _canvas.renderCanvas(_viewX, _viewY, frame);
            // scroll it up one pixel
            meg.scrollPixel(MeggyJr::ScrollUp);
            // draw the bottom line black.
            for (uint8_t x = 0; x < meg.getScreenWidth(); ++x) {
                meg.setPixel(x, meg.getScreenHeight()-1, Color::black());
            }
            // draw the player pixel
            meg.setPixel(_player.getLevelX()-_viewX, 7, Color(15, 7, 15));
        } else if ((endFrame % scrollSpeed) == 0 && endFrame < (scrollSpeed * 16)) {
            meg.setPixel(_player.getLevelX()-_viewX, 7, Color(0, 0, 0));
            meg.scrollPixel(MeggyJr::ScrollUp);
            for (uint8_t x = 0; x < meg.getScreenWidth(); ++x) {
                meg.setPixel(x, meg.getScreenHeight()-1, Color::black());
            }
            const uint8_t color = 15 - (endFrame / scrollSpeed);
            meg.setPixel(_player.getLevelX()-_viewX, 7, Color(color, color/2, color));
        } else if (endFrame == (scrollSpeed*16)) {
            setNextScene(Scene::DeathId);
            setState(SceneSwitch);
        }
    } else if (_endReason == EndDeathDynObject) {
        if (endFrame == 192) {
            setNextScene(Scene::DeathId);
            setState(SceneSwitch);
        } else if (endFrame < 128) {
            // Render the death player on the canvas.
            _canvas.renderCanvas(_viewX, _viewY, endFrame);
            _player.renderDeathPlayer(_viewX, _viewY, endFrame);
        } else if (endFrame >= 128 && endFrame < 192) {
            // blend out
            const uint8_t screenPlayerX = _player.getLevelX()-_viewX;
            const uint8_t screenPlayerY = meg.getScreenHeight()-(_player.getLevelY()-_viewY)-1;
            const uint8_t phase = ((endFrame - 128) / 8) + 1;
            for (uint8_t x = 0; x < (phase*2)+1; ++x) {
                for (uint8_t y = 0; y < (phase*2)+1; ++y) {
                    const uint8_t tx = x-phase+screenPlayerX;
                    const uint8_t ty = y-phase+screenPlayerY;
                    meg.setPixelS(tx, ty, Color::black());
                }
            }
            const uint8_t color = 14-((phase-1)*2);
            meg.setPixel(screenPlayerX, screenPlayerY, Color(color, color/2, color));
        }
    }
}
    
    
void GameScene::readLevelHeader()
{
    // read and interpret the start position.
    const uint8_t byte0 = pgm_read_byte(_levelPtr++);
    const uint8_t playerStartX = byte0 & 0x0F;
    const uint8_t playerStartY = (byte0 >> 4);
    const uint8_t byte1 = pgm_read_byte(_levelPtr++);
    _levelWidth = ((uint16_t)(byte1 & 0x0f)+1) * 16;
    _levelTheme = ((byte1 >> 4) & 0x3);
    
    // Assign some values.
    _player.setLevelPos(playerStartX, playerStartY);
    _canvas.setLastX(_levelWidth-_canvas.getWidth());
    _canvas.setTheme(_levelTheme);
}
    

void GameScene::removeVanishedStaticObjects(uint8_t beforeX)
{
    // First check all static objects.
    for (GameObjectArray::SizeType i = 0; i < _canvasObjects.getSize();) {
        const GameObject &gameObject = _canvasObjects.getGameObject(i);
        const uint8_t gameObjectLastX = gameObject.getX() + gameObject.getWidth() - 1;
        if (gameObjectLastX < beforeX) {
            _canvasObjects.removeObject(i);
        } else {
            ++i; // only increment if we didn't remove any object from the array.
        }
    }
}

    
void GameScene::removeVanishedDynamicObjects(uint8_t beforeX)
{
    // Now check all dynamic objects.
    DynamicGameObject *currentObject = _firstDynamicGameObject;
    DynamicGameObject *previousObject = 0; // using previous object pointer for removal.
    while (currentObject != 0) {
        const uint8_t gameObjectLastX = currentObject->getX() + currentObject->getWidth() - 1;
        if (gameObjectLastX < beforeX) {
            // Remove the current object from the list.
            if (previousObject == 0) {
                // no previous object, manipulate the first pointer.
                _firstDynamicGameObject = currentObject->getNextObject();
                delete currentObject;
                currentObject = _firstDynamicGameObject;
            } else {
                // Point the previous object to the next one.
                previousObject->setNextObject(currentObject->getNextObject());
                delete currentObject;
                currentObject = previousObject->getNextObject();
            }
        } else {
            // Just continue in the iteration.
            previousObject = currentObject;
            currentObject = currentObject->getNextObject();
        }
    }
}
    
    
void GameScene::readLevelObjectsUpTo(uint8_t lastX)
{
    // Check the next object.
    GameObject gameObject;
    for (;;) {
        gameObject = GameObject::fromLevelData(_levelPtr);
        if (gameObject.isEndMark() || gameObject.getX() > lastX) {
            break;
        }
        _levelPtr += 3; // consume game object.
        if (gameObject.isDynamic()) {
            DynamicGameObject *dynamicGameObject = DynamicGameObject::createFromGameObject(gameObject);
            if (dynamicGameObject != 0) {
                // add it to the dynamic game object list.
                dynamicGameObject->setNextObject(_firstDynamicGameObject);
                _firstDynamicGameObject = dynamicGameObject;
            }
        } else {
            _canvasObjects.addObject(gameObject);
        }
    }
}
    
    
void GameScene::endGame(EndReason endReason, const uint32_t &frame)
{
    _endReason = endReason;
    setState(SceneEnding);
    _endStartFrame = frame;
}
    

}



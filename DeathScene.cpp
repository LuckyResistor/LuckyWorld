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

#include "DeathScene.h"


#include "GameState.h"
#include "Sounds.h"

#include <LRMeggyJr.h>


namespace lr {
    
    
namespace {
const uint8_t liveDigits[] PROGMEM = {
    B00111100,
    B01100110,
    B01100110,
    B01100110,
    B00111100,
    
    B00111000,
    B00011000,
    B00011000,
    B00011000,
    B01111110,

    B01111100,
    B00000110,
    B00111100,
    B01100000,
    B01111110,

    B01111100,
    B00000110,
    B00111100,
    B00000110,
    B01111100,

    B01100110,
    B01100110,
    B01111110,
    B00000110,
    B00000110,

    B01111110,
    B01100000,
    B01111100,
    B00000110,
    B01111100,

    B00111100,
    B01100000,
    B01111100,
    B01100110,
    B00111100,

    B01111110,
    B00001100,
    B00001100,
    B00011000,
    B00011000,

    B00111100,
    B01100110,
    B00111100,
    B01100110,
    B00111100,
};
}
 
    
DeathScene::DeathScene(GameState *gameState)
:
    Scene(gameState),
    _frame(0)
{
    // Here we have to logic to reduce the lives
    if (gameState->getLives() <= 1) {
        // game over
        gameState->setLives(0);
        // on game over the player loses the progress of the current
        // four level block. The next section checks the current game
        // progress.
        const uint8_t regularLevelCount = gameState->getLevelManager()->getRegularLevelCount();
        uint8_t maxFinishedLevel = regularLevelCount;
        for (uint8_t i = 0; i < regularLevelCount; ++i) {
            const LevelIdentifier level = gameState->getLevelManager()->getLevelForIndex(i);
            if (!gameState->isLevelFinished(level)) {
                maxFinishedLevel = i;
                break;
            }
        }
        const uint8_t finishedBlock = maxFinishedLevel/4;
        // If all regular levels are finished, game over does not reset the progress
        if (finishedBlock < 4) {
            // reset progress on the regular levels, keep the first one visible.
            for (uint8_t i = finishedBlock*4+1; i < regularLevelCount; ++i) {
                const LevelIdentifier level = gameState->getLevelManager()->getLevelForIndex(i);
                gameState->setLevelVisible(level, false);
            }
            for (uint8_t i = finishedBlock*4; i < regularLevelCount; ++i) {
                const LevelIdentifier level = gameState->getLevelManager()->getLevelForIndex(i);
                gameState->setLevelFinished(level, false);
            }
            // reset the progress on the hidden levels.
            for (uint8_t i = finishedBlock*4; i < regularLevelCount; ++i) {
                const LevelIdentifier hiddenLevel = gameState->getLevelManager()->getLevelForIndex(i+0x10);
                gameState->setLevelVisible(hiddenLevel, false);
                gameState->setLevelFinished(hiddenLevel, false);
            }
            // Set the player position back to the initial level of that block
            gameState->setLevel(gameState->getLevelManager()->getLevelForIndex(finishedBlock*4));
        }
        // Set the number of coins to zero
        gameState->setCoins(0);
    } else {
        // lose one live
        gameState->setLives(gameState->getLives()-1);
    }
    // Prepare the pointer
    _oldLives = liveDigits + (5*(gameState->getLives()+1));
    _newLives = liveDigits + (5*gameState->getLives());
    // Draw the number of lives
    meg.drawSprite(_oldLives, 5, 0, 1, Color::red());
    // Now store the state
    gameState->saveToStorage();
}

    
DeathScene::~DeathScene()
{
}
    
    
void DeathScene::running(const uint32_t &frame)
{
    if (_frame == 0) {
        _frame = frame;
    }
    const uint32_t sceneFrame = frame-_frame;
    meg.clear();
    
    // show the old lives for two seconds, then scroll to the new number
    const uint32_t timing = 50;
    if (sceneFrame < timing) {
        meg.drawSprite(_oldLives, 5, 0, 1, Color::red());
    } else if (sceneFrame < (timing*2)) {
        if (sceneFrame == timing) {
            if (getGameState()->getLives() == 0) {
                meg.playSound(kSoundGameOver);
            } else {
                meg.playSound(kSoundLiveLost);
            }
        }
        int8_t shift = ((sceneFrame - timing) * 8 / timing);
        if (shift < 2) {
            meg.drawSprite(_oldLives, 5, 0, 1, Color::red());
        } else {
            meg.drawSprite(_oldLives, 5, 0, 3-shift, Color::red());
        }
        meg.drawSprite(_newLives, 5, 0, 9-shift, Color::white());
    } else if (sceneFrame < (timing*4)) {
        meg.drawSprite(_newLives, 5, 0, 1, Color::white());
    } else if (sceneFrame >= (timing*4)) {
        if (getGameState()->getLives() == 0) {
            // after game over, set lives back to 3
            getGameState()->setLives(getGameState()->getInitialLives());
            setNextScene(Scene::IntroId);
        } else {
            setNextScene(Scene::LevelSelectId);
        }
        setState(SceneSwitch);
    }
}

    
}



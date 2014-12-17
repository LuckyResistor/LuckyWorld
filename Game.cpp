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
#include "Game.h"


#include "IntroScene.h"
#include "SelectLevelScene.h"
#include "GameScene.h"
#include "ClearGameDataScene.h"
#include "GameState.h"
#include "LevelManager.h"
#include "DeathScene.h"
#include "FinalScene.h"

#include <LRMeggyJr.h>


namespace lr {


// The global instance of the game.
Game gGame;
    
    
Game::Game()
:
    _levelManager(0),
    _gameState(0),
    _currentScene(0),
    _livesDisplayState(LDS_Idle),
    _lastLives(0),
    _livesChangeFrame(0)
{
    _levelManager = new LevelManager();
    _gameState = new GameState(_levelManager);
}

    
Game::~Game()
{
    delete _gameState;
    delete _levelManager;
}


void Game::setup()
{
    meg.setup(MeggyJr::FrameRate60);
    
    // Try to read the game state
    _gameState->loadFromStorage();
    
    // Create the initial scene
    _currentScene = createScene(Scene::StartId);
    
    // set the initial number of lives
    if (_gameState->getLives() == 0) {
        // save after a game over, increase to 3 for a fresh start.
        _gameState->setLives(_gameState->getInitialLives());
    }
    _lastLives = _gameState->getLives();
}


void Game::loop()
{
    // synchronize the screen
    const uint32_t frame = meg.frameSync();
    
    // the scene logic.
    switch (_currentScene->getState()) {
        case Scene::SceneStarting:
            _currentScene->starting(frame);
            break;
            
        case Scene::SceneRunning:
            _currentScene->running(frame);
            break;
            
        case Scene::SceneEnding:
            _currentScene->ending(frame);
            break;
            
        case Scene::SceneSwitch:
        {
            const Scene::Identifier nextScene = _currentScene->getNextScene();
            // Destroy the current scene.
            delete _currentScene;
            // Create the new scene.
            _currentScene = createScene(nextScene);
        }
        break;
    }
    
    // extra LED logic to display lives
    displayLives(frame);
}

    
Scene* Game::createScene(Scene::Identifier sceneIdentifier)
{
    switch (sceneIdentifier) {
        case Scene::IntroId:
            return new IntroScene(_gameState);
        case Scene::LevelSelectId:
            return new SelectLevelScene(_gameState);
        case Scene::ClearGameDataId:
            return new ClearGameDataScene(_gameState);
        case Scene::GameId:
        {
            const uint8_t* nextLevel = _levelManager->getLevel(_gameState->getLevel());
            if (nextLevel != 0) {
                return new GameScene(nextLevel, _gameState);
            }
        }
        case Scene::DeathId:
            return new DeathScene(_gameState);
        case Scene::FinalId:
            return new FinalScene(_gameState);
        default:
            break;
    }
    return 0;
}

    
void Game::displayLives(const uint32_t &frame)
{
    if (_livesDisplayState == LDS_Idle) {
        if (_lastLives == _gameState->getLives()) {
            setLEDToLives(_gameState->getLives());
        } else {
            _livesDisplayState = LDS_Blink;
            _livesChangeFrame = frame;
        }
    } else {
        const uint32_t changeFrame = frame - _livesChangeFrame;
        if (changeFrame > 120) {
            setLEDToLives(_gameState->getLives());
            _lastLives = _gameState->getLives();
            _livesDisplayState = LDS_Idle;
        } else {
            if ((changeFrame & 0xf) < 8) {
                setLEDToLives(_gameState->getLives());
            } else {
                setLEDToLives(_lastLives);
            }
        }
    }
}

    
void Game::setLEDToLives(const uint8_t lives)
{
    if (lives == 0) {
        meg.setExtraLeds(B00000000);
    } else if (lives > 7) {
        meg.setExtraLeds(B11111111);
    } else {
        meg.setExtraLeds((1<<lives)-1);
    }
}

    
}



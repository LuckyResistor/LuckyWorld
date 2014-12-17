#pragma once
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


#include <Arduino.h>


#include "Scene.h"


namespace lr {

    
class LevelManager;
class GameState;


/// This represents the complete game.
///
class Game
{
    enum LivesDisplayState : uint8_t {
        LDS_Idle,
        LDS_Blink,
    };
    
public:
    /// Create the game scene logic
    ///
    Game();
    
    /// dtor
    ///
    ~Game();
    
public:
    /// The setup method has to be called once.
    ///
    void setup();
    
    /// The loop method should be called in loop();
    ///
    void loop();
    
private:
    /// The scene factory.
    /// Create a new scene for the given identifier.
    ///
    Scene* createScene(Scene::Identifier sceneIdentifier);
    
    /// Handle the lives logic
    ///
    void displayLives(const uint32_t &frame);
    
    /// Set the extra LED to the given number of lives
    ///
    void setLEDToLives(const uint8_t lives);
    
private:
    LevelManager *_levelManager; // The level manager.
    GameState *_gameState; // The state of the game.
    Scene *_currentScene; // The current running scene
    LivesDisplayState _livesDisplayState; // The display state for the lives.
    uint8_t _lastLives; // The last number of lives
    uint32_t _livesChangeFrame; // The frame at a lives change.
};


// The global instance of the game.
extern Game gGame;
    
    
}


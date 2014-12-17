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


namespace lr {

    
class GameState;
    
    
/// Class description
///
class Scene
{
public:
    /// All known scenes of the game.
    enum Identifier : uint8_t {
        IntroId,
        LevelSelectId,
        GameId,
        DeathId,
        FinalId,
        ClearGameDataId,
        
        /// The start scene of the game.
        StartId = IntroId
    };

public:
    /// The state of this scene.
    enum State : uint8_t {
        SceneStarting, // The scene is starting. starting() is called.
        SceneRunning, // The scene is running. running() is called.
        SceneEnding, // The scene is ending. ending() is called.
        SceneSwitch, // The controller shall switch to the requested scene.
    };
    
public:
    /// ctor
    Scene(GameState *gameState);
   
    /// dtor
    virtual ~Scene();
    
public:
    /// Get the scene state.
    inline State getState() const { return _state; }
    
    /// Set the scene state.
    inline void setState(State state) { _state = state; }
    
    /// Get the game state.
    inline GameState* getGameState() const { return _gameState; }
    
    /// Get the next scene.
    inline Identifier getNextScene() const { return _nextScene; }
    
    /// Set the next scene.
    inline void setNextScene(Identifier nextScene) { _nextScene = nextScene; }
    
public:
    /// The starting logic.
    virtual void starting(const uint32_t &frame);
    
    /// The running logic.
    virtual void running(const uint32_t &frame);
    
    /// The ending logic.
    virtual void ending(const uint32_t &frame);
    
private:
    State _state; // The state of the scene.
    GameState *_gameState; // The state of the game.
    Identifier _nextScene; // The requested next scene.
};
    

}



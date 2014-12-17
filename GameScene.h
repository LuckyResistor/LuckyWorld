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

#include "GameObject.h"
#include "GameObjectArray.h"
#include "GameCanvas.h"
#include "Scene.h"
#include "Player.h"


namespace lr {
    
    
class DynamicGameObject;
    

/// Represents the scene with the actual game.
///
class GameScene : public Scene
{
private:
    /// The reason for the end of the game.
    enum EndReason {
        /// The game is still running.
        NoEnd,
        /// The player lives, level successfully finshed.
        EndSuccess,
        /// The last level was successfully finished.
        EndFinal,
        /// The player was killed by a dynamic object
        EndDeathDynObject,
        /// Player fell out of the bottom of the level.
        EndDeathBottom,
    };
    
public:
    /// Create a new game scene with the given level and game state
    ///
    GameScene(const uint8_t* level, GameState *gameState);
    
    /// dtor
    virtual ~GameScene();
    
public: // implement Scene
    //virtual void starting(const uint32_t &frame);
    virtual void running(const uint32_t &frame);
    virtual void ending(const uint32_t &frame);
    
private:
    /// Read the header of the level
    void readLevelHeader();
    
    /// Read all level objetcs up to and including position "lastX".
    void readLevelObjectsUpTo(uint8_t lastX);

    /// Remove vanished static objects.
    void removeVanishedStaticObjects(uint8_t beforeX);

    /// Remove vanished dynamic objects.
    void removeVanishedDynamicObjects(uint8_t beforeX);
    
    /// End the game
    void endGame(EndReason endReason, const uint32_t &frame);
    
    /// The player logic
    void runningPlayer(const uint32_t &frame);
    
    /// Running the logic of the dynamic objects
    void runningDynamicObjectLogic(const uint32_t &frame);
    
    /// The collision logic
    void runningCollisionCheck(const uint32_t &frame);
    
    /// The view logic
    void runningView(const uint32_t &frame);

    /// Render the scene
    void runningRender(const uint32_t &frame);
    
private:
    // The level pointer
    const uint8_t *_levelPtr;
    
    // The canvas where a small part of the level is kept
    GameCanvas _canvas;
        
    // An array with max 32 static game objects currently in the canvas
    GameObjectArray _canvasObjects;
    
    // A pointer to the first dynamic game object.
    DynamicGameObject *_firstDynamicGameObject;
    
    // The player object.
    Player _player;
    
    // The bottom left coordinate of the current view
    uint8_t _viewX;
    
    // The bottom left coordinate of the current view.
    uint8_t _viewY;
    
    // The width of the current level
    uint16_t _levelWidth;
    
    // The theme of the current level
    uint8_t _levelTheme;
    
    // The optimal canvas offset
    uint8_t _optimalCanvasOffset;
    
    // The reason for the end
    EndReason _endReason;
    
    // The start frame for the end
    uint32_t _endStartFrame;
};


}


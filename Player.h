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

    
class GameCanvas;
class GameState;
    
    
/// Represents the player figure.
///
class Player
{
public:
    /// The action state of the player
    ///
    enum ActionState : uint8_t {
        /// The player is running or falling.
        ActionRun,
        /// The player currently jumps up.
        ActionJumpUp,
        /// The player is hovering. At the top of a jump.
        ActionJumpHover,
    };
    
public:
    /// The position type of the player.
    ///
    typedef int16_t Position;
    
    /// The speed type of the player.
    ///
    typedef int8_t Speed;
        
public:
    /// ctor
    ///
    Player();
   
    /// dtor
    ///
    ~Player();
    
public:
    /// Set the canvas to allow collision detection
    ///
    inline void setGameCanvas(GameCanvas *gameCanvas) { _gameCanvas = gameCanvas; }
    
    /// Set the position of the player in level coordinates
    ///
    void setLevelPos(uint8_t x, uint8_t y);
    
    /// Get the resolution
    ///
    inline Position getPositionResolution() const { return _positionResolution; }
    
    /// Get the absolute x position of the player.
    ///
    inline Position getX() const { return _x; }
    
    /// Set the x position of the player.
    ///
    inline void setX(Position x) { _x = x; }
    
    /// Get the y position of the player.
    ///
    inline Position getY() const { return _y; }
    
    /// Set the y position of the player.
    ///
    inline void setY(Position y) { _y = y; }
    
    /// Get the level X position of the player.
    ///
    inline uint8_t getLevelX() const { return _x / _positionResolution; }
    
    /// Set the level X position of the player.
    ///
    inline void setLevelX(uint8_t x) { _x = (((Position)x)*_positionResolution)+(_positionResolution/2); }
    
    /// Get the level Y position of the player.
    ///
    inline uint8_t getLevelY() const { return _y / _positionResolution; }
    
    /// Set the level Y position of the player
    ///
    inline void setLevelY(uint8_t y) { _y = (((Position)y)*_positionResolution)+(_positionResolution/2); }

    /// Reset the X position into the middle position
    ///
    inline void setMiddleX() { setLevelX(getLevelX()); }
    
    /// Reset the Y position into the middle position
    ///
    inline void setMiddleY() { setLevelY(getLevelY()); }
    
    /// Get the current speed in X direction.
    ///
    inline Speed getSpeedX() { return _speedX; }
    
    /// Set the current speed in X direction.
    ///
    inline void setSpeedX(Speed speedX) { _speedX = speedX; }
    
    /// Get the current speed in Y direction.
    ///
    inline Speed getSpeedY() { return _speedY; }
    
    /// Set the current speed in Y direction.
    ///
    inline void setSpeedY(Speed speedY) { _speedY = speedY; }
    
    /// Render the player to the display.
    ///
    /// @param viewX The bottom left position of the current view.
    /// @param viewY The bottom left position of the current view.
    /// @param frame The current frame.
    ///
    void renderPlayer(uint8_t viewX, uint8_t viewY, const uint32_t &frame);

    /// Render the player to the display if dead.
    ///
    /// @param viewX The bottom left position of the current view.
    /// @param viewY The bottom left position of the current view.
    /// @param frame The current frame.
    ///
    void renderDeathPlayer(uint8_t viewX, uint8_t viewY, const uint32_t &frame);
 
    /// The logic for the player.
    ///
    /// @param frame The current frame of the game
    /// @return true if everything is ok, false if the player fell out of the bottom of the level.
    ///
    bool logic(const uint32_t &frame, GameState *gameState);
    
private:
    const Position _positionResolution; // The resolution
    const Speed _maximumSpeed; // The maximum speed.
    const Speed _speedReduction; // The speed reduction if no button is pressed.
    GameCanvas *_gameCanvas; // The game canvas.
    Position _x; // The x position of the player.
    Position _y; // The y position of the player.
    Speed _speedX; // The current speed in X position.
    Speed _speedY; // The current speed in Y position.
    ActionState _actionState; // The action state.
    Position _jumpMaxY; // The maximum height of the jump.
    uint32_t _jumpStartFrame; // The start frame of the jump for the timing.
};
    

}



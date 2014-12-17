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

    
class GameObject
{
public:
    /// The type of the object.
    ///
    enum Type : uint8_t {
        Ground           = 0x01,
        Water            = 0x02,
        Lava             = 0x03,
        Coin             = 0x04,
        GroundSteps      = 0x05,
        MovingPlatform   = 0x06,
        JumpingFireball  = 0x07,
        Canon            = 0x08,
        Monster          = 0x09,
        UnstableGround   = 0x0a,
        
        Portal           = 0x0f,
        
        // Pure dynamic objects.
        CanonBall        = 0x10,
    };
    
    /// The direction of the object.
    ///
    enum Direction : uint8_t {
        DirectionNorth     = 0,
        DirectionNorthEast = 1,
        DirectionEast      = 2,
        DirectionSouthEast = 3,
        DirectionSouth     = 4,
        DirectionSouthWest = 5,
        DirectionWest      = 6,
        DirectionNorthWest = 7,
        NoDirection        = 8,
    };
    
    /// Orientation of the object.
    ///
    enum Orientation : uint8_t {
        NoOrientation = 0,
        OrientationHorizontal,
        OrientationVertical,
    };
    
    /// The momevemt of an object
    ///
    enum Movement : uint8_t {
        NoMovement = 0,
        MovementContinuous,
        MovementBounce,
    };
    
public:
    /// Creates an empty game object.
    ///
    GameObject();
    
public:
    /// Check if this is the end mark.
    ///
    inline bool isEndMark() const { return _byte0 == 0 && _byte1 == 0 && _byte2 == 0; }
    
    /// Get the X position of the object.
    ///
    inline uint8_t getX() const { return _byte1; }
    
    /// Get the Y position of the object.
    ///
    inline uint8_t getY() const { return (_byte0 & 0x0f); }
    
    /// Get the type of the object.
    ///
    inline Type getType() const { return static_cast<Type>(_byte0 >> 4); }
    
    /// Get the width of the object.
    ///
    uint8_t getWidth() const;
    
    /// Get the height of the object.
    ///
    uint8_t getHeight() const;
    
    /// Get the spacing for the object.
    /// This is supported by the coin block.
    ///
    uint8_t getSpacing() const;
    
    /// Get the direction of the object.
    /// For ground steps where the ground is.
    /// For the canon in which direction it points.
    ///
    Direction getDirection() const;
    
    /// Get the orientation of the object.
    /// For moving platform the orientation.
    ///
    Orientation getOrientation() const;

    /// Get the platform width
    /// For moving platforms.
    ///
    uint8_t getPlatformWidth() const;
    
    /// Get the movement
    ///
    Movement getMovement() const;
    
    /// Get the speed
    ///
    uint8_t getSpeed() const;
    
    /// Get the phase shift
    ///
    uint8_t getPhaseShift() const;
    
    /// Get the interval
    ///
    uint8_t getInterval() const;
    
    /// If the object is a hidden object
    ///
    bool isHidden() const;
    
    /// Get the next level index for a portal
    ///
    uint8_t getNextLevelIndex() const;
    
    /// Check is this game object is dynamic.
    /// Dynamic objects have to be redrawn in every frame.
    ///
    bool isDynamic() const;
    
public:
    /// Read a game object from a level
    ///
    static GameObject fromLevelData(const uint8_t *levelData);

private:
    /// Private ctor: Creates a level object from the given values.
    ///
    GameObject(uint8_t byte0, uint8_t byte1, uint8_t byte2);

private:
    uint8_t _byte0;
    uint8_t _byte1;
    uint8_t _byte2;
};
    
    
}
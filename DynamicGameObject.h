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


namespace lr {


class DynamicGameObject;
class GameCanvas;
class Player;
  
    
/// The result of the logic call
///
class LogicResult
{
public:
    enum Result : uint8_t {
        Continue, // No action necessary, keep the object.
        KilledPlayer, // This object killed the player.
        Remove, // Remove this object from the list.
        AddObject, // Add this new object to the object list.
        NextLevel, // Finish this level and move to the next level.
    };
    
public:
    /// Create a logic result.
    ///
    LogicResult(Result result, DynamicGameObject *gameObject = 0);
    
    /// Create a logic result with a next level index
    ///
    LogicResult(const uint8_t nextLevelIndex);
    
    /// dtor
    ///
    ~LogicResult();

public:
    /// Compare the object with the enumeration.
    ///
    inline bool operator==(Result result) const { return _result == result; }
    
    /// Get the game object
    ///
    inline DynamicGameObject* getGameObject() const { return _values.gameObject; }
    
    /// Get the next level
    ///
    inline uint8_t getNextLevelIndex() const { return _values.nextLevelIndex; }
    
private:
    Result _result;
    union {
        DynamicGameObject *gameObject;
        uint8_t nextLevelIndex;
    } _values;
};
    
    
/// The base class for all dynamic game objects.
///
class DynamicGameObject
{
public:
    /// Creates a new dynamic game object.
    ///
    /// Copies the X and Y position from the game object.
    ///
    /// @param gameObject The game object to copy the x and y coordinates from.
    ///
    DynamicGameObject(const GameObject &gameObject);
   
    /// Creates a new dynamic game object.
    ///
    /// @param x The start x coordinate of the object.
    /// @param y The start y coordinate of the object.
    ///
    DynamicGameObject(const uint8_t x, const uint8_t y);
    
    /// dtor
    ///
    virtual ~DynamicGameObject();
    
public:
    /// Get the next game object in the list.
    ///
    inline DynamicGameObject* getNextObject() const { return _nextObject; }
    
    /// Set the next game object in the list.
    ///
    inline void setNextObject(DynamicGameObject *dynamicGameObject) { _nextObject = dynamicGameObject; }
    
    /// Get the x position of this object
    ///
    inline uint8_t getX() const { return _x; }
    
    /// Set the x position of this object.
    ///
    inline void setX(uint8_t x) { _x = x; }
    
    /// Get the y position of this object.
    ///
    inline uint8_t getY() const { return _y; }
    
    /// Set the y position of this object.
    ///
    inline void setY(uint8_t y) { _y = y; }
    
    /// Get the width of this object.
    ///
    inline uint8_t getWidth() const { return _width; }
    
    /// Set the width of this object.
    ///
    inline void setWidth(uint8_t width) { _width = width; }
    
    /// Get the height of this object.
    ///
    inline uint8_t getHeight() const { return _height; }
    
    /// Set the height of this object.
    ///
    inline void setHeight(uint8_t height) { _height = height; }
    
    /// Get the maximum x position of the object.
    ///
    inline uint8_t getMaxX() const { return _x + _width - 1; }
    
    /// Get the maximum y position of the object.
    ///
    inline uint8_t getMaxY() const { return _y + _height -1; }
    
public: // Virtual methods
    /// Get the type of this game object
    ///
    virtual GameObject::Type getType() const = 0;
    
    /// Process the logic for this object.
    ///
    virtual LogicResult logic(GameCanvas &canvas, Player &player, const uint32_t &frame) = 0;

    /// Render this object into the canvas.
    ///
    virtual void render(GameCanvas &canvas, const uint32_t &frame) const = 0;
    
    /// If this object does render to the display directly
    ///
    virtual bool doesRenderToDisplay() const;
    
    /// Render this object onto the screen.
    ///
    virtual void renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const;
    
public: // Factory method
    /// Create a new dynamic object from the given static one
    ///
    static DynamicGameObject* createFromGameObject(const GameObject &gameObject);
    
protected:
    /// Increase the given coordinates into the direction
    ///
    static void moveToDirection(const GameObject::Direction direction, uint8_t &x, uint8_t &y);
    
protected:
    uint8_t _x; // The x position of this object.
    uint8_t _y; // The y position of this object.
    uint8_t _width; // The width of this object.
    uint8_t _height; // The height of this object.
    
private:
    DynamicGameObject *_nextObject; // The next object to build a simple list.
};
    

}



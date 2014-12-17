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

#include "GameObject.h"


namespace lr {
    
    
/// A simple and fast array of game objects.
///
class GameObjectArray
{
public:
    /// The type for the size used for the array.
    typedef uint8_t SizeType;
    
public:
    /// Create a new game object array with the given maximum size.
    GameObjectArray(SizeType maxSize);
    
    /// Destroy the array and free the objects.
    ~GameObjectArray();
    
public:
    /// Get the size of this array.
    inline SizeType getSize() const { return _size; }
    
    /// Get the maximum size of this array.
    inline SizeType getMaximumSize() const { return _maximumSize; }
    
    /// Get the game object at the given position.
    inline const GameObject& getGameObject(const SizeType index) const { return _gameObjects[index]; }
    
    /// Add an object to the end of the array
    void addObject(const GameObject &gameObject);
    
    /// Remove an object at the given position.
    void removeObject(const SizeType index);
    
private:
    SizeType _maximumSize;
    SizeType _size;
    GameObject *_gameObjects;
};

    
}
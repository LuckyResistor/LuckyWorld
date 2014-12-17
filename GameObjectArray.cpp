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
#include "GameObjectArray.h"


namespace lr {
    

GameObjectArray::GameObjectArray(SizeType maxSize)
    : _maximumSize(maxSize), _size(0), _gameObjects(new GameObject[maxSize])
{
}

    
GameObjectArray::~GameObjectArray()
{
    delete[] _gameObjects;
}

    
void GameObjectArray::addObject(const GameObject &gameObject)
{
    if (_size == _maximumSize) {
        return; // ignore this call
    }
    _gameObjects[_size] = gameObject;
    ++_size;
}

    
void GameObjectArray::removeObject(const SizeType index)
{
    // check if we remove the last object.
    if (index+1 == _size) {
        // in this case just decrease the size
        --_size;
    } else if (index < _size) { // check if this is a valid index.
        // move all objects after the index.
        memmove(&_gameObjects[index], &_gameObjects[index+1], ((_size-index-1) * 3));
        --_size;
    }
}
    
    
}
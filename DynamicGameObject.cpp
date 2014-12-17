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

#include "DynamicGameObject.h"


#include "MovingPlatformObject.h"
#include "JumpingFireballObject.h"
#include "CanonObject.h"
#include "MonsterObject.h"
#include "PortalObject.h"


namespace lr {
    
    
LogicResult::LogicResult(Result result, DynamicGameObject *gameObject)
    : _result(result)
{
    _values.gameObject = gameObject;
}

    
LogicResult::LogicResult(const uint8_t nextLevelIndex)
    : _result(NextLevel)
{
    _values.nextLevelIndex = nextLevelIndex;
}

    
LogicResult::~LogicResult()
{
}

    
DynamicGameObject::DynamicGameObject(const GameObject &gameObject)
    : _x(gameObject.getX()), _y(gameObject.getY()), _width(1), _height(1)
{
}

    
DynamicGameObject::DynamicGameObject(const uint8_t x, const uint8_t y)
    : _x(x), _y(y), _width(1), _height(1)
{
}


DynamicGameObject::~DynamicGameObject()
{
}

    
bool DynamicGameObject::doesRenderToDisplay() const
{
    return false;
}
    
    
void DynamicGameObject::renderToDisplay(const uint8_t, const uint8_t, const uint32_t&) const
{
}

    
DynamicGameObject* DynamicGameObject::createFromGameObject(const GameObject &gameObject)
{
    switch (gameObject.getType()) {
        case GameObject::MovingPlatform:
            return new MovingPlatformObject(gameObject);
            
        case GameObject::JumpingFireball:
            return new JumpingFireballObject(gameObject);
            
        case GameObject::Canon:
            return new CanonObject(gameObject);
            
        case GameObject::Monster:
            return new MonsterObject(gameObject);
            
        case GameObject::Portal:
            return new PortalObject(gameObject);
            
        default:
            break;
    }
    return 0;
}

    
void DynamicGameObject::moveToDirection(const GameObject::Direction direction, uint8_t &x, uint8_t &y)
{
    switch (direction) {
        case GameObject::DirectionNorth: ++y; break;
        case GameObject::DirectionNorthEast: ++y; ++x; break;
        case GameObject::DirectionEast: ++x; break;
        case GameObject::DirectionSouthEast: --y; ++x; break;
        case GameObject::DirectionSouth: --y; break;
        case GameObject::DirectionSouthWest: --y; --x; break;
        case GameObject::DirectionWest: --x; break;
        case GameObject::DirectionNorthWest: ++y; --x; break;
        default: break;
    }
}

    
}



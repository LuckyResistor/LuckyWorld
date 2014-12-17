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


#include <avr/pgmspace.h>


namespace lr {
 
    
GameObject::GameObject()
    : _byte0(0), _byte1(0), _byte2(0)
{
}

    
GameObject::GameObject(uint8_t byte0, uint8_t byte1, uint8_t byte2)
    : _byte0(byte0), _byte1(byte1), _byte2(byte2)
{
}
    
    
GameObject GameObject::fromLevelData(const uint8_t *levelData)
{
    return GameObject(
        pgm_read_byte(levelData),
        pgm_read_byte(levelData+1),
        pgm_read_byte(levelData+2)
    );
}
    
    
uint8_t GameObject::getWidth() const
{
    switch (getType()) {
        case Ground:
        case Water:
        case Lava:
        case GroundSteps:
        case UnstableGround:
            return (_byte2 & 0x0f) + 1;
            
        case Coin:
            return ((_byte2 & B00000111) + 1) * (((_byte2 & B11000000) >> 6) + 1);
 
        case MovingPlatform:
            if ((_byte2 & 0x80) == 0) {
                return ((_byte2 & 0x03)+1)*4;
            } else {
                if ((_byte2 & 0x20) == 0) {
                    return 1;
                } else {
                    return 3;
                }
            }
            
        case Monster:
            return ((_byte2 & B00001100) >> 2) + 1;
            
        case Portal:
            if ((_byte2 & B01000000) == 0) {
                return 2;
            } else {
                return 3;
            }
            
        default:
            return 1;
    }
}

    
uint8_t GameObject::getHeight() const
{
    switch (getType()) {
        case Ground:
        case Water:
        case Lava:
            return (_byte2 >> 4) + 1;
            
        case GroundSteps:
            return (_byte2 & 0x0f) + 1;
            
        case Coin:
            return (((_byte2 & B00111000) >> 3) + 1) * (((_byte2 & B11000000) >> 6) + 1);
            
        case MovingPlatform:
            if ((_byte2 & 0x80) == 0) {
                return 1;
            } else {
                return ((_byte2 & 0x03)+1)*4;
            }
            
        case JumpingFireball:
            return (_byte2 & 0x0f);

        case Monster:
            return ((_byte2 & B00001100) >> 2) + 1;
            
        case Portal:
            return 3;
            
        default:
            return 1;
    }
}

                    
uint8_t GameObject::getSpacing() const
{
    if (getType() == Coin) {
        return ((_byte2 & B11000000) >> 6);
    } else {
        return 0;
    }
}
                    
    
GameObject::Direction GameObject::getDirection() const
{
    if (getType() == GroundSteps) {
        switch ((_byte2 >> 4) & 0x3) {
            case 0: return DirectionSouthEast;
            case 1: return DirectionSouthWest;
            case 2: return DirectionNorthWest;
            case 3: return DirectionNorthEast;
        }
    } else if (getType() == MovingPlatform) {
        if (getOrientation() == OrientationHorizontal) {
            if ((_byte2 & B01000000) == 0) {
                return DirectionWest;
            } else {
                return DirectionEast;
            }
        } else {
            if ((_byte2 & B01000000) == 0) {
                return DirectionNorth;
            } else {
                return DirectionSouth;
            }
        }
    } else if (getType() == Canon) {
        return static_cast<Direction>((_byte2 & B00011100) >> 2);
    }
    return NoDirection;
}
    
    
GameObject::Orientation GameObject::getOrientation() const
{
    if (getType() == MovingPlatform) {
        if ((_byte2 & 0x80) == 0) {
            return OrientationHorizontal;
        } else {
            return OrientationVertical;
        }
    }
    return NoOrientation;
}
    
    
uint8_t GameObject::getPlatformWidth() const
{
    if (getType() == MovingPlatform) {
        if ((_byte2 & B00100000) == 0) {
            return 1;
        } else {
            return 3;
        }
    }
    return 0;
}
    
    
GameObject::Movement GameObject::getMovement() const
{
    if (getType() == MovingPlatform) {
        if ((_byte2 & B00000100) == 0) {
            return MovementContinuous;
        } else {
            return MovementBounce;
        }
    } else if (getType() == Monster) {
        if ((_byte2 & B00010000) == 0) {
            return MovementContinuous;
        } else {
            return MovementBounce;
        }
    }
    return NoMovement;
}

    
uint8_t GameObject::getSpeed() const
{
    switch (getType()) {
        case MovingPlatform:
            return ((_byte2 & B00011000) >> 3);
        
        case JumpingFireball:
            return ((_byte2 & B00110000) >> 4);
            
        case Canon:
        case Monster:
            return _byte2 & B00000011;
            
        default:
            break;
    }
    return 0;
}

    
uint8_t GameObject::getPhaseShift() const
{
    if (getType() == JumpingFireball) {
        return (_byte2 >> 6);
    }
    return 0;
}

    
uint8_t GameObject::getInterval() const
{
    if (getType() == Canon) {
        return (_byte2 >> 5);
    }
    return 0;
}

    
bool GameObject::isHidden() const
{
    if (getType() == Portal) {
        return (_byte2 & B10000000) != 0;
    }
    return false;
}

    
uint8_t GameObject::getNextLevelIndex() const
{
    if (getType() == Portal) {
        return (_byte2 & B00111111);
    }
    return 0;
}

    
bool GameObject::isDynamic() const
{
    switch (getType()) {
        case Ground:
        case Water:
        case Lava:
        case Coin:
        case GroundSteps:
            return false;
            
        default:
            return true;
    }
}
    
    
}
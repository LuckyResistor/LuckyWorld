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

#include "DynamicGameObject.h"


namespace lr {

    
/// A moving platform.
///
class MovingPlatformObject : public DynamicGameObject
{
private:
    enum Flags : uint8_t {
        OrientationMask = _BV(0),
        OrientationHorizontal = 0,
        OrientationVertical = OrientationMask,
        
        MovingMask = _BV(1),
        MovingContinuous = 0,
        MovingBounce = MovingMask,
        
        CurrentDirectionMask = _BV(2),
        CurrentDirectionPlus = 0,
        CurrentDirectionMinus = CurrentDirectionMask,
    };
    
public:
    /// Creates a moving platfrom object.
    ///
    MovingPlatformObject(const GameObject &gameObject);
   
    /// dtor
    ///
    virtual ~MovingPlatformObject();
    
public: // Implement DynamicGameObject
    virtual GameObject::Type getType() const;
    virtual LogicResult logic(GameCanvas &canvas, Player &player, const uint32_t &frame);
    virtual void render(GameCanvas &canvas, const uint32_t &frame) const;
    
private:
    inline bool isHorizontal() const {
        return (_flags & OrientationMask) == OrientationHorizontal;
    }
    inline bool isCurrentDirectionPlus() const {
        return (_flags & CurrentDirectionMask) == CurrentDirectionPlus;
    }
    inline bool doesBounce() const {
        return (_flags & MovingMask) == MovingBounce;
    }
    void advancePosition();
    void flipDirection();
    LogicResult movePlayerX(GameCanvas &canvas, Player &player);
    LogicResult movePlayerY(GameCanvas &canvas, Player &player);
    
private:
    uint8_t _flags;
    uint8_t _platformWidth;
    uint8_t _speed;
    uint8_t _currentPosition;
    uint32_t _lastMove;
};
    

}



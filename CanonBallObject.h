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

    
/// Represents a canon ball object.
///
class CanonBallObject : public DynamicGameObject
{
public:
    /// Create a new canon ball object.
    ///
    /// @param x The start x coordinate.
    /// @param y The start y coordinate.
    /// @param speed The speed of the canon ball.
    /// @param direction The direction of the canon ball.
    /// @param frame The initial frame to time the speed.
    ///
    CanonBallObject(const uint8_t x, const uint8_t y, const uint8_t speed, const GameObject::Direction direction, const uint32_t &frame);
   
    /// dtor
    ///
    virtual ~CanonBallObject();
    
public: // Implement DynamicGameObject
    virtual GameObject::Type getType() const;
    virtual LogicResult logic(GameCanvas &canvas, Player &player, const uint32_t &frame);
    virtual void render(GameCanvas &canvas, const uint32_t &frame) const;
    virtual bool doesRenderToDisplay() const;
    virtual void renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const;
    
private:
    uint32_t _frame;
    uint8_t _speed;
    GameObject::Direction _direction;
};
    

}



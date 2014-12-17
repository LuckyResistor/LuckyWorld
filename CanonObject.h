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

    
/// Represents a canon object
///
class CanonObject : public DynamicGameObject
{
public:
    /// Create a new canon object
    ///
    CanonObject(const GameObject &gameObject);
   
    /// dtor
    ///
    virtual ~CanonObject();
    
public: // Implement DynamicGameObject
    virtual GameObject::Type getType() const;
    virtual LogicResult logic(GameCanvas &canvas, Player &player, const uint32_t &frame);
    virtual void render(GameCanvas &canvas, const uint32_t &frame) const;
    virtual bool doesRenderToDisplay() const;
    virtual void renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const;
   
private:
    GameObject::Direction _direction;
    uint16_t _interval;
    uint8_t _phase;
    uint8_t _speed;
};
    

}



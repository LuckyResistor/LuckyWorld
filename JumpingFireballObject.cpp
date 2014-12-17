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

#include "JumpingFireballObject.h"


#include "GameCanvas.h"


namespace lr {
    

namespace {
const uint8_t jumpTableSize = 128;
const uint8_t jumpTable[] PROGMEM = {
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a,
    0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0e,
    0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0e, 0x0e,
    0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0d, 0x0d, 0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0b, 0x0b,
    0x0b, 0x0a, 0x0a, 0x0a, 0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x07, 0x07, 0x07, 0x06, 0x06,
    0x06, 0x05, 0x05, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00};
const uint8_t jumpPositionInvisible = 0xff;
}
    
    
JumpingFireballObject::JumpingFireballObject(const GameObject &gameObject)
    : DynamicGameObject(gameObject), _yPosition(jumpPositionInvisible)
{
    _height = gameObject.getHeight();
    switch (gameObject.getSpeed()) {
        case 0: _phaseSpeed = 240; _fireSpeed = 1; break;
        case 1: _phaseSpeed = 160; _fireSpeed = 1; break;
        case 2: _phaseSpeed = 120; _fireSpeed = 2; break;
        case 3: _phaseSpeed = 80; _fireSpeed = 2; break;
    }
    _phaseShift = (_phaseSpeed/4*gameObject.getPhaseShift());
}

    
JumpingFireballObject::~JumpingFireballObject()
{
}

    
GameObject::Type JumpingFireballObject::getType() const
{
    return GameObject::JumpingFireball;
}
    
    
LogicResult JumpingFireballObject::logic(GameCanvas &canvas, Player &player, const uint32_t &frame)
{
    const uint8_t phase = ((frame + _phaseShift) % _phaseSpeed);
    if (phase < (jumpTableSize/_fireSpeed)) {
        _yPosition = pgm_read_byte(&jumpTable[phase*_fireSpeed]) * _height / 16;
    } else {
        _yPosition = jumpPositionInvisible;
    }
    return LogicResult::Continue;
}
    

void JumpingFireballObject::render(GameCanvas &canvas, const uint32_t &frame) const
{
    // clear the fireball area
    for (uint8_t y = 0; y < _height; ++y) {
        canvas.setBlockAt(_x, _y+y, GameCanvas::Empty);
    }
    // draw the fireball
    if (_yPosition != jumpPositionInvisible) {
        canvas.setBlockAt(_x, _y+_yPosition, GameCanvas::Lava);
    }
}
    
    
}



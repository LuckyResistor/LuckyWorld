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

#include "CanonObject.h"


#include "GameCanvas.h"
#include "CanonBallObject.h"

#include <LRMeggyJr.h>


namespace lr {
    
    
namespace {
const uint16_t firePoint = 60;
}
    
    
CanonObject::CanonObject(const GameObject &gameObject)
    : DynamicGameObject(gameObject)
{
    switch (gameObject.getSpeed()) {
        case 0: _speed = 30; break;
        case 1: _speed = 22; break;
        case 2: _speed = 15; break;
        case 3: _speed = 10; break;
    }
    switch (gameObject.getInterval()) {
        case 0: _interval = 6*60; break;
        case 1: _interval = 5*60+30; break;
        case 2: _interval = 5*60; break;
        case 3: _interval = 4*60+30; break;
        case 4: _interval = 4*60; break;
        case 5: _interval = 3*60+30; break;
        case 6: _interval = 3*60; break;
        case 7: _interval = 2*60; break;
    }
    _direction = gameObject.getDirection();
}

    
CanonObject::~CanonObject()
{
}
    
    
GameObject::Type CanonObject::getType() const
{
    return GameObject::JumpingFireball;
}


LogicResult CanonObject::logic(GameCanvas &canvas, Player &player, const uint32_t &frame)
{
    _phase = 0;
    uint16_t phase = (frame % _interval);
    if (phase == firePoint) {
        // fire the canon
        uint8_t startX = _x;
        uint8_t startY = _y;
        moveToDirection(_direction, startX, startY);
        return LogicResult(LogicResult::AddObject, new CanonBallObject(startX, startY, _speed, _direction, frame));
    } else if (phase < firePoint) {
        _phase = static_cast<uint8_t>(phase);
    }
    return LogicResult::Continue;
}


void CanonObject::render(GameCanvas &canvas, const uint32_t &frame) const
{
    canvas.setBlockAt(_x, _y, GameCanvas::Canon);
}
  
    
bool CanonObject::doesRenderToDisplay() const
{
    return true;
}
    
    
void CanonObject::renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const
{
    if ((_phase & 0x3) > 1) {
        meg.setPixelS(_x-viewX, 7-(_y-viewY), Color(0xf, 0x2, 0x4));
    }
}

    
}



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

#include "CanonBallObject.h"


#include "Player.h"
#include "GameCanvas.h"

#include <LRMeggyJr.h>


namespace lr {
    
    
CanonBallObject::CanonBallObject(const uint8_t x, const uint8_t y, const uint8_t speed, const GameObject::Direction direction, const uint32_t &frame)
    : DynamicGameObject(x, y), _frame(frame), _speed(speed), _direction(direction)
{
}

    
CanonBallObject::~CanonBallObject()
{
}
 
    
GameObject::Type CanonBallObject::getType() const
{
    return GameObject::CanonBall;
}
    
    
LogicResult CanonBallObject::logic(GameCanvas &canvas, Player &player, const uint32_t &frame)
{
    if (frame-_frame > _speed) {
        _frame = frame;
        // move the ball
        moveToDirection(_direction, _x, _y);
        // Collision check with the player object.
        if (player.getLevelX() == _x && player.getLevelY() == _y) {
            return LogicResult::KilledPlayer;
        }
        // Check if the canon ball hit something solid
        if (canvas.isSolidGround(_x, _y)) {
            return LogicResult::Remove;
        }
        // Check if the canon ball left the canvas.
        if (_x < canvas.getLeftX() || _x >= canvas.getRightX() || _y >= canvas.getHeight()) {
            return LogicResult::Remove;
        }
    }
    return LogicResult::Continue;
}
    
    
void CanonBallObject::render(GameCanvas &canvas, const uint32_t &frame) const
{
    // we do not render this object into the canvas.
}
    
    
bool CanonBallObject::doesRenderToDisplay() const
{
    return true;
}
    
    
void CanonBallObject::renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const
{
    Color color = 0;
    if ((frame & 0x3) == 0) {
        color = Color(0xf, 0x2, 0x0);
    } else {
        color = Color(0xf, 0x0, 0x0);
    }
    meg.setPixelS(_x-viewX, 7-(_y-viewY), color);
}

    
}



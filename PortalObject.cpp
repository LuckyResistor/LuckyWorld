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

#include "PortalObject.h"


#include "Player.h"
#include "GameCanvas.h"

#include <LRMeggyJr.h>


namespace lr {
    
    
PortalObject::PortalObject(const GameObject &gameObject)
    : DynamicGameObject(gameObject)
{
    _height = gameObject.getHeight();
    _width = gameObject.getWidth();
    _isHiddenObject = gameObject.isHidden();
    _nextLevel = gameObject.getNextLevelIndex();
}

    
PortalObject::~PortalObject()
{
    
}

    
GameObject::Type PortalObject::getType() const
{
    return GameObject::JumpingFireball;
}


LogicResult PortalObject::logic(GameCanvas &canvas, Player &player, const uint32_t &frame)
{
    // check if the portal is visible to the player.
    const uint8_t playerX = player.getLevelX();
    const uint8_t playerY = player.getLevelY();
    if (_isHiddenObject) {
        if (abs((_x+1)-playerX) < 4 && abs((_y+1)-playerY) < 4) {
            _isVisible = true;
        } else {
            _isVisible = false;
        }
    } else {
        _isVisible = true;
    }
    // check if the player entered the portal
    if ((playerY == _y || playerY == (_y+1)) && playerX == (_x+1)) {
        return LogicResult(_nextLevel);
    }
    return LogicResult::Continue;
}


void PortalObject::render(GameCanvas &canvas, const uint32_t &frame) const
{
    // nothing to render
}


bool PortalObject::doesRenderToDisplay() const
{
    return true;
}


void PortalObject::renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const
{
    if (_isVisible) {
        const uint8_t phase = (frame & 0x1f);
        const int8_t x = _x-viewX;
        const int8_t y = 7-(_y-viewY);
        Color color = Color(0);
        if (phase < 0x10) {
            color = Color(0x0, 0x0, phase);
        } else {
            color = Color(0x0, 0x0, 0x1f-phase);
        }
        meg.fillRectS(x, y-2, _width, _height, color);
        meg.fillRectS(x+1, y-1, 1, 2, Color::black());
    }
}

    
}



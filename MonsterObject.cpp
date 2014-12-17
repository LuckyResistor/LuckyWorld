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

#include "MonsterObject.h"


#include "GameCanvas.h"
#include "Player.h"

#include <LRMeggyJr.h>


namespace lr {
    
    
MonsterObject::MonsterObject(const GameObject &gameObject)
    : DynamicGameObject(gameObject), _frame(0), _speed(30), _direction(GameObject::DirectionWest)
{
    _width = gameObject.getWidth();
    _height = gameObject.getHeight();
    _movement = gameObject.getMovement();
    switch (gameObject.getSpeed()) {
        case 0: _speed = 30; break;
        case 1: _speed = 22; break;
        case 2: _speed = 15; break;
        case 3: _speed = 10; break;
    }
}

    
MonsterObject::~MonsterObject()
{
}
    
    
GameObject::Type MonsterObject::getType() const
{
    return GameObject::Monster;
}

    
LogicResult MonsterObject::logic(GameCanvas &canvas, Player &player, const uint32_t &frame)
{
    if (_frame == 0) {
        _frame = frame;
    }
    // check if the monster is falling
    bool isFalling = true;
    if (_y > 0) {
        for (uint8_t x = 0; x < _width; ++x) {
            if (canvas.isSolidGround(_x+x, _y-1)) {
                isFalling = false;
                break;
            }
        }
    }
    if (isFalling) {
        // falling
        if (frame-_frame > 10) { // fixed falling speed
            _frame = frame;
            --_y;
        }
    } else {
        if (frame-_frame > _speed) {
            _frame = frame;
            // normal movement
            uint8_t newX = _x;
            uint8_t newY = _y;
            if (_movement == GameObject::MovementBounce) {
                if (_direction == GameObject::DirectionWest && _x == 0) {
                    _direction = GameObject::DirectionEast;
                }
                if (_direction == GameObject::DirectionEast && _x == canvas.getLastObjectPositionX()) {
                    return LogicResult::Remove;
                }
            } else {
                if (_direction == GameObject::DirectionWest && _x == 0) {
                    return LogicResult::Remove;
                }
                if (_direction == GameObject::DirectionEast && _x == canvas.getLastObjectPositionX()) {
                    return LogicResult::Remove;
                }
            }
            if (_direction == GameObject::DirectionWest) {
                --newX;
            } else {
                ++newX;
            }
            // check for collision
            bool hasCollision = false;
            for (uint8_t x = 0; x < _width; ++x) {
                for (uint8_t y = 0; y < _height; ++y) {
                    if (canvas.isSolidGround(newX+x, newY+y)) {
                        hasCollision = true;
                        break;
                    }
                }
                if (hasCollision) {
                    break;
                }
            }
            if (hasCollision) {
                if (_direction == GameObject::DirectionWest) {
                    _direction = GameObject::DirectionEast;
                } else {
                    _direction = GameObject::DirectionWest;
                }
            } else {
                if (_movement == GameObject::MovementBounce) {
                    // check if there is the risk of a fall
                    bool isFallRisk = false;
                    for (uint8_t x = 0; x < _width; ++x) {
                        if (!canvas.isSolidGround(newX+x, _y-1)) {
                            isFallRisk = true;
                            break;
                        }
                    }
                    if (isFallRisk) {
                        if (_direction == GameObject::DirectionWest) {
                            _direction = GameObject::DirectionEast;
                        } else {
                            _direction = GameObject::DirectionWest;
                        }
                    }
                } else {
                    _x = newX;
                    _y = newY;
                }
            }
        }
    }
    // check for collision with player.
    const uint8_t playerX = player.getLevelX();
    const uint8_t playerY = player.getLevelY();
    if (playerX >= _x && playerX < (_x+_width) && playerY >= _y && playerY < (_y+_width)) {
        return LogicResult::KilledPlayer;
    }
    
    return LogicResult::Continue;
}

    
void MonsterObject::render(GameCanvas &canvas, const uint32_t &frame) const
{
    // do not render to canvas directly.
}

    
bool MonsterObject::doesRenderToDisplay() const
{
    return true;
}

    
void MonsterObject::renderToDisplay(const uint8_t viewX, const uint8_t viewY, const uint32_t &frame) const
{
    Color color = 0;
    if ((frame & 0x3) == 0) {
        color = Color(0xc, 0x2, 0x0);
    } else {
        color = Color(0xc, 0x0, 0x0);
    }
    meg.fillRectS(_x-viewX, 8-(_y-viewY)-_height, _width, _height, color);
}

    
}



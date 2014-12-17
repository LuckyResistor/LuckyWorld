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

#include "MovingPlatformObject.h"


#include "GameCanvas.h"
#include "Player.h"


namespace lr {
    
    
MovingPlatformObject::MovingPlatformObject(const GameObject &gameObject)
    : DynamicGameObject(gameObject), _flags(0), _currentPosition(0), _lastMove(0)
{
    if (gameObject.getOrientation() == GameObject::OrientationHorizontal) {
        _flags |= OrientationHorizontal;
        if (gameObject.getDirection() == GameObject::DirectionEast) {
            _flags |= CurrentDirectionMinus;
        } else {
            _flags |= CurrentDirectionPlus;
        }
    } else {
        _flags |= OrientationVertical;
        if (gameObject.getDirection() == GameObject::DirectionSouth) {
            _flags |= CurrentDirectionMinus;
        } else {
            _flags |= CurrentDirectionPlus;
        }
    }
    _width = gameObject.getWidth();
    _height = gameObject.getHeight();
    _platformWidth = gameObject.getPlatformWidth();
    switch (gameObject.getSpeed()) {
        case 0: _speed = 24; break;
        case 1: _speed = 16; break;
        case 2: _speed = 12; break;
        case 3: _speed = 8; break;
    }
    if (gameObject.getMovement() == GameObject::MovementContinuous) {
        _flags |= MovingContinuous;
    } else {
        _flags |= MovingBounce;
    }
}

    
MovingPlatformObject::~MovingPlatformObject()
{
}
 
    
GameObject::Type MovingPlatformObject::getType() const
{
    return GameObject::MovingPlatform;
}

    
LogicResult MovingPlatformObject::logic(GameCanvas &canvas, Player &player, const uint32_t &frame)
{
    // check if we need to move the platform.
    if (_lastMove == 0 || (frame - _lastMove) > _speed) {
        _lastMove = frame;
        // Move the platform and the player
        if (isHorizontal()) {
            if (doesBounce()) {
                if (isCurrentDirectionPlus() && _currentPosition == (_width - _platformWidth)) {
                    flipDirection();
                } else if (!isCurrentDirectionPlus() && _currentPosition == 0) {
                    flipDirection();
                }
            }
            const LogicResult result = movePlayerX(canvas, player);
            if (result == LogicResult::Continue) {
                advancePosition();
            }
            return result;
        } else {
            if (doesBounce()) {
                if (isCurrentDirectionPlus() && _currentPosition == (_height-1)) {
                    flipDirection();
                } else if (!isCurrentDirectionPlus() && _currentPosition == 0) {
                    flipDirection();
                }
            }
            const LogicResult result = movePlayerY(canvas, player);
            if (result == LogicResult::Continue) {
                advancePosition();
            }
            return result;
        }
    }
    return LogicResult::Continue;
}
    
    
void MovingPlatformObject::advancePosition()
{
    if (isHorizontal()) {
        if (isCurrentDirectionPlus()) {
            if (++_currentPosition > _width) {
                _currentPosition = 0;
            }
        } else {
            if (_currentPosition == 0) {
               _currentPosition = _width-1;
            } else {
                --_currentPosition;
            }
        }
    } else {
        if (isCurrentDirectionPlus()) {
            if (++_currentPosition >= _height) {
                _currentPosition = 0;
            }
        } else {
            if (_currentPosition == 0) {
                _currentPosition = _height-1;
            } else {
                --_currentPosition;
            }
        }
    }
}

    
void MovingPlatformObject::flipDirection()
{
    if (isCurrentDirectionPlus()) {
        _flags = ((_flags & ~(CurrentDirectionMask)) | CurrentDirectionMinus);
    } else {
        _flags = ((_flags & ~(CurrentDirectionMask)) | CurrentDirectionPlus);
    }
}

    
LogicResult MovingPlatformObject::movePlayerX(GameCanvas &canvas, Player &player)
{
    // check if the player is affected
    const uint8_t playerY = player.getLevelY();
    const uint8_t playerX = player.getLevelX();
    bool movePlayer = false;
    if (playerY == (_y+1)) { // on top of the platform?
        for (uint8_t i = 0; i < _platformWidth; ++i) {
            const uint8_t x = ((_currentPosition + i) % _width) + _x;
            if (playerX == x) {
                movePlayer = true;
                break;
            }
        }
        if (movePlayer) {
            if (isCurrentDirectionPlus()) {
                // any obstacles in the way?
                if (playerX!=0xff && !canvas.isSolidGround(playerX+1, playerY)) {
                    // no, move the player
                    player.setX(player.getX()+player.getPositionResolution());
                }
            } else {
                // any obstacles in the way?
                if (playerX!=0x00 && !canvas.isSolidGround(playerX-1, playerY)) {
                    // no, move the player
                    player.setX(player.getX()-player.getPositionResolution());
                }
            }
        }
    } else if (playerY == _y) { // pushed by the platform? danger!
        if (isCurrentDirectionPlus()) {
            const uint8_t x = ((_currentPosition + _platformWidth) % _width) + _x;
            if (playerX == x) {
                // any obstacles in the way?
                if (playerX == 0xff || canvas.isSolidGround(playerX+1, playerY)) {
                    // yes, the player dies
                    return LogicResult::KilledPlayer;
                } else {
                    // no, move the player
                    player.setX(player.getX()+player.getPositionResolution());
                }
            }
        } else {
            const uint8_t x = ((_currentPosition - 1) % _width) + _x;
            if (playerX == x) {
                // any obstacles in the way?
                if (playerX == 0x00 || canvas.isSolidGround(playerX-1, playerY)) {
                    // yes, the player dies
                    return LogicResult::KilledPlayer;
                } else {
                    // no, move the player
                    player.setX(player.getX()-player.getPositionResolution());
                }
            }
        }
    }
    return LogicResult::Continue;
}

    
LogicResult MovingPlatformObject::movePlayerY(GameCanvas &canvas, Player &player)
{
    const uint8_t playerY = player.getLevelY();
    const uint8_t playerX = player.getLevelX();
    if (playerY != (_y+_currentPosition+1) || playerX < _x || playerX > (_x+_platformWidth-1)) {
        return LogicResult::Continue;
    }
    if (isCurrentDirectionPlus()) {
        // any obstacles in the way?
        if (playerY == (canvas.getHeight()-1) || canvas.isSolidGround(playerX, playerY+1)) {
            // yes, the player dies
            return LogicResult::KilledPlayer;
        } else {
            player.setY(player.getY()+player.getPositionResolution());
        }
    } else {
        player.setY(player.getY()-player.getPositionResolution());
    }
    return LogicResult::Continue;
}

    
void MovingPlatformObject::render(GameCanvas &canvas, const uint32_t &frame) const
{
    if (isHorizontal()) {
        // first erase the whole area
        for (uint8_t x = 0; x < _width; ++x) {
            canvas.setBlockAt(_x+x, _y, GameCanvas::Empty);
        }
        // now draw the platform
        for (uint8_t i = 0; i < _platformWidth; ++i) {
            uint8_t x = _currentPosition + i;
            if (x >= _width) {
                x -= _width;
            }
            canvas.setBlockAt(_x + x, _y, GameCanvas::Platform);
        }
    } else {
        // first erase the whole area
        if (_platformWidth == 1) {
            const uint8_t maxY = _y+_height;
            for (uint8_t y = _y; y != maxY; ++y) {
                canvas.setBlockAt(_x, y, GameCanvas::Empty);
            }
        } else {
            const uint8_t maxY = _y+_height;
            for (uint8_t y = _y; y != maxY; ++y) {
                canvas.setBlockAt(_x, y, GameCanvas::Empty);
                canvas.setBlockAt(_x+1, y, GameCanvas::Empty);
                canvas.setBlockAt(_x+2, y, GameCanvas::Empty);
            }
        }
        // now draw the platform
        const uint8_t y = _y + _currentPosition;
        for (uint8_t x = 0; x < _width; ++x) {
            canvas.setBlockAt(_x + x, y, GameCanvas::Platform);
        }
    }
}
 
    
}



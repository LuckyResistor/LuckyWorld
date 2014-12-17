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

#include "Player.h"


#include "GameCanvas.h"
#include "GameState.h"
#include "Sounds.h"

#include <LRMeggyJr.h>


namespace lr {
  
    
const uint32_t kPlayerJumpTime = 15; // in frames.
    
    
Player::Player()
:
    _positionResolution(16),
    _maximumSpeed(6),
    _speedReduction(3),
    _x(0),
    _y(0),
    _speedX(0),
    _speedY(0),
    _actionState(ActionRun),
    _jumpMaxY(0),
    _jumpStartFrame(0)
{
}

    
Player::~Player()
{
}

    
void Player::setLevelPos(uint8_t x, uint8_t y)
{
    setLevelX(x);
    setLevelY(y);
}
    
    
void Player::renderPlayer(uint8_t viewX, uint8_t viewY, const uint32_t &frame)
{
    const uint8_t tx = getLevelX()-viewX;
    const uint8_t ty = 7-(getLevelY()-viewY);
    if (tx < 8 && ty < 8) {
        if ((frame & 0x7f) == 0) {
            meg.setPixel(tx, ty, Color::maximum());
        } else {
            meg.setPixel(tx, ty, Color(0xf, 0x7, 0x8));
        }
    }
}

    
void Player::renderDeathPlayer(uint8_t viewX, uint8_t viewY, const uint32_t &frame)
{
    const uint8_t tx = getLevelX()-viewX;
    const uint8_t ty = 7-(getLevelY()-viewY);
    if (tx < 8 && ty < 8) {
        if ((frame & 0x0f) < 8) {
            meg.setPixel(tx, ty, Color(0xf, 0x2, 0x2));
        } else {
            meg.setPixel(tx, ty, Color(0xf, 0x7, 0x8));
        }
    }
}

    
bool Player::logic(const uint32_t &frame, GameState *gameState)
{
    // The logic for the X movement.
    
    // Get the current maximum speed.
    Speed maximumSpeed = _maximumSpeed;
    if (!meg.isBButtonDown()) {
        maximumSpeed -= 2;
        if (_speedX > maximumSpeed) {
            _speedX = maximumSpeed;
        } else if (_speedX < -maximumSpeed) {
            _speedX = -maximumSpeed;
        }
    }
    
    // Increase the speed on button press but only every 4 frames.
    if ((frame & 3) == 0) {
        if (meg.isRightButtonDown() && _speedX < _maximumSpeed) {
            if (_speedX < 0) {
                _speedX += _speedReduction + 1;
            } else {
                ++_speedX;
            }
        }
        if (meg.isLeftButtonDown() && _speedX > (-_maximumSpeed)) {
            if (_speedX > 0) {
                _speedX -= _speedReduction + 1;
            } else {
                --_speedX;
            }
        }
        
        if (!meg.isRightButtonDown() && !meg.isLeftButtonDown()) {
            if (_speedX > _speedReduction) {
                _speedX -= _speedReduction;
            } else if (_speedX < -_speedReduction) {
                _speedX += _speedReduction;
            } else {
                _speedX = 0;
            }
        }
    }
    
    // If there is an obstacle, reduce the speed to zero.
    if (_speedX > 0 && _gameCanvas->isSolidGround(getLevelX()+1, getLevelY())) {
        _speedX = 0;
        setMiddleX(); // back to middle position.
    }
    if (_speedX < 0 && _gameCanvas->isSolidGround(getLevelX()-1, getLevelY())) {
        _speedX = 0;
        setMiddleX(); // back to middle position.
    }
    
    // move the player.
    _x += _speedX;
    if (_x < 0) _x = 0;
    if (_x/_positionResolution > 256) _x = (255*_positionResolution);
    
    // The logic for the Y movement
    
    if (_actionState == ActionRun) {
        // no jump
        if (getLevelY() > 0 && _gameCanvas->isSolidGround(getLevelX(), getLevelY()-1)) {
            // the player stands on solid ground
            _speedY = 0;
            setMiddleY(); // back to middle position.
            // At this state a jump is possible if there is no ground above the player.
            if (meg.isAButtonPressed() && !_gameCanvas->isSolidGround(getLevelX(), getLevelY()+1)) {
                // calculate the maximum height of the jump
                _jumpMaxY = _y + _positionResolution*3;
                _jumpStartFrame = frame;
                _actionState = ActionJumpUp;
                meg.playSound(kSoundJump);
            }
        } else {
            // the player should fall
            if (_speedY > -_maximumSpeed) {
                --_speedY;
            }
        }
    }
    
    if (_actionState == ActionJumpUp) {
        if (meg.isAButtonReleased()) {
            // if the button is released, the jump ends immediately.
            _actionState = ActionRun;
            _speedY = 0;
        } else if (_y >= _jumpMaxY) {
            // if we reach the maximum height, speed drops to zero and we enter hover state.
            _actionState = ActionJumpHover;
            _speedY = 0;
            setMiddleY();
        } else if (_gameCanvas->isSolidGround(getLevelX(), getLevelY()+1)) {
            // if we touch solid ground, speed drops to zero and we enter hover state.
            _actionState = ActionJumpHover;
            _speedY = 0;
            setMiddleY();
        } else {
            // in any other case, calculate the up speed.
            _speedY = (_jumpMaxY - _y) / 4;
            if (_speedY == 0) {
                _actionState = ActionJumpHover;
                setMiddleY();
            }
        }
    } else if (_actionState == ActionJumpHover) {
        if (meg.isAButtonReleased()) {
            // if the button is released, the jump ends immediately.
            _actionState = ActionRun;
            _speedY = 0;
        } else if ((frame - _jumpStartFrame) > kPlayerJumpTime) {
            _actionState = ActionRun;
        }
    }
    
    _y += _speedY;
    if (_y < 0) {
        // die
        return false;
    }
    
    // everything is ok.
    return true;
}
    
    
}



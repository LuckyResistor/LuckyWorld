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
#include "GameCanvas.h"


#include "GameObjectArray.h"
#include "GameObject.h"

#include <LRMeggyJr.h>


namespace lr {
    

GameCanvas::GameCanvas(uint8_t width, uint8_t height)
:
    _width(width),
    _height(height),
    _rowSize(width>>1),
    _canvasSize(_rowSize*height),
    _canvas(new uint8_t[_canvasSize]),
    _x(0),
    _drawMinX(_width - 2),
    _lastX(256-_width)
{
    // Set the canvas to 0 (background).
    memset(_canvas, 0, _canvasSize);
}
    

GameCanvas::~GameCanvas()
{
    delete[] _canvas;
}

    
bool GameCanvas::isSolidGround(uint8_t x, uint8_t y) const
{
    if (x < _x || y >= _height || (x - _x) >= _width) {
        return true;
    }
    const Block block = getBlockAt(x,y);
    return block == Ground || block == Platform || block == Canon;
}

    
bool GameCanvas::isDeadly(uint8_t x, uint8_t y) const
{
    if (x < _x || y >= _height || (x - _x) >= _width) {
        return false;
    }
    const Block block = getBlockAt(x,y);
    return block == Water || block == Lava;
}
    
    
void GameCanvas::drawInitialStaticObjects(const lr::GameObjectArray &gameObjectArray)
{
    // start with an empty canvas.
    clear();
    
    // Set minimum to 0
    _drawMinX = 0;
    
    // Draw the initial level objects
    drawStaticObjects(gameObjectArray);
    
    // Set back to the correct value.
    _drawMinX = (_width - 2);
}
    
    
void GameCanvas::drawStaticObjects(const GameObjectArray &gameObjectArray)
{
    // Draw each object. To avoid virtual methods for the static game objects
    // the actual rendering is done here.
    const GameObjectArray::SizeType size = gameObjectArray.getSize();
    for (GameObjectArray::SizeType i = 0; i < size; ++i) {
        const GameObject &gameObject = gameObjectArray.getGameObject(i);
        switch (gameObject.getType()) {
            case GameObject::Ground:
                fillRect(gameObject.getX(), gameObject.getY(), gameObject.getWidth(), gameObject.getHeight(), Ground);
                break;
                
            case GameObject::Water:
                fillRect(gameObject.getX(), gameObject.getY(), gameObject.getWidth(), gameObject.getHeight(), Water);
                break;
                
            case GameObject::Lava:
                fillRect(gameObject.getX(), gameObject.getY(), gameObject.getWidth(), gameObject.getHeight(), Lava);
                break;
                
            case GameObject::GroundSteps:
            {
                const GameObject::Direction direction = gameObject.getDirection();
                const uint8_t x = gameObject.getX();
                const uint8_t y = gameObject.getY();
                const uint8_t width = gameObject.getWidth();
                const uint8_t height = gameObject.getHeight();
                for (uint8_t xd = 0; xd < width; ++xd) {
                    for (uint8_t yd = 0; yd < height; ++yd) {
                        if (direction == GameObject::DirectionSouthEast && xd < yd) continue;
                        if (direction == GameObject::DirectionNorthEast && xd < (height-yd-1)) continue;
                        if (direction == GameObject::DirectionNorthWest && xd > yd) continue;
                        if (direction == GameObject::DirectionSouthWest && xd > (height-yd-1)) continue;
                        const uint8_t tx = x + xd;
                        const uint8_t ty = y + yd;
                        setNewBlockAt(tx, ty, Ground);
                    }
                }
                break;
            }
                
            case GameObject::Coin:
                fillRectWithSpacing(gameObject.getX(), gameObject.getY(), gameObject.getWidth(), gameObject.getHeight(), Coin, gameObject.getSpacing());
                break;
                
            default:
                // ignore the rest.
                break;
        }
    }
}
    
    
void GameCanvas::clear()
{
    memset(_canvas, 0, _canvasSize);
}
    
    
void GameCanvas::scroll2Left()
{
    // Scroll the canvas by 2 positions.
    memmove(_canvas, _canvas+1, _canvasSize-1);
    
    // Fill the right area with empty blocks.
    for (uint8_t x = 0; x < 2; ++x) {
        for (uint8_t y = 0; y < _height; ++y) {
            setBlockAt(x+_x+_width-2, y, Empty);
        }
    }
    
    // Increase the left position by 2
    _x += 2;
}
    
    
void GameCanvas::renderCanvas(const uint8_t x, const uint8_t y, const uint32_t &frame)
{
    for (uint8_t xd = 0; xd < 8; ++xd) {
        for (uint8_t yd = 0; yd < 8; ++yd) {
            const uint8_t tx = x+xd;
            const uint8_t ty = y+yd;
            const Block block = getBlockAt(tx, ty);
            Color color(0,0,0);
            switch (block) {
                case Empty:
                    if (_theme == 0) {
                        color = Color(((y+yd)>>1), 0x0, 0x1);
                    } else {
                        color = Color(0, 0, 0);
                    }
                    break;
                    
                case Ground:
                {
                    // chessboard pattern
                    const bool brighter = (((x+xd) & 0x3) < 2);
                    // make the top blocks brighter.
                    if (ty<15&&getBlockAt(tx, ty+1)!=Ground) {
                        if (brighter) {
                            color = Color(0x0, 0x3, 0x0);
                        } else {
                            color = Color(0x4, 0x3, 0x0);
                        }
                    } else {
                        if (brighter) {
                            color = Color(0x0, 0x1, 0x0);
                        } else {
                            color = Color(0x2, 0x1, 0x0);
                        }
                    }
                }
                break;
                    
                case Platform:
                    // brigther green.
                    color = Color(0x4, 0x4, 0x0);
                    break;
                    
                case Water:
                {
                    const uint8_t phase = ((ty*5)+tx+(7-((frame&0x38)>>3))) & 0x07;
                    switch (phase) {
                        case 0: color = Color(0x0, 0x1, 0x1); break;
                        case 1: color = Color(0x0, 0x2, 0x2); break;
                        case 2: color = Color(0x0, 0x2, 0x3); break;
                        case 3: color = Color(0x0, 0x3, 0x4); break;
                        case 4: color = Color(0x0, 0x2, 0x4); break;
                        case 5: color = Color(0x0, 0x3, 0x3); break;
                        case 6: color = Color(0x0, 0x2, 0x2); break;
                        case 7: color = Color(0x0, 0x2, 0x1); break;
                    }
                    break;
                }
                    
                case Lava:
                {
                    const uint8_t phase = ((tx*5)+ty+(7-((frame&0x38)>>3))) & 0x07;
                    switch (phase) {
                        case 0: color = Color(0xd, 0x3, 0x0); break;
                        case 1: color = Color(0xe, 0x2, 0x0); break;
                        case 2: color = Color(0xf, 0x1, 0x0); break;
                        case 3: color = Color(0xc, 0x1, 0x0); break;
                        case 4: color = Color(0xd, 0x2, 0x0); break;
                        case 5: color = Color(0xc, 0x1, 0x0); break;
                        case 6: color = Color(0xa, 0x1, 0x0); break;
                        case 7: color = Color(0xc, 0x1, 0x0); break;
                    }
                    break;
                }
                    
                case Coin:
                {
                    const uint8_t phase = (((frame & 0x3e)>>1) + tx - ty + _x) & 0x1f;
                    switch (phase) {
                        case 0:
                            color = Color(0xf, 0x6, 0x0);
                            break;
                        case 1:
                            color = Color(0xe, 0x5, 0x0);
                            break;
                        case 2:
                            color = Color(0xe, 0x4, 0x0);
                            break;
                        default:
                            color = Color(0xd, 0x3, 0x0);
                            break;
                    }
                    break;
                }
                    
                case Canon:
                    color = Color(0xc, 0x1, 0x2);
                    break;
                    
                default:
                    break;
            }
            meg.setPixel(xd, 7-yd, color);
        }
    }
}
    
    
void GameCanvas::fillRect(const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height, const Block block)
{
    for (uint8_t xd = 0; xd < width; ++xd) {
        for (uint8_t yd = 0; yd < height; ++yd) {
            const uint8_t tx = x + xd;
            const uint8_t ty = y + yd;
            setNewBlockAt(tx, ty, block);
        }
    }
}
    
    
void GameCanvas::fillRectWithSpacing(const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height, const Block block, uint8_t spacing)
{
    if (spacing == 0) {
        fillRect(x, y, width, height, block);
    } else {
        uint8_t stepSize = spacing + 1;
        for (uint8_t xd = 0; xd < width; xd += stepSize) {
            for (uint8_t yd = 0; yd < height; yd += stepSize) {
                const uint8_t tx = x + xd;
                const uint8_t ty = y + yd;
                setNewBlockAt(tx, ty, block);
            }
        }
    }
}
    
    
}




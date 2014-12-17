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

#include "ClearGameDataScene.h"


#include "GameState.h"

#include <LRMeggyJr.h>


namespace lr {
    
    
namespace {
    const uint8_t symbol[] PROGMEM = {
        B11111111,
        B10011001,
        B11000011,
        B11100111,
        B11100111,
        B11000011,
        B10011001,
        B11111111,
    };
}

    
ClearGameDataScene::ClearGameDataScene(GameState *gameState)
    : Scene(gameState), _counter(0)
{
}

    
ClearGameDataScene::~ClearGameDataScene()
{
}
 
    
void ClearGameDataScene::running(const uint32_t &frame)
{
    // draw the symbol
    uint8_t redIndex = 10-(_counter / 0x20);
    Color color = Color::green();
    for (uint8_t y = 0; y < 8; ++y) {
        if (y == redIndex) {
            color = Color::red();
        }
        uint8_t byte = pgm_read_byte(&symbol[y]);
        for (uint8_t x = 0; x < 8; ++x) {
            if ((byte & (1<<x)) != 0) {
                meg.setPixel(x, y, color);
            } else {
                meg.setPixel(x, y, Color::black());
            }
        }
    }
    // Increase the counter
    if ((meg.getCurrentButtonState() & 0x3f) == 0x3f) {
        ++_counter;
    } else {
        _counter = 0;
    }
    if (_counter > 10*0x20) {
        getGameState()->clearStorage();
        setNextScene(Scene::IntroId);
        setState(SceneSwitch);
    }
}

    
}




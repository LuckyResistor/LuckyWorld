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

#include "FinalScene.h"


#include <LRMeggyJr.h>


namespace lr {
    
    
namespace {
    const uint8_t scrollText[] PROGMEM = "congratulations! you fighted throught all levels of this game. i hope "
        "you enjoyed playing. go back and check if you found all hidden levels. thank you for playing!   ";
}
    
    
FinalScene::FinalScene(GameState *gameState)
    : Scene(gameState), _scrollText(scrollText)
{
    _scrollText.setTextColor(Color::yellow());
}

    
FinalScene::~FinalScene()
{
}
 
    
void FinalScene::running(const uint32_t &frame)
{
    meg.clear();
    
    _scrollText.render();
    
    if ((frame%6)==0) {
        _scrollText.move();
    }
    
    if (meg.isAButtonPressed()) {
        setNextScene(IntroId);
        setState(SceneSwitch);
    }
}

    
}



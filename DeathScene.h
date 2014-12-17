#pragma once
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

#include <Arduino.h>

#include "Scene.h"


namespace lr {

    
/// The death scene with the lives count
///
class DeathScene : public Scene
{
public:
    /// Create the death scene
    ///
    DeathScene(GameState *gameState);
   
    /// dtor
    ///
    virtual ~DeathScene();
    
public: // implement Scene
    //virtual void starting(const uint32_t &frame);
    virtual void running(const uint32_t &frame);
    //virtual void ending(const uint32_t &frame);
    
private:
    uint32_t _frame;
    const uint8_t *_oldLives;
    const uint8_t *_newLives;
};
    

}



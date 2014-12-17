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

#include "LevelManager.h"


#include "Level1_1.h"
#include "Level1_2.h"
#include "Level1_3.h"
#include "Level1_4.h"
#include "Level1_5.h"
#include "Level1_6.h"
#include "Level1_7.h"
#include "Level2_1.h"
#include "Level2_2.h"
#include "Level2_3.h"
#include "Level2_4.h"
#include "Level2_5.h"
#include "Level3_1.h"
#include "Level3_2.h"
#include "Level3_3.h"
#include "Level3_4.h"
#include "Level3_5.h"
#include "Level4_1.h"
#include "Level4_2.h"
#include "Level4_3.h"
#include "Level4_4.h"


namespace lr {
    
    
namespace {
    // The level Map
    //
    // [1-1]-[1-2]-[---]-[1-3]-[1-4]
    //   |                 |     |
    // [1-5]-[1-6] [_-_] [1-7] [|||]
    //                     |     |
    // [2-4]-[---]-[2-3]-[2-2]-[2-1]
    //   |                 |
    // [|||] [_-_] [_-_] [2-5] [_-_]
    //   |                 |
    // [3-1]-[3-2]-[---]-[3-3]-[3-4]
    //         |                 |
    // [_-_] [3-5] [_-_] [_-_] [|||]
    //         |                 |
    // [4-4]-[4-3]-[4-2]-[---]-[4-1]
    //
    const uint8_t levelMap[] PROGMEM = {
        Level1_1|LevelConnectS|LevelConnectE,
        Level1_2|LevelConnectE,
        LevelHorizontalLine|LevelConnectE,
        Level1_3|LevelConnectS|LevelConnectE,
        Level1_4|LevelConnectS|LevelConnectE,
        
        LevelHidden1_5|LevelConnectE,
        LevelHidden1_6,
        NoLevel,
        LevelHidden1_7|LevelConnectS,
        LevelVerticalLine|LevelConnectS,
        
        Level2_4|LevelConnectE|LevelConnectS,
        LevelHorizontalLine|LevelConnectE,
        Level2_3|LevelConnectE,
        Level2_2|LevelConnectS|LevelConnectE,
        Level2_1,
        
        LevelVerticalLine|LevelConnectS,
        NoLevel,
        NoLevel,
        LevelHidden2_5|LevelConnectS,
        NoLevel,
        
        Level3_1|LevelConnectE,
        Level3_2|LevelConnectS|LevelConnectE,
        LevelHorizontalLine|LevelConnectE,
        Level3_3|LevelConnectE,
        Level3_4|LevelConnectS,
        
        NoLevel,
        LevelHidden3_5|LevelConnectS,
        NoLevel,
        NoLevel,
        LevelVerticalLine|LevelConnectS,
        
        Level4_4|LevelConnectE,
        Level4_3|LevelConnectE,
        Level4_2|LevelConnectE,
        LevelHorizontalLine|LevelConnectE,
        Level4_1,
    };
}
    
    
LevelManager::LevelManager()
{
}

    
LevelManager::~LevelManager()
{
}
    

LevelIdentifier LevelManager::getLevelAt(const int8_t x, const int8_t y)
{
    const uint8_t value = getLevelValue(x, y);
    return static_cast<LevelIdentifier>(value & LevelMask);
}

    
bool LevelManager::isLevelConnectedEast(const int8_t x, const int8_t y)
{
    const uint8_t value = getLevelValue(x, y);
    return (value & LevelConnectE) != 0;
}

    
bool LevelManager::isLevelConnectedSouth(const int8_t x, const int8_t y)
{
    const uint8_t value = getLevelValue(x, y);
    return (value & LevelConnectS) != 0;
}

    
const uint8_t LevelManager::getLevelValue(const int8_t x, const int8_t y) const
{
    if (x < 0 || y < 0 || x >= getLevelMapWidth() || y >= getLevelMapHeight()) {
        return 0;
    }
    return pgm_read_byte(&levelMap[x+(getLevelMapWidth()*y)]);
}

    
uint8_t LevelManager::getIndexForLevel(LevelIdentifier identifier) const
{
    return identifier-LevelBase;
}

    
LevelIdentifier LevelManager::getLevelForIndex(uint8_t index) const
{
    return static_cast<LevelIdentifier>(index + LevelBase);
}

    
const uint8_t* LevelManager::getLevel(LevelIdentifier identifier) const
{
    switch (identifier) {
        case Level1_1: return lvLevel1_1;
        case Level1_2: return lvLevel1_2;
        case Level1_3: return lvLevel1_3;
        case Level1_4: return lvLevel1_4;
        case Level2_1: return lvLevel2_1;
        case Level2_2: return lvLevel2_2;
        case Level2_3: return lvLevel2_3;
        case Level2_4: return lvLevel2_4;
        case Level3_1: return lvLevel3_1;
        case Level3_2: return lvLevel3_2;
        case Level3_3: return lvLevel3_3;
        case Level3_4: return lvLevel3_4;
        case Level4_1: return lvLevel4_1;
        case Level4_2: return lvLevel4_2;
        case Level4_3: return lvLevel4_3;
        case Level4_4: return lvLevel4_4;
        case LevelHidden1_5: return lvLevel1_5;
        case LevelHidden1_6: return lvLevel1_6;
        case LevelHidden1_7: return lvLevel1_7;
        case LevelHidden2_5: return lvLevel2_5;
        case LevelHidden3_5: return lvLevel3_5;
        default: return 0;
    }
    return 0;
}
    
    
}



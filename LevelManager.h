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


namespace lr {

    
/// The level of the game.
enum LevelIdentifier : uint8_t {
    
    /// Represents no level (empty space)
    NoLevel     = 0x00,
    
    /// Represents a horizontal line in the map.
    LevelHorizontalLine = 0x01,
    
    /// Represents a vertical line in the map.
    LevelVerticalLine = 0x02,
    
    /// The base value for all level identifiers.
    LevelBase   = 0x10,
    
    Level1_1    = LevelBase+0x00,
    Level1_2    = LevelBase+0x01,
    Level1_3    = LevelBase+0x02,
    Level1_4    = LevelBase+0x03,
    Level2_1    = LevelBase+0x04,
    Level2_2    = LevelBase+0x05,
    Level2_3    = LevelBase+0x06,
    Level2_4    = LevelBase+0x07,
    Level3_1    = LevelBase+0x08,
    Level3_2    = LevelBase+0x09,
    Level3_3    = LevelBase+0x0a,
    Level3_4    = LevelBase+0x0b,
    Level4_1    = LevelBase+0x0c,
    Level4_2    = LevelBase+0x0d,
    Level4_3    = LevelBase+0x0e,
    Level4_4    = LevelBase+0x0f,
    
    LevelHidden1_5 = LevelBase+0x10,
    LevelHidden1_6 = LevelBase+0x11,
    LevelHidden1_7 = LevelBase+0x12,
    LevelHidden1_8 = LevelBase+0x13,
    LevelHidden2_5 = LevelBase+0x14,
    LevelHidden2_6 = LevelBase+0x15,
    LevelHidden2_7 = LevelBase+0x16,
    LevelHidden2_8 = LevelBase+0x17,
    LevelHidden3_5 = LevelBase+0x18,
    LevelHidden3_6 = LevelBase+0x19,
    LevelHidden3_7 = LevelBase+0x1a,
    LevelHidden3_8 = LevelBase+0x1b,
    LevelHidden4_5 = LevelBase+0x1c,
    LevelHidden4_6 = LevelBase+0x1d,
    LevelHidden4_7 = LevelBase+0x1e,
    LevelHidden4_8 = LevelBase+0x1f,
    
    /// The start level.
    LevelStart  = Level1_1,
    
    /// A mask to separate the level information from the connect information in the map.
    LevelMask     = 0x3f,
    
    /// Flag if the level connects to the east.
    LevelConnectE = 0x40,
    
    /// Flag if the level connects to the south.
    LevelConnectS = 0x80,
};

    
/// Manages all the built-in levels.
///
class LevelManager
{
public:
    /// ctor
    ///
    LevelManager();
   
    /// dtor
    ///
    ~LevelManager();
    
public:
    /// Get the width of the level map
    ///
    inline uint8_t getLevelMapWidth() const { return 5; }
    
    /// Get the height of the level map
    ///
    inline uint8_t getLevelMapHeight() const { return 7; }
    
    /// Get the identifier for a given level map position.
    ///
    LevelIdentifier getLevelAt(const int8_t x, const int8_t y);
    
    /// Check if a field is connected to east
    ///
    bool isLevelConnectedEast(const int8_t x, const int8_t y);

    /// Check if a field is connected to south
    ///
    bool isLevelConnectedSouth(const int8_t x, const int8_t y);
    
    /// Get the level index for the given level.
    ///
    uint8_t getIndexForLevel(LevelIdentifier identifier) const;
    
    /// Get a level for the given index
    ///
    LevelIdentifier getLevelForIndex(uint8_t index) const;
    
    /// Get the maximum number of levels
    /// Returns the maximum number of levels (16 regular + 16 hidden).
    ///
    inline uint8_t getLevelCount() const { return 0x20; }

    /// Get the number of regular levels.
    ///
    inline uint8_t getRegularLevelCount() const { return 0x10; }
    
    /// Get the level for the given identifier.
    ///
    const uint8_t* getLevel(LevelIdentifier identifier) const;
        
private:
    /// Get the internal level value for the given level.
    ///
    const uint8_t getLevelValue(const int8_t x, const int8_t y) const;
};
    

}



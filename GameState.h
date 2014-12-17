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


#include "LevelManager.h"


namespace lr {

    
/// The current state of the whole game.
///
class GameState
{
public:
    /// Create a fresh game start after the game resets.
    ///
    GameState(LevelManager *levelManager);
   
    /// dtor
    ///
    ~GameState();
    
public:
    /// Get the current level of the game.
    ///
    inline LevelIdentifier getLevel() const { return _level; }
    
    /// Set the current level.
    ///
    inline void setLevel(const LevelIdentifier level) { _level = level; }
    
    /// Get the last uncovered level.
    ///
    inline LevelIdentifier getLastUncoveredLevel() const { return _lastUncoveredLevel; }
    
    /// Set the last uncovered level.
    ///
    inline void setLastUncoveredLevel(const LevelIdentifier level) { _lastUncoveredLevel = level; }
    
    /// Get the number of initial lives
    ///
    inline uint8_t getInitialLives() const { return 5; }
    
    /// Get the current lives.
    ///
    inline uint8_t getLives() const { return _lives; }
    
    /// Set the current lives.
    ///
    inline void setLives(const uint8_t lives) { _lives = lives; }
    
    /// Get the number of coins
    ///
    inline uint8_t getCoins() const { return _coins; }
    
    /// Set the number of coins
    ///
    inline void setCoins(const uint8_t coins) { _coins = coins; }
    
    /// Access the level manager
    ///
    inline LevelManager* getLevelManager() const { return _levelManager; }

    /// Set a level visible
    ///
    void setLevelVisible(LevelIdentifier levelIdentifier, bool visible = true);
    
    /// Check if a level is visible.
    ///
    bool isLevelVisible(LevelIdentifier levelIdentifier) const;
    
    /// Set a level finished
    ///
    void setLevelFinished(LevelIdentifier levelIdentifier, bool finished = true);
    
    /// Check if a level is finished
    ///
    bool isLevelFinished(LevelIdentifier levelIdentifier) const;
    
    /// Set to initial state
    ///
    void clear();
    
    /// Try to load the state from the EEPROM
    /// If no state is found, an empty state is created.
    ///
    void loadFromStorage();
    
    /// Save the current state to the EEPROM
    /// This can take a while.
    ///
    void saveToStorage();
    
    /// Clear storage
    /// To reset everything.
    ///
    void clearStorage();
    
private:
    /// Clear a slot
    ///
    void clearSlot(uint8_t slotIndex);
    
    /// Try to read data from the given slot.
    ///
    /// @return true on success, false if the CRC does not match.
    ///
    bool readFromSlot(uint8_t slotIndex, uint8_t *data);
    
    /// Write the data to the given slot
    ///
    void writeToSlot(uint8_t slotIndex, uint8_t *data);
    
private:
    LevelIdentifier _level; // The current (selected) level.
    LevelIdentifier _lastUncoveredLevel; // The last level which got visible.
    uint8_t _lives; // The number of lives.
    uint8_t _coins; // The number of collected coins
    uint8_t _levelFlagByteCount; // The number of bytes for the level bit matrices.
    uint8_t *_visibleLevels; // A matrix of bits of visible levels.
    uint8_t *_finishedLevels; // A matrix of bits of the finished levels.
    uint8_t _storeSlotSize; // The size in bytes of one slot including the CRC
    uint8_t _loadSlotIndex; // The index of the slot from which the state was loaded.
    LevelManager *_levelManager; // Access the level manager.
};
    

}



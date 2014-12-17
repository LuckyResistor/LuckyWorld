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

#include "GameState.h"

#include <avr/eeprom.h>
#include <util/crc16.h>


namespace lr {
    
    
GameState::GameState(LevelManager *levelManager)
:
    _level(LevelStart),
    _lastUncoveredLevel(NoLevel),
    _lives(getInitialLives()),
    _coins(0),
    _loadSlotIndex(0xff),
    _levelManager(levelManager)
{
    // Create the bitmasks for visible and finished levels.
    _levelFlagByteCount = (_levelManager->getLevelCount()-1)/8+1;
    _visibleLevels = new uint8_t[_levelFlagByteCount];
    _finishedLevels = new uint8_t[_levelFlagByteCount];
    memset(_visibleLevels, 0, _levelFlagByteCount);
    memset(_finishedLevels, 0, _levelFlagByteCount);
    // calculate the size of one storage slot.
    _storeSlotSize = 2/*crc*/+1/*selected level*/+1/*lives*/+1/*coins*/+(_levelFlagByteCount*2);
}

    
GameState::~GameState()
{
}
    
 
void GameState::clear()
{
    _level = LevelStart;
    _lives = getInitialLives();
    _coins = 0;
    memset(_visibleLevels, 0, _levelFlagByteCount);
    memset(_finishedLevels, 0, _levelFlagByteCount);
    setLevelVisible(LevelStart);
}

    
void GameState::setLevelVisible(LevelIdentifier levelIdentifier, bool visible)
{
    const uint8_t levelIndex = _levelManager->getIndexForLevel(levelIdentifier);
    const uint8_t byte = levelIndex/8;
    const uint8_t bit = levelIndex%8;
    if (byte < _levelFlagByteCount) {
        if (visible) {
            _visibleLevels[byte] |= (1<<bit);
        } else {
            _visibleLevels[byte] &= ~(1<<bit);
        }
    }
}

    
bool GameState::isLevelVisible(LevelIdentifier levelIdentifier) const
{
    const uint8_t levelIndex = _levelManager->getIndexForLevel(levelIdentifier);
    const uint8_t byte = levelIndex/8;
    const uint8_t bit = levelIndex%8;
    if (byte < _levelFlagByteCount) {
        return (_visibleLevels[byte] & (1<<bit)) != 0;
    } else {
        return false;
    }
}


void GameState::setLevelFinished(LevelIdentifier levelIdentifier, bool finished)
{
    const uint8_t levelIndex = _levelManager->getIndexForLevel(levelIdentifier);
    const uint8_t byte = levelIndex/8;
    const uint8_t bit = levelIndex%8;
    if (byte < _levelFlagByteCount) {
        if (finished) {
            _finishedLevels[byte] |= (1<<bit);
        } else {
            _finishedLevels[byte] &= ~(1<<bit);
        }
    }
}

    
bool GameState::isLevelFinished(LevelIdentifier levelIdentifier) const
{
    const uint8_t levelIndex = _levelManager->getIndexForLevel(levelIdentifier);
    const uint8_t byte = levelIndex/8;
    const uint8_t bit = levelIndex%8;
    if (byte < _levelFlagByteCount) {
        return (_finishedLevels[byte] & (1<<bit)) != 0;
    } else {
        return false;
    }
}


void GameState::loadFromStorage()
{
    // Prepare the data block
    uint8_t *data = new uint8_t[_storeSlotSize];
    // Read up to two slots to search for valid data.
    uint8_t readIndex = 0xff;
    for (uint16_t i = 0; i < 2; ++i) {
        if (readFromSlot(i, data)) {
            readIndex = i;
            break;
        }
    }
    // Could be read the data?
    if (readIndex == 0xff) { // no
        clear();
        _loadSlotIndex = 0xff; // no slot read.
    } else {
        _loadSlotIndex = readIndex; // remember the slot we used to read.
        // Parse the data
        _level = static_cast<LevelIdentifier>(data[2]);
        _lives = data[3];
        _coins = data[4];
        for (uint8_t i = 0; i < _levelFlagByteCount; ++i) {
            _visibleLevels[i] = data[5+i];
            _finishedLevels[i] = data[5+_levelFlagByteCount+i];
        }
    }
    // free the data block.
    delete data;
}
    
    
void GameState::saveToStorage()
{
    // Prepare the data block
    uint8_t *data = new uint8_t[_storeSlotSize];
    data[2] = _level;
    data[3] = _lives;
    data[4] = _coins;
    for (uint8_t i = 0; i < _levelFlagByteCount; ++i) {
        data[5+i] = _visibleLevels[i];
        data[5+_levelFlagByteCount+i] = _finishedLevels[i];
    }
    // calculate the crc
    uint16_t crc = 0xaaaa; // start with magic value.
    for (uint8_t i = 2; i < _storeSlotSize; ++i) {
        crc = _crc16_update(crc, data[i]);
    }
    ((uint16_t*)data)[0] = crc;
    // Write to the opposite slot
    if (_loadSlotIndex == 0) {
        writeToSlot(1, data);
        clearSlot(0);
    } else {
        writeToSlot(0, data);
        clearSlot(1);
    }
    // free the data block.
    delete data;
}

    
void GameState::clearStorage()
{
    // Overwrite the storage
    for (uint8_t i = 0; i < 2; ++i) {
        clearSlot(i);
    }
    clear();
}

    
void GameState::clearSlot(uint8_t slotIndex)
{
    // convert the index to a byte pointer.
    uint8_t *startByte = (uint8_t*)(slotIndex*_storeSlotSize);
    for (uint8_t i = 0; i < _storeSlotSize; ++i) {
        eeprom_write_byte(startByte+i, 0);
    }
}

   
bool GameState::readFromSlot(uint8_t slotIndex, uint8_t *data)
{
    // convert the index to a byte pointer.
    uint8_t *startByte = (uint8_t*)(slotIndex*_storeSlotSize);
    eeprom_read_block(data, startByte, _storeSlotSize);
    // calculate the crc
    uint16_t crc = 0xaaaa; // start with magic value
    for (uint8_t i = 2; i < _storeSlotSize; ++i) {
        crc = _crc16_update(crc, data[i]);
    }
    // Verify the CRC
    const uint16_t dataCRC = ((uint16_t*)data)[0];
    return crc == dataCRC;
}
    
    
void GameState::writeToSlot(uint8_t slotIndex, uint8_t *data)
{
    // convert the index to a byte pointer.
    uint8_t *startByte = (uint8_t*)(slotIndex*_storeSlotSize);
    eeprom_write_block(data, startByte, _storeSlotSize);
}

    
}



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
#include "LevelManager.h"


namespace lr {

    
/// The scene to select a level
///
class SelectLevelScene : public Scene
{
private:
    /// The state of the scene
    ///
    enum State {
        WaitForUserInput,
        ScrollingN,
        ScrollingE,
        ScrollingS,
        ScrollingW,
    };
    
    /// Flags for the level
    ///
    enum Flag {
        /// There is a connection to the east
        ConnectionE = (1<<0),
        /// There is a connection to the south
        ConnectionS = (1<<1),
        /// There is a connection to the west
        ConnectionW = (1<<2),
        /// There is a connection to the north
        ConnectionN = (1<<3),
        /// The level was finished
        LevelFinished = (1<<4),
        /// The level is visible.
        LevelVisible = (1<<5),
        /// There is a visible connection to the east
        VisibleBridgeE = (1<<6),
        /// There is a visible connection to the south.
        VisibleBridgeS = (1<<7)
    };
    
    /// Represents a level entry in the map.
    ///
    class Level {
    public:
        /// ctor
        ///
        Level();
        
    public:
        /// Get the x position of this level.
        ///
        inline uint8_t getX() const { return (_position & 0x0f); }
        
        /// Get the y position of this level.
        ///
        inline uint8_t getY() const { return (_position >> 4); }
        
        /// Set the position of this level.
        ///
        inline void setPosition(const uint8_t x, const uint8_t y) { _position = x | (y << 4); }
        
        /// Get the level identifier.
        ///
        inline LevelIdentifier getLevelIdentifier() const { return _levelIdentifier; }
        
        /// Set the level identifier.
        ///
        inline void setLevelIdentifier(const LevelIdentifier levelIdentifier) { _levelIdentifier = levelIdentifier; }
        
        /// Enable a flag for this level.
        ///
        inline void enableFlag(const Flag flag) { _flags |= flag; }
        
        /// If this level has a connection to the east.
        ///
        inline bool hasConnectionE() const { return (_flags & ConnectionE) != 0; }
        
        /// If this level has a connection to the south.
        ///
        inline bool hasConnectionS() const { return (_flags & ConnectionS) != 0; }
        
        /// If this level has a connection to the west.
        ///
        inline bool hasConnectionW() const { return (_flags & ConnectionW) != 0; }
        
        /// If this level has a connection to the north.
        ///
        inline bool hasConnectionN() const { return (_flags & ConnectionN) != 0; }
        
        /// If this level was finished before.
        ///
        inline bool isLevelFinished() const { return (_flags & LevelFinished) != 0; }
        
        /// If this level is visible to the player.
        ///
        inline bool isLevelVisible() const { return (_flags & LevelVisible) != 0; }
        
        /// If there is a visible bridge to the east.
        ///
        inline bool hasVisibleBridgeE() const { return (_flags & VisibleBridgeE) != 0; }
        
        /// If there is a visible brigde to the south.
        ///
        inline bool hasVisibleBridgeS() const { return (_flags & VisibleBridgeS) != 0; }
        
        /// If this is real level.
        ///
        inline bool isLevel() const { return _levelIdentifier >= LevelBase; }
        
        /// Set the color for this level.
        ///
        inline void setLevelColor(const uint8_t color) { _color = ((_color & 0xf0) | (color & 0x0f)); }
        
        /// Get the color for this level.
        ///
        inline uint8_t getLevelColor() const { return (_color & 0x0f); }
        
        /// Set if this level should be highlighted.
        ///
        inline void setHighlight() { _color |= B00010000; }
        
        /// Check if this level is highlighted.
        ///
        inline bool isHighlighted() const { return (_color & B00010000) != 0; }
        
        /// Check if this level is a horizontal line.
        ///
        inline bool isHorizontalLine() const { return _levelIdentifier == LevelHorizontalLine; }
        
        /// Check if this level is a vertical line.
        ///
        inline bool isVerticalLine() const { return _levelIdentifier == LevelVerticalLine; }
        
    private:
        LevelIdentifier _levelIdentifier; // The represented level
        uint8_t _flags; // Various flags
        uint8_t _position; // yyyyxxxx
        uint8_t _color; // The color of the level. And the highlight flag.
    };
    
public:
    /// Create the scene.
    ///
    SelectLevelScene(GameState *gameState);
   
    /// dtor
    ///
    virtual ~SelectLevelScene();
    
public: // implement Scene
    virtual void starting(const uint32_t &frame);
    virtual void running(const uint32_t &frame);
    virtual void ending(const uint32_t &frame);
    
private:
    /// Access the level at the given position.
    ///
    Level* getLevel(const int8_t x, const int8_t y);
    
    /// Get the x/y position of a level
    ///
    bool getLevelXY(const LevelIdentifier levelIdentifier, int8_t *x, int8_t *y) const;
    
    /// Draw the levels.
    ///
    void drawLevels(const uint32_t &frame);
    
private:
    State _state; // The current state
    int8_t _x; // The x position of the player in the map
    int8_t _y; // The y position of the player in the map
    int8_t _dx; // Delta x in pixels
    int8_t _dy; // Delta y in pixels
    LevelIdentifier _selectedLevel; // The current selected level.
    uint8_t _levelMapCount; // The number of level elements.
    uint8_t _levelMapWidth; // The width of the level map.
    uint8_t _levelMapHeight; // The height of the level map.
    Level *_levelMap; // The level map.
    LevelManager *_levelManager; // The level manager.
    uint32_t _lastFrame; // The last frame for timing.
};
    

}



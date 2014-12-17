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

    
class GameObjectArray;
    
    
/// This represents the game canvas.
///
/// A small area to buffer the immediate surroundings of the player for
/// repetive and quick rendering. The canvas just contains all static objects,
/// because they can cover a large area. Dynamic objects are rendered on top
/// of the canvas, because they usually just need a few pixels for display.
///
/// The x coordinate moves along and is the lowest possible location which
/// can be displayed for the game. The player can not move backwards over this
/// magical barrier, and dynamic objects will vanish if they leave the canvas.
///
class GameCanvas
{
public:
    /// The type of a block
    enum Block {
        Empty    = 0, // No block- background
        Ground   = 1, // Solid ground
        Platform = 2, // A moving platform.
        Water    = 3, // Water (deadly)
        Lava     = 4, // Laval (deadly)
        Coin     = 5, // Coins
        Canon    = 6, // A canon
    };
    
public:
    /// Create an empty canvas with the given size.
    ///
    GameCanvas(uint8_t width, uint8_t height);
    
    /// dtor
    ///
    ~GameCanvas();
    
public:
    /// Get the width of the canvas.
    ///
    inline uint8_t getWidth() const { return _width; }
    
    /// Get the height of the canvas.
    ///
    inline uint8_t getHeight() const { return _height; }
    
    /// Get the x coordinate of the canvas.
    ///
    inline uint8_t getLeftX() const { return _x; }
    
    /// Set the x coordinate of the canvas.
    ///
    inline void setLeftX(uint8_t x) { _x = x; }
    
    /// Set the last possible X position
    ///
    inline void setLastX(const uint8_t lastX) { _lastX = lastX; }
    
    /// Get the last possible X position
    ///
    inline uint8_t getLastX() const { return _lastX; }
    
    /// Get the last object position.
    ///
    inline uint8_t getLastObjectPositionX() const { return _x+_width-2; }
    
    /// Get the x position of the right side of the canvas.
    ///
    inline uint8_t getRightX() const { return _x+_width-1; }

    /// Set the level theme
    ///
    inline void setTheme(const uint8_t theme) { _theme = theme; }
    
    /// Get the level theme
    ///
    inline uint8_t getTheme() const { return _theme; }
    
    /// Check if there is solid ground at the given position
    ///
    bool isSolidGround(uint8_t x, uint8_t y) const;
    
    /// Check if there is some deadly obstacle at the given position.
    /// This is true for lava or water.
    ///
    bool isDeadly(uint8_t x, uint8_t y) const;
    
    /// Scroll the canvas by two x positions to left
    ///
    /// At the right side, two empty columns are added to the canvas.
    ///
    void scroll2Left();
    
    /// Draw the initial static game objects to the canvas
    ///
    /// This will use the whole canvas to draw the game object and
    /// it clears the canvas before the actual drawing is started.
    ///
    /// @param gameObjectArray The game object arary to render.
    ///
    void drawInitialStaticObjects(const GameObjectArray &gameObjectArray);

    /// Draw the current static game objects to the canvas.
    ///
    /// This will only draw in the last two columns of the canvas to
    /// add new game objects to the canvas.
    ///
    /// @param gameObjectArray The game object arary to render.
    ///
    void drawStaticObjects(const GameObjectArray &gameObjectArray);

    /// Render a part of the canvas
    ///
    /// @param x The left x position where the render shall start.
    /// @param y The bottom y position where the render shall start.
    /// @param frame The frame for the animatons.
    ///
    void renderCanvas(const uint8_t x, const uint8_t y, const uint32_t &frame);

    /// Set the block at the given level position
    ///
    /// @param x The x position of the block, level coordinates.
    /// @param y The y position of the block, level coordinates.
    /// @param block The block to draw.
    ///
    inline void setBlockAt(const uint8_t x, const uint8_t y, Block block) {
        const uint8_t tx = x-_x;
        if (tx >= _width) {
            return;
        }
        if (y >= _height) {
            return;
        }
        uint8_t *byte = &_canvas[(tx>>1) + (y*_rowSize)];
        if ((x&1)==0) {
            *byte = (((*byte) & 0xf0) | block);
        } else {
            *byte = (((*byte) & 0x0f) | (block << 4));
        }
    }
    
    /// Get the block at the given level position
    ///
    /// If the block is outside of the canvas, the behaviour is undefined.
    ///
    /// @param x The x position of the block, level coordinates.
    /// @param y The y position of the block, level coordinates.
    /// @return The block at the given position.
    ///
    inline Block getBlockAt(const uint8_t x, const uint8_t y) const {
        const uint8_t tx = x-_x;
        uint8_t byte = _canvas[(tx>>1) + (y*_rowSize)];
        if ((x&1)==0) {
            return (Block)(byte & 0xf);
        } else {
            return (Block)(byte >> 4);
        }
    }

private:
    /// Clear the canvas.
    ///
    void clear();
    
    /// Fill a rect with the given block
    ///
    void fillRect(const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height, const Block block);

    /// Fill a rect with the given block and use a given spacing between the pixels.
    ///
    void fillRectWithSpacing(const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height, const Block block, uint8_t spacing);

    
    /// Set the block at the given level position but mask using _drawMinX.
    ///
    /// @param x The x position of the block.
    /// @param y The y position of the block.
    /// @param block The block to draw.
    ///
    inline void setNewBlockAt(const uint8_t x, const uint8_t y, Block block) {
        const uint8_t tx = x-_x;
        if (tx >= _width || tx < _drawMinX) {
            return;
        }
        if (y >= _height) {
            return;
        }
        uint8_t *byte = &_canvas[(tx>>1) + (y*_rowSize)];
        if ((x&1)==0) {
            *byte = (((*byte) & 0xf0) | block);
        } else {
            *byte = (((*byte) & 0x0f) | (block << 4));
        }
    }
   
private:
    const uint8_t _width; // The width of the canvas.
    const uint8_t _height; // The height of the canvas.
    const uint8_t _rowSize; // The size of a row in bytes.
    const uint16_t _canvasSize; // The size of the canvas in bytes.
    uint8_t *_canvas; // The canvas data.
    uint8_t _x; // The x position of the left edge of the canvas.
    uint8_t _drawMinX; // The minimal x position to draw a block.
    uint8_t _lastX; // The last X position.
    uint8_t _theme; // The current level theme.
};


}
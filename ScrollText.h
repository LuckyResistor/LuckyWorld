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

#include "LRColor.h"


namespace lr {

    
/// A scroll text generator
///
class ScrollText
{
    enum ScrollTextState : uint8_t {
        STSSpace,
        STSCharacter,
    };
    
public:
    /// Create a scroll text with the given text.
    ///
    ScrollText(const uint8_t *text);
   
    /// dtor
    ///
    ~ScrollText();
    
public:
    /// Set the text color
    ///
    inline void setTextColor(const Color &color) { _textColor = color; }
    
    /// Set the shadow color
    ///
    inline void setShadowColor(const Color &color) { _shadowColor = color; }
    
    /// Renders the text on the screen
    ///
    void render();
    
    /// Move the scroll text one position
    ///
    void move();
    
private:
    const uint8_t *_text; // The text
    ScrollTextState _textState; // The state of the scroll text.
    uint8_t _textBitBuffer[9]; // The currently visible text
    const uint8_t *_textPtr; // A pointer to the current text position.
    uint8_t _charRowsLeft; // The number of characters rows left in the current character.
    const uint8_t *_charRowPtr; // A pointer to the current row in the character.
    Color _textColor; // The text color
    Color _shadowColor; // The shadow color;
};
    

}



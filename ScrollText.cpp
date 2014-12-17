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

#include "ScrollText.h"


#include <LRMeggyJr.h>


namespace lr {
    

namespace {
const uint8_t fontChars[] PROGMEM = " abcdefghijklmnopqrstuvwxyz!.";
const uint8_t fontCharSize[] PROGMEM = {
    2, 5, 5, 4, 5, 4, 4, 5, 5, 4, 4, 5, 4, 7, 5, 5, 5, 5, 5, 5, 4, 5, 5, 7, 5, 5, 5, 2, 2
};
const uint8_t font[] PROGMEM = {
    B00000000, // 2
    B00000000,
    
    B00111110, // 5
    B00111111,
    B00001001,
    B00111111,
    B00111110,
    
    B00111111, // 5
    B00111111,
    B00100101,
    B00111111,
    B00011010,
    
    B00011110, // 4
    B00111111,
    B00100001,
    B00100001,
    
    B00111111, // 5
    B00111111,
    B00100001,
    B00111111,
    B00011110,
    
    B00111111, // 4
    B00111111,
    B00100101,
    B00100001,
    
    B00111111, // 4
    B00111111,
    B00000101,
    B00000001,
    
    B00011110, // 5
    B00111111,
    B00100001,
    B00111101,
    B00111100,
    
    B00111111, // 5
    B00111111,
    B00000100,
    B00111111,
    B00111111,
    
    B00100001, // 4
    B00111111,
    B00111111,
    B00100001,
    
    B00100000, // 4
    B00100000,
    B00111111,
    B00011111,
    
    B00111111, // 5
    B00111111,
    B00000100,
    B00111011,
    B00110001,
    
    B00111111, // 4
    B00111111,
    B00100000,
    B00100000,
    
    B00111111, // 7
    B00111111,
    B00000001,
    B00111111,
    B00000001,
    B00111111,
    B00111110,
    
    B00111111, // 5
    B00111111,
    B00000001,
    B00111111,
    B00111110,
    
    B00011110, // 5
    B00111111,
    B00100001,
    B00111111,
    B00011110,
    
    B00111111, // 5
    B00111111,
    B00001001,
    B00001111,
    B00000110,
    
    B00011110, // 5
    B00111111,
    B00110001,
    B00101111,
    B01011110,
    
    B00111111, // 5
    B00111111,
    B00001001,
    B00111111,
    B00110110,
    
    B00110010, // 5
    B00110111,
    B00100101,
    B00111101,
    B00011001,
    
    B00000001, // 4
    B00111111,
    B00111111,
    B00000001,
    
    B00011111, // 5
    B00111111,
    B00100000,
    B00111111,
    B00011111,
    
    B00000111, // 5
    B00011111,
    B00100000,
    B00111111,
    B00111111,
    
    B00011111, // 7
    B00111111,
    B00100000,
    B00111111,
    B00100000,
    B00111111,
    B00011111,
    
    B00110001, // 5
    B00111011,
    B00000100,
    B00111011,
    B00110001,
    
    B00100111, // 5
    B00101111,
    B00101000,
    B00111111,
    B00011111,
    
    B00110001, // 5
    B00111001,
    B00101001,
    B00100111,
    B00100011,

    B00101111, // 2
    B00101111,

    B00110000, // 2
    B00110000,
};
}

    
ScrollText::ScrollText(const uint8_t *text)
:
    _text(text),
    _textState(STSSpace),
    _textPtr(text),
    _charRowsLeft(1),
    _charRowPtr(font),
    _textColor(12, 6, 6),
    _shadowColor(0, 0, 0)
{
    // Initialize the text bit buffer empty.
    memset(_textBitBuffer, 0, 9);
}

    
ScrollText::~ScrollText()
{
    
}
    
    
void ScrollText::render()
{
    for (uint8_t x = 0; x < 8; ++x) {
        const uint8_t textBits = _textBitBuffer[x];
        for (uint8_t y = 0; y < 8; ++y) {
            const uint8_t bitMask = (1<<y);
            if ((_textBitBuffer[x+1]&bitMask) != 0) {
                meg.setPixelS(x, y, _shadowColor);
            }
            if ((textBits & bitMask) != 0) {
                meg.setPixelS(x, y, _textColor);
            }
        }
    }
}

    
void ScrollText::move()
{
    // Calculate the scroll text
    memmove(_textBitBuffer, _textBitBuffer+1, 8);
    if (_textState == STSSpace) {
        _textBitBuffer[8] = 0;
        if (--_charRowsLeft == 0) {
            // read the next character from the text.
            uint8_t nextChar = pgm_read_byte(_textPtr++);
            if (nextChar == 0) {
                _textPtr = _text;
                nextChar = pgm_read_byte(_textPtr++);
            }
            // serach the character
            _charRowPtr = font;
            const uint8_t *findChar = fontChars;
            const uint8_t *findCharSize = fontCharSize;
            uint8_t foundChar = pgm_read_byte(findChar++);
            _charRowsLeft = pgm_read_byte(findCharSize++);
            while (foundChar != nextChar) {
                _charRowPtr += _charRowsLeft;
                foundChar = pgm_read_byte(findChar++);
                if (foundChar == 0) {
                    _charRowsLeft = pgm_read_byte(findCharSize);
                    _charRowPtr = fontChars;
                    break;
                }
                _charRowsLeft = pgm_read_byte(findCharSize++);
            }
            _textState = STSCharacter;
        }
    } else {
        _textBitBuffer[8] = pgm_read_byte(_charRowPtr++);
        if (--_charRowsLeft == 0) {
            _textState = STSSpace;
            _charRowsLeft = 1;
        }
    }
}


}



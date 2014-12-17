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

#include "IntroScene.h"


#include "Sounds.h"
#include "GameState.h"

#include <LRMeggyJr.h>

#include <avr/pgmspace.h>


namespace lr {
 
    
namespace {
    
const uint8_t mountain1Count = 24;
const uint8_t mountain1[] PROGMEM = {
    B10000000,
    B11000000,
    B11100000,
    B11100000,
    B11000000,
    B10000000,
    B00000000,
    B10000000,
    
    B11000000,
    B10000000,
    B00000000,
    B10000000,
    B11000000,
    B11100000,
    B11000000,
    B10000000,

    B00000000,
    B10000000,
    B11000000,
    B11100000,
    B11000000,
    B11100000,
    B11000000,
    B10000000,
};

    
const uint8_t mountain2Count = 16;
const uint8_t mountain2[] PROGMEM = {
    B11111100,
    B11111000,
    B11100000,
    B11100000,
    B11110000,
    B11111100,
    B11111100,
    B11111000,

    B11110000,
    B11110000,
    B11111000,
    B11111100,
    B11110000,
    B11100000,
    B11110000,
    B11111000,
};

    
const uint8_t scrollText[] PROGMEM = "   lucky world    press a to start    ";
    
    
const uint16_t mountain1Color[] PROGMEM = {
    LRCOLOR_STATIC(0, 4, 0),
    LRCOLOR_STATIC(0, 4, 0),
    LRCOLOR_STATIC(0, 4, 0),
    LRCOLOR_STATIC(0, 4, 0),
    LRCOLOR_STATIC(1, 4, 0),
    LRCOLOR_STATIC(2, 4, 0),
    LRCOLOR_STATIC(3, 4, 0),
    LRCOLOR_STATIC(4, 4, 0),
};

    
const uint16_t mountain2Color[] PROGMEM = {
    LRCOLOR_STATIC(0, 2, 0),
    LRCOLOR_STATIC(0, 2, 0),
    LRCOLOR_STATIC(0, 2, 0),
    LRCOLOR_STATIC(2, 2, 0),
    LRCOLOR_STATIC(4, 2, 0),
    LRCOLOR_STATIC(6, 2, 0),
    LRCOLOR_STATIC(8, 2, 0),
    LRCOLOR_STATIC(10, 2, 0),
};

    
const uint16_t skyColor[] PROGMEM = {
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),

    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 1),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 1),
    LRCOLOR_STATIC(1, 0, 1),
    LRCOLOR_STATIC(1, 0, 2),
    LRCOLOR_STATIC(2, 1, 1),
    LRCOLOR_STATIC(3, 1, 1),
    
    LRCOLOR_STATIC(4, 1, 1), // normal from here
    LRCOLOR_STATIC(3, 1, 1),
    LRCOLOR_STATIC(2, 1, 1),
    LRCOLOR_STATIC(1, 1, 1),
    LRCOLOR_STATIC(0, 1, 1),
    LRCOLOR_STATIC(0, 1, 1),
    LRCOLOR_STATIC(0, 1, 1),
    LRCOLOR_STATIC(0, 1, 1),
};

    
const uint16_t fadeOutColor[] PROGMEM = {
    LRCOLOR_STATIC(15, 15, 15),
    LRCOLOR_STATIC(15, 12, 15),
    LRCOLOR_STATIC(15, 8, 15),
    LRCOLOR_STATIC(12, 6, 12),
    LRCOLOR_STATIC(8, 4, 8),
    LRCOLOR_STATIC(6, 3, 6),
    LRCOLOR_STATIC(4, 2, 4),
    LRCOLOR_STATIC(2, 1, 2),
    LRCOLOR_STATIC(1, 1, 1),
    LRCOLOR_STATIC(1, 0, 1),
    LRCOLOR_STATIC(0, 0, 1),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
    LRCOLOR_STATIC(0, 0, 0),
};
   

const uint8_t commandLength = 8;
const char commandLives[] PROGMEM = "uurrddll";
const char commandUncover[] PROGMEM = "lrdurlll";
    
    
}
    
    
IntroScene::IntroScene(GameState *gameState)
    : Scene(gameState), _scrollText(scrollText), _startShift(16), _commandStartFrame(0), _commandPosition(0)
{
    memset(_command, 0, 8);
    _scrollText.setShadowColor(Color(0, 0, 1));
}

    
IntroScene::~IntroScene()
{
}

    
void IntroScene::starting(const uint32_t &frame)
{
    meg.clear();
    
    // Show the scene
    running(frame);
    
    if ((frame & 3) == 0) {
        if (--_startShift == 0) {
            setState(SceneRunning);
        }
    }
}
    
    
void IntroScene::running(const uint32_t &frame)
{
    // Calculate the offsets
    uint8_t mountain1Index = (frame / 5) % mountain1Count;
    uint8_t mountain2Index = (frame / 9) % mountain2Count;
    
    // Render the scene with all layers.
    for (uint8_t x = 0; x < 8; ++x) {
        const uint8_t m1Bits = (pgm_read_byte(&mountain1[mountain1Index]) << _startShift);
        const uint8_t m2Bits = (pgm_read_byte(&mountain2[mountain2Index]) << _startShift);
        for (uint8_t y = 0; y < 8; ++y) {
            const uint8_t bitMask = (1<<y);
            if ((m1Bits & bitMask) != 0) {
                meg.setPixelS(x, y, Color::fromProgMem(&mountain1Color[y]));
            } else if ((m2Bits & bitMask) != 0) {
                meg.setPixelS(x, y, Color::fromProgMem(&mountain2Color[y]));
            } else {
                meg.setPixelS(x, y, Color::fromProgMem(&skyColor[y+(16-_startShift)]));
            }
        }
        if (++mountain1Index == mountain1Count) {
            mountain1Index = 0;
        }
        if (++mountain2Index == mountain2Count) {
            mountain2Index = 0;
        }
    }
    
    // Render the scroll text on top
    _scrollText.render();
    
    // This parts are own shown if the scene is running
    if (getState() == SceneRunning) {
        // Move the scroll text.
        if ((frame % 6) == 0) {
            _scrollText.move();
        }
        
        // Check if all buttons are pressed and switch to the clear game data scene.
        if ((meg.getCurrentButtonState() & 0x3f) == 0x3f) {
            setNextScene(Scene::ClearGameDataId);
            setState(SceneSwitch);
            return;
        }

        // Check the A button
        if (meg.isAButtonPressed()) {
            setState(SceneEnding);
            _endFrameOffset = frame;
            meg.playSound(kSoundCoin);
        }
        
        if (_commandPosition > 0 && (frame - _commandStartFrame) > 240) {
            _commandPosition = 0;
        }
        
        char commandChar = 0;
        if (meg.isLeftButtonPressed()) {
            commandChar = 'l';
        } else if (meg.isRightButtonPressed()) {
            commandChar = 'r';
        } else if (meg.isUpButtonPressed()) {
            commandChar = 'u';
        } else if (meg.isDownButtonPressed()) {
            commandChar = 'd';
        }
        if (commandChar != 0) {
            if (_commandPosition == 0) {
                _commandStartFrame = frame;
            }
            _command[_commandPosition++] = commandChar;
            if (_commandPosition == commandLength) {
                if (compareCommand(commandLives)) {
                    getGameState()->setLives(8);
                    meg.playSound(kSoundNewLive);
                } else if (compareCommand(commandUncover)) {
                    const uint8_t levelCount = getGameState()->getLevelManager()->getLevelCount();
                    for (uint8_t i = 0; i < levelCount; ++i) {
                        const LevelIdentifier level = getGameState()->getLevelManager()->getLevelForIndex(i);
                        getGameState()->setLevelFinished(level);
                        getGameState()->setLevelVisible(level);
                    }
                    meg.playSound(kSoundFinishLevel);
                }
                _commandPosition = 0;
            }
        }
    }
}
  
    
bool IntroScene::compareCommand(const char *pmCommand) const
{
    for (uint8_t i = 0; i < commandLength; ++i) {
        const char cmdChar = pgm_read_byte(pmCommand + i);
        if (cmdChar != _command[i]) {
            return false;
        }
    }
    return true;
}

    
void IntroScene::ending(const uint32_t &frame)
{
    // render the previous scene
    running(frame);
    
    // Draw an animation over the scene.
    const int16_t phase = (frame - _endFrameOffset) / 2;
    for (uint8_t x = 0; x < 8; ++x) {
        const int16_t colorIndex = (-9 + phase) + x;
        if (colorIndex >= 0 && colorIndex < 16) {
            for (uint8_t y = 0; y < 8; ++y) {
                meg.setPixelS(x, y, Color::fromProgMem(&fadeOutColor[colorIndex]));
            }
        } else if (colorIndex >= 16) {
            for (uint8_t y = 0; y < 8; ++y) {
                meg.setPixelS(x, y, Color::black());
            }
        }
    }
    
    // Switch the scene after the animation.
    if (phase == 20) {
        setNextScene(Scene::LevelSelectId);
        setState(SceneSwitch);
    }
}
    
    
}



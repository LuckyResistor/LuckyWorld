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

#include "SelectLevelScene.h"


#include "GameState.h"
#include "LevelManager.h"

#include <LRMeggyJr.h>


namespace lr {
   

namespace {
const uint16_t levelColors[] PROGMEM = {
    LRCOLOR_STATIC(0x2, 0x4, 0x0),
    LRCOLOR_STATIC(0xc, 0x2, 0x0),
    LRCOLOR_STATIC(0xf, 0x1, 0x4),
    LRCOLOR_STATIC(0x0, 0x7, 0x2),
    LRCOLOR_STATIC(0xd, 0x4, 0x0),
    LRCOLOR_STATIC(0xd, 0x4, 0x0),
    LRCOLOR_STATIC(0xd, 0x4, 0x0),
    LRCOLOR_STATIC(0xd, 0x4, 0x0),
};
}
    

SelectLevelScene::Level::Level()
:
    _levelIdentifier(NoLevel),
    _flags(0),
    _position(0),
    _color(0)
{
}

    
SelectLevelScene::SelectLevelScene(GameState *gameState)
:
    Scene(gameState),
    _state(WaitForUserInput),
    _x(0),
    _y(0),
    _dx(0),
    _dy(0),
    _selectedLevel(gameState->getLevel()),
    _levelManager(gameState->getLevelManager()),
    _lastFrame(0)
{
    // Create a map with all level elements.
    _levelMapWidth = _levelManager->getLevelMapWidth();
    _levelMapHeight = _levelManager->getLevelMapHeight();
    _levelMapCount = _levelMapWidth*_levelMapHeight;
    _levelMap = new Level[_levelMapCount];
    // Assign all required values and build a visual map of the levels.
    for (uint8_t x = 0; x < _levelMapWidth; ++x) {
        for (uint8_t y = 0; y < _levelMapHeight; ++y) {
            // Extract static attributes from the level manager.
            Level *level = getLevel(x, y);
            if (level == 0) {
                continue;
            }
            level->setPosition(x, y);
            const LevelIdentifier levelIdentifier = _levelManager->getLevelAt(x, y);
            level->setLevelIdentifier(levelIdentifier);
            if (_levelManager->isLevelConnectedEast(x, y)) {
                level->enableFlag(ConnectionE);
                Level *otherLevel = getLevel(x+1, y);
                if (otherLevel != 0) {
                    otherLevel->enableFlag(ConnectionW);
                }
            }
            if (_levelManager->isLevelConnectedSouth(x, y)) {
                level->enableFlag(ConnectionS);
                Level *otherLevel = getLevel(x, y+1);
                if (otherLevel != 0) {
                    otherLevel->enableFlag(ConnectionN);
                }
            }
            // Combine with the dynamic attributes from the game state.
            if (getGameState()->isLevelVisible(levelIdentifier)) {
                level->enableFlag(LevelVisible);
            }
            if (getGameState()->isLevelFinished(levelIdentifier)) {
                level->enableFlag(LevelFinished);
            }
            if (level->isLevel()) {
                level->setLevelColor((level->getLevelIdentifier()-LevelBase)/4);
            }
            if (getGameState()->getLastUncoveredLevel() == levelIdentifier) {
                level->setHighlight();
            }
        }
    }
    // Now in a second step, make briges visible
    for (uint8_t x = 0; x < _levelMapWidth; ++x) {
        for (uint8_t y = 0; y < _levelMapHeight; ++y) {
            // Extract static attributes from the level manager.
            Level *level = getLevel(x, y);
            if (level == 0) {
                continue;
            }
            // Check visible bridges
            if (level->isLevel() && level->isLevelVisible()) {
                // horizontal
                for (uint8_t dx = 1; (x+dx) < _levelMapWidth; ++dx) {
                    const Level *otherLevel = getLevel(x+dx, y);
                    if (otherLevel != 0) {
                        if (otherLevel->isHorizontalLine()) {
                            continue;
                        }
                        if (!otherLevel->hasConnectionW()||!otherLevel->isLevel()||!otherLevel->isLevelVisible()) {
                            break;
                        }
                        // yes, make a link.
                        for (uint8_t ddx = 0; ddx < dx; ++ddx) {
                            Level *connectedLevel = getLevel(x+ddx, y);
                            connectedLevel->enableFlag(VisibleBridgeE);
                            connectedLevel->enableFlag(LevelVisible);
                            if (connectedLevel->isHorizontalLine()) {
                                connectedLevel->setLevelColor(level->getLevelColor());
                            }
                        }
                        break;
                    }
                }
                // vertical
                for (uint8_t dy = 1; (y+dy) < _levelMapHeight; ++dy) {
                    const Level *otherLevel = getLevel(x, y+dy);
                    if (otherLevel != 0) {
                        if (otherLevel->isVerticalLine()) {
                            continue;
                        }
                        if (!otherLevel->hasConnectionN()||!otherLevel->isLevel()||!otherLevel->isLevelVisible()) {
                            break;
                        }
                        // yes, make a link.
                        for (uint8_t ddy = 0; ddy < dy; ++ddy) {
                            Level *connectedLevel = getLevel(x, y+ddy);
                            connectedLevel->enableFlag(VisibleBridgeS);
                            connectedLevel->enableFlag(LevelVisible);
                            if (connectedLevel->isVerticalLine()) {
                                connectedLevel->setLevelColor(level->getLevelColor());
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    // Set the initial position
    getLevelXY(_selectedLevel, &_x, &_y);
}

    
SelectLevelScene::~SelectLevelScene()
{
    // Free the level map resources.
    delete[] _levelMap;
}
   
    
SelectLevelScene::Level* SelectLevelScene::getLevel(const int8_t x, const int8_t y)
{
    if (x < 0 || y < 0 || x >= _levelMapWidth || y >= _levelMapHeight) {
        return 0;
    }
    const uint8_t index = x + (_levelMapWidth * y);
    if (index >= _levelMapCount) {
        return 0;
    }
    return &_levelMap[index];
}

    
bool SelectLevelScene::getLevelXY(const LevelIdentifier levelIdentifier, int8_t *x, int8_t *y) const
{
    for (uint8_t i = 0; i < _levelMapCount; ++i) {
        if (_levelMap[i].getLevelIdentifier() == levelIdentifier) {
            *x = _levelMap[i].getX();
            *y = _levelMap[i].getY();
            return true;
        }
    }
    return false;
}

    
void SelectLevelScene::running(const uint32_t &frame)
{
    // Fill the background.
    meg.fillRectS(0, 0, meg.getScreenWidth(), meg.getScreenHeight(), Color(0, 1, 2));
    
    // Draw the levels
    drawLevels(frame);
    
    // Draw the player
    if ((frame & 0x7f) == 0) {
        meg.setPixelS(3, 3, Color::maximum());
    } else {
        meg.setPixelS(3, 3, Color(0xf, 0x7, 0x8));
    }
    
    // The logic
    if (_state == WaitForUserInput) {
        const Level *level = getLevel(_x, _y);
        if (meg.isBButtonPressed()) {
            setNextScene(Scene::IntroId);
            setState(SceneEnding);
            _lastFrame = frame;
        } else if (meg.isAButtonPressed()) {
            // Start the given level.
            getGameState()->setLevel(_selectedLevel);
            setNextScene(Scene::GameId);
            setState(SceneEnding);
            _lastFrame = frame;
        } else if (meg.isUpButtonDown()) {
            if (_y > 0 && level->hasConnectionN() && getLevel(_x, _y-1)->isLevelVisible()) {
                _state = ScrollingN;
                _lastFrame = frame;
            }
        } else if (meg.isDownButtonDown()) {
            if (_y+1 < _levelMapHeight && level->hasConnectionS() && getLevel(_x, _y+1)->isLevelVisible()) {
                _state = ScrollingS;
                _lastFrame = frame;
            }
        } else if (meg.isLeftButtonDown()) {
            if (_x > 0 && level->hasConnectionW() && getLevel(_x-1, _y)->isLevelVisible()) {
                _state = ScrollingW;
                _lastFrame = frame;
            }
        } else if (meg.isRightButtonDown()) {
            if (_x+1 < _levelMapWidth && level->hasConnectionE() && getLevel(_x+1, _y)->isLevelVisible()) {
                _state = ScrollingE;
                _lastFrame = frame;
            }
        }
    } else {
        if ((frame - _lastFrame) > 5) {
            _lastFrame = frame;
            int8_t dx = 0;
            int8_t dy = 0;
            switch (_state) {
                case ScrollingN: dy = -1; break;
                case ScrollingE: dx = 1; break;
                case ScrollingS: dy = 1; break;
                case ScrollingW: dx = -1; break;
                default: break;
            }
            _dx += dx;
            _dy += dy;
            if (_dx == 4 || _dx == -4 || _dy == 4 || _dy == -4) {
                _x += dx;
                _y += dy;
                _dx = 0;
                _dy = 0;
                const Level *level = getLevel(_x, _y);
                if (level->getLevelIdentifier() == LevelHorizontalLine || level->getLevelIdentifier() == LevelVerticalLine) {
                    // ignore and continue
                } else {
                    // stop and wait for user input.
                    _state = WaitForUserInput;
                    _selectedLevel = level->getLevelIdentifier();
                }
            }
        }
    }
}

    
void SelectLevelScene::drawLevels(const uint32_t &frame)
{
    const Color shadowColor = Color(0, 0, 0);
    for (int8_t dx = -3; dx < 4; ++dx) {
        for (int8_t dy = -3; dy < 4; ++dy) {
            // Skip if out of range.
            const int8_t lx = _x+dx;
            const int8_t ly = _y+dy;
            // Get the level at the given position.
            const Level *level = getLevel(lx, ly);
            if (level == 0) {
                continue;
            }
            if (level->getLevelIdentifier() == NoLevel || !level->isLevelVisible()) {
                continue;
            }
            // draw the level with shadow.
            const int8_t x = 2 + (dx*4) - _dx;
            const int8_t y = 2 + (dy*4) - _dy;
            const Color levelColor = Color::fromProgMem(&levelColors[level->getLevelColor()]);
            if (level->getLevelIdentifier() == LevelHorizontalLine) {
                meg.fillRectS(x+1, y+2, 3, 1, shadowColor);
                meg.fillRectS(x, y+1, 3, 1, levelColor);
            } else if (level->getLevelIdentifier() == LevelVerticalLine) {
                meg.fillRectS(x+2, y+1, 1, 3, shadowColor);
                meg.fillRectS(x+1, y, 1, 3, levelColor);
            } else if (level->isLevel()) {
                meg.fillRectS(x+1, y+1, 3, 3, shadowColor);
                meg.fillRectS(x, y, 3, 3, levelColor);
            }
            // draw a bridge
            if (level->hasVisibleBridgeE()) {
                meg.setPixelS(x+4, y+2, shadowColor);
                meg.setPixelS(x+3, y+1, levelColor);
            }
            if (level->hasVisibleBridgeS()) {
                meg.setPixelS(x+2, y+4, shadowColor);
                meg.setPixelS(x+1, y+3, levelColor);
            }
            
            // If the level is highlighted animate the highlight
            if (level->isHighlighted()) {
                Color highlightColor = Color::maximum();
                if ((frame & 0x07) == 0) {
                    switch ((frame >> 3) & 0x3) {
                        case 0:
                            meg.setPixelS(x, y, highlightColor);
                            meg.setPixelS(x+2, y+2, highlightColor);
                            break;
                        case 1:
                            meg.setPixelS(x, y+1, highlightColor);
                            meg.setPixelS(x+2, y+1, highlightColor);
                            break;
                        case 2:
                            meg.setPixelS(x, y+2, highlightColor);
                            meg.setPixelS(x+2, y, highlightColor);
                            break;
                        case 3:
                            meg.setPixelS(x+1, y+2, highlightColor);
                            meg.setPixelS(x+1, y, highlightColor);
                            break;
                    }
                }
            }
        }
    }
}

    
void SelectLevelScene::ending(const uint32_t &frame)
{
    drawLevels(frame);
    const uint8_t phase = ((frame-_lastFrame) / 3);
    meg.fillRectS(3-phase, 3-phase, phase*2+1, phase*2+1, Color::white());
    if (phase > 0) {
        meg.fillRectS(3-(phase-1), 3-(phase-1), (phase-1)*2+1, (phase-1)*2+1, Color::black());
    }
    if (phase == 8) {
        setState(SceneSwitch);
    }
}
 
    
void SelectLevelScene::starting(const uint32_t &frame)
{
    if (_lastFrame == 0) {
        _lastFrame = frame;
    }
    drawLevels(frame);
    const uint8_t phase = 8-((frame-_lastFrame) / 3);
    meg.fillRectS(3-phase, 3-phase, phase*2+1, phase*2+1, Color::white());
    if (phase > 0) {
        meg.fillRectS(3-(phase-1), 3-(phase-1), (phase-1)*2+1, (phase-1)*2+1, Color::black());
    }
    if (phase == 0) {
        setState(SceneRunning);
    }
}

    
    
}



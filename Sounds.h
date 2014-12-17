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

#include <LRMeggyJr.h>


namespace lr {


/// The sound if a coin was collected.
extern const SoundToken PROGMEM kSoundCoin[];

/// The sound if a coin was collected.
extern const SoundToken PROGMEM kSoundNewLive[];

/// The jump sound.
extern const SoundToken PROGMEM kSoundJump[];
    
/// The sound if the player falls out of the level.
extern const SoundToken PROGMEM kSoundDeathFallDown[];

/// The sound if the player dies from a dynamic object.
extern const SoundToken PROGMEM kSoundDeathDynObject[];

/// The sound if the player successfully finishes a level
extern const SoundToken PROGMEM kSoundFinishLevel[];

/// The game over sound
extern const SoundToken PROGMEM kSoundGameOver[];

/// The sound if a live was lost
extern const SoundToken PROGMEM kSoundLiveLost[];

}



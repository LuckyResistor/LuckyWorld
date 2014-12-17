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

#include "Sounds.h"


namespace lr {
    

const SoundToken kSoundCoin[] PROGMEM = {
    PlaySpeed120,
    NoteA5, Play32,
    NoteA6, Play32,
    SoundEnd
};

    
const SoundToken kSoundNewLive[] PROGMEM = {
    PlaySpeed120,
    NoteE5, Play32,
    NoteG5, Play32,
    NoteH5, Play32,
    NoteC6, Play32,
    NoteE5, Play32,
    NoteG5, Play32,
    NoteH5, Play32,
    NoteC6, Play32,
    NoteE5, Play32,
    NoteG5, Play32,
    NoteH5, Play32,
    NoteC6, Play32,
    SoundEnd
};

    
const SoundToken kSoundJump[] PROGMEM = {
    PlaySpeed120,
    NoteShiftUp6,
    NoteA2, Play32,
    NoteA3, Play16,
    SoundEnd
};

    
const SoundToken kSoundDeathFallDown[] PROGMEM = {
    PlaySpeed120,
    NoteShiftDown3,
    NoteA3, Play2,
    SoundEnd
};

    
const SoundToken kSoundDeathDynObject[] PROGMEM = {
    PlaySpeed120,
    NoteC2, Play16,
    NoteD2, Play16,
    NoteC2, Play16,
    NoteA1, Play4,
    SoundEnd
};

    
const SoundToken kSoundFinishLevel[] PROGMEM = {
    PlaySpeed120,
    NoteC4, Play8,
    NoteE4, Play32,
    NoteC4, Play32, Play16,
    NoteG4, Play8,
    NoteF4, Play8,
    NoteE4, Play8,
    NoteF4, Play8,
    NoteC5, Play4,
    SoundEnd
};


const SoundToken kSoundGameOver[] PROGMEM = {
    PlaySpeed90,
    NoteG3, PlayWithPause4,
    Play8, PlayWithPause16, PlayWithPause16,
    Play4,
    NoteAs3, Play8, Play16,
    NoteA3, PlayWithPause16,
    Play8, Play16,
    NoteG3, PlayWithPause16,
    Play8, Play16,
    NoteFs3, Play16,
    NoteG3, Play4, Play8, Play16,
    SoundEnd
};

    
const SoundToken kSoundLiveLost[] PROGMEM = {
    PlaySpeed120,
    NoteC3, PlayStaccato16,
    NoteH2, PlayStaccato16,
    NoteA2, PlayStaccato16,
    SoundEnd
};

    
}



/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef ENIGMA_OXYD_HH
#define ENIGMA_OXYD_HH

#include "oxydlib/OxydVersion.h"

namespace oxyd
{
    void Init();
    void Shutdown();

    bool FoundOxyd (OxydLib::OxydVersion ver);

    /**
     * Select soundset or set default soundset.
     * @arg new_sound_set  Enigma = 1, OxydVersion number +2, or 0 for "use default"
     * @arg isDefault      true if a levelpack declares it default soundset,
     *                     false if the user selected soundset is given 
     */
    void ChangeSoundset(int new_sound_set, bool isDefault);
}

#endif

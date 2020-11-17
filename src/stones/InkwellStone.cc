/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008 Ronald Lamprecht
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
 */

#include "stones/InkwellStone.hh"
#include "errors.hh"

namespace enigma {

    InkwellStone::InkwellStone(int initState) : Stone () {
        state = initState;
    }

    std::string InkwellStone::getClass() const {
        return "st_inkwell";
    }
    
    Value InkwellStone::message(const Message &m) {
        return Stone::message(m);
    }
    
    int InkwellStone::maxState() const {
        return 6;
    }
    
    void InkwellStone::setState(int extState) {
        if (extState >= minState() && extState <= maxState()) {
            state = extState;
            if (isDisplayable())
                init_model();
        }
    }
        
    void InkwellStone::init_model() {
        if (state == BLACK_INK)
            set_model("st_inkwell_black");
        else if (state == WHITE_INK)
            set_model("st_inkwell_white");
        else if (state == BLACK_TO_EMPTY)
            set_anim("st_inkwell_black_to_empty");
        else if (state == EMPTY_TO_WHITE)
            set_anim("st_inkwell_empty_to_white");
        else if (state == WHITE_TO_EMPTY)
            set_anim("st_inkwell_white_to_empty");
        else if (state == EMPTY_TO_BLACK)
            set_anim("st_inkwell_empty_to_black");
        else // (state == EMPTY_WELL) and catch-all
            set_model("st_inkwell_empty");
    }
    
    void InkwellStone::animcb() {
        if (state == BLACK_TO_EMPTY)
            setState(EMPTY_WELL);
        else if (state == EMPTY_TO_WHITE)
            setState(WHITE_INK);
        else if (state == WHITE_TO_EMPTY)
            setState(EMPTY_WELL);
        else if (state == EMPTY_TO_BLACK)
            setState(BLACK_INK);
        else // should not actually happen
            setState(EMPTY_WELL);
    }
    
    void InkwellStone::actor_hit(const StoneContact &sc) {
        Value marblecolor = sc.actor->getAttr("color");
        if (state == BLACK_INK && marblecolor == GLASS) {
            if (SendMessage(sc.actor, "_setcolor", BLACK).to_bool()) {
                sound_event("drown");
                setState(BLACK_TO_EMPTY);
            }
        } else if (state == EMPTY_WELL && marblecolor == WHITE) {
            if (SendMessage(sc.actor, "_setcolor", GLASS).to_bool()) {
                sound_event("drown");
                setState(EMPTY_TO_WHITE);
            }
        } else if (state == WHITE_INK && marblecolor == GLASS) {
            if (SendMessage(sc.actor, "_setcolor", WHITE).to_bool()) {
                sound_event("drown");
                setState(WHITE_TO_EMPTY);
            }
        } else if (state == EMPTY_WELL && marblecolor == BLACK) {
            if (SendMessage(sc.actor, "_setcolor", GLASS).to_bool()) {
                sound_event("drown");
                setState(EMPTY_TO_BLACK);
            }
        }
    }
    
    DEF_TRAITS(InkwellStone, "st_inkwell", st_inkwell);

    BOOT_REGISTER_START
        BootRegister(new InkwellStone(2), "st_inkwell");
        BootRegister(new InkwellStone(0), "st_inkwell_black");
        BootRegister(new InkwellStone(2), "st_inkwell_empty");
        BootRegister(new InkwellStone(1), "st_inkwell_white");
    BOOT_REGISTER_END

} // namespace enigma

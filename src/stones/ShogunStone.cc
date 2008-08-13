/*
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
 *
 */

#include "stones/ShogunStone.hh"
#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    ShogunStone::ShogunStone(int initState) : Stone () {
        state = initState;
    }
    
    ShogunStone* ShogunStone::clone() {
        return new ShogunStone(*this);
    }
    
    void ShogunStone::dispose() {
         if (subShogun != NULL) {
            SendMessage(subShogun, "disconnect");
            DisposeObject(subShogun);
         }
         subShogun = NULL;
         superShogun = NULL;
         delete this;
    }

    std::string ShogunStone::getClass() const {
        return "st_shogun";
    }
    
    Value ShogunStone::message(const Message &m) {
        if (m.message == "kill") {
            if (yieldShogun()) {
                SendMessage(this, "disconnect");
                DisposeObject(this);
            }
            return Value();
        } else if (m.message =="_shogun") {
            return m.value == state;
        }
            return Stone::message(m);
    }

    void ShogunStone::setState(int extState) {
        if (!isDisplayable())
            Stone::setState(extState & 28);   // just not yet set shoguns with legal values
    }
    
    void ShogunStone::init_model() {
        set_model(ecl::strf("st-shogun%d", state/4));
    }
    
    void ShogunStone::setOwnerPos(GridPos po) {
        Stone::setOwnerPos(po);
        if (subShogun != NULL)
            subShogun->setOwnerPos(po);
    }
    
    void ShogunStone::on_creation(GridPos p) {
        Stone::on_creation(p);
        if (subShogun != NULL)
            // swap or pull based new grid positioning of a shogun stack
            subShogun->setOwnerPos(p);
        else if (state != ownHole()) {
            // initial set of a new shogun stack
            int subState = state & ~ownHole();
            if (subState & 8) {
                ShogunStone *s = dynamic_cast<ShogunStone *>(MakeObject("st_shogun_m"));
                subShogun = s;
                s->superShogun = this;
                s->setOwnerPos(p);
                s->state = subState;
                subState &= ~8;
                if (Value v = getAttr("name_m"))
                    NameObject(s, v.to_string());
            }
            if (subState & 4) {
                ShogunStone *s = dynamic_cast<ShogunStone *>(MakeObject("st_shogun_s"));
                if (subShogun == NULL) {
                    subShogun = s;
                    s->superShogun = this;
                } else {
                    subShogun->subShogun = s;
                    s->superShogun = subShogun;
                }
                if (Value v = getAttr("name_s"))
                    NameObject(s, v.to_string());
                s->setOwnerPos(p);
            }
        }
        SendMessage(GetItem(p), "_shogun", state);
    }
    
    void ShogunStone::on_removal(GridPos p) {
        SendMessage(GetItem(p), "_shogun", 0);
        if (subShogun != NULL)
            subShogun->setOwnerPos(GridPos(-1,-1));
        Stone::on_removal(p);
    }

    void ShogunStone::on_impulse(const Impulse& impulse) {
        static char * soundevent = "movesmall";
        
        if (!impulse.byWire && subShogun != NULL) {
            subShogun->on_impulse(impulse);
            return;
        }
        GridPos newPos = move(getOwnerPos(), impulse.dir);
        Stone * st = GetStone(newPos);
        ShogunStone *nss = NULL;
        bool fitsNeighborShogun = false;
        if (st != NULL) {
            nss = dynamic_cast<ShogunStone *>(st);
            if (nss != NULL)
                fitsNeighborShogun = (nss->state & (2*ownHole() -1)) == 0;
        }
        
        if ((st == NULL) || fitsNeighborShogun) {  // can we move?
            // first remove from current position
            if (!yieldShogun())
                return;            // being swapped or pulled
            
            sound_event(soundevent);
            
            // then put to new position
            if (st == NULL) {
                SetStone(newPos, this);
                SendMessage(GetItem(newPos), "_shogun", state);
            } else {
                // register our hole to all super shoguns
                ShogunStone *s = nss;
                for (; s->subShogun != NULL; s = s->subShogun) {
                    s->state |= ownHole();
                }
                // register ourself to smallest shogun
                s->state |= ownHole();
                s->subShogun = this;
                superShogun = s;

                nss->init_model();     // display new hole         
                SendMessage(GetItem(newPos), "_shogun", nss->state);
                setOwnerPos(newPos);   // the stone is owned at the new position
            }
            
            server::IncMoveCounter();
            ShatterActorsInsideField(newPos);
        }
        propagateImpulse(impulse);
    }
    
    int ShogunStone::ownHole() {
        return (state >= 16) ? 16 : ((state >= 8) ? 8 : 4);
    }
    
    bool ShogunStone::yieldShogun() {
        if (isDisplayable() && subShogun == NULL) {
            YieldStone(get_pos());
            SendMessage(GetItem(get_pos()), "_shogun", 0);
        } else if (isDisplayable()) {
            // top most shogun moved by wire or killed
            GridPos oldPos = get_pos(); 
            YieldStone(oldPos);
            subShogun->superShogun = NULL;
            SetStone(oldPos, subShogun);
            SendMessage(GetItem(oldPos), "_shogun", subShogun->state);
            subShogun = NULL;
            state = ownHole();
        } else {
            // a sub shogun
            ShogunStone *oss = dynamic_cast<ShogunStone *>(GetStone(getOwnerPos()));
            if (oss == NULL)   // we are swapped or pulled and impulsed by wire
                return false;        // forget impulse
            
            ASSERT(superShogun != NULL, XLevelRuntime, "Shogun: missing super shogun");
            superShogun->subShogun = subShogun;
            superShogun->state &= ~ownHole();
            if (ShogunStone *superSuperShogun = superShogun->superShogun) {
                superSuperShogun->state &= ~ownHole();
                superSuperShogun->init_model();
                SendMessage(GetItem(getOwnerPos()), "_shogun", superSuperShogun->state);
            } else
                superShogun->init_model();
                SendMessage(GetItem(getOwnerPos()), "_shogun", superShogun->state);
            if (subShogun != NULL) subShogun->superShogun = superShogun;
            superShogun = NULL;
            subShogun = NULL;
            state = ownHole();
        }
        return true;
    }
    
    FreezeStatusBits ShogunStone::get_freeze_bits() {
        return (state == 4) ? FREEZEBIT_STANDARD : FREEZEBIT_NO_STONE;
    }
    
    DEF_TRAITSM(ShogunStone, "st_shogun", st_shogun, MOVABLE_IRREGULAR);
    
    BOOT_REGISTER_START
        BootRegister(new ShogunStone(4), "st_shogun");
        BootRegister(new ShogunStone(4), "st_shogun_s");
        BootRegister(new ShogunStone(8), "st_shogun_m");
        BootRegister(new ShogunStone(12), "st_shogun_sm");
        BootRegister(new ShogunStone(16), "st_shogun_l");
        BootRegister(new ShogunStone(20), "st_shogun_sl");
        BootRegister(new ShogunStone(24), "st_shogun_ml");
        BootRegister(new ShogunStone(28), "st_shogun_sml");
    BOOT_REGISTER_END

} // namespace enigma

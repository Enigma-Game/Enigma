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
    ShogunStone::ShogunStone(int holes) : Stone () {
        objFlags |= holes << 24;
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

    void ShogunStone::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            ASSERT(!isDisplayable(), XLevelRuntime, "ShogunStone: attempt to reflavor an existing shogun");
            std::string flavor = val.to_string();
            int holes = 0;
            if (flavor.find('n') != std::string::npos)  holes += ShogunStone::N;
            if (flavor.find('t') != std::string::npos)  holes += ShogunStone::T;
            if (flavor.find('s') != std::string::npos)  holes += ShogunStone::S;
            if (flavor.find('m') != std::string::npos)  holes += ShogunStone::M;
            if (flavor.find('l') != std::string::npos)  holes += ShogunStone::L;
            if (flavor.find('g') != std::string::npos)  holes += ShogunStone::G;
            if (flavor.find('u') != std::string::npos)  holes += ShogunStone::U;
            ASSERT(holes != 0, XLevelRuntime, ecl::strf("ShogunStone: illegal 'flavor' of '%s'", flavor.c_str()).c_str());
            objFlags &= ~OBJBIT_HOLES;
            objFlags |= holes << 24;
            return;
        }
        Stone::setAttr(key, val);
    }

    Value ShogunStone::getAttr(const string &key) const {
        if (key == "flavor") {
            std::string result;
            int holes = getHoles();
            if (holes & ShogunStone::N) result += "n";
            if (holes & ShogunStone::T) result += "t";
            if (holes & ShogunStone::S) result += "s";
            if (holes & ShogunStone::M) result += "m";
            if (holes & ShogunStone::L) result += "l";
            if (holes & ShogunStone::G) result += "g";
            if (holes & ShogunStone::U) result += "u";
            return result;
        } else
            return Stone::getAttr(key);
    }

    Value ShogunStone::message(const Message &m) {
        if (m.message == "kill") {
            if (yieldShogun()) {
                SendMessage(this, "disconnect");
                DisposeObject(this);
            }
            return Value();
        } else if (m.message =="_shogun") {
            return m.value == getHoles();
        }
            return Stone::message(m);
    }

    void ShogunStone::setState(int extState) {
        // reject any write attempts
    }

    void ShogunStone::init_model() {
        set_model(ecl::strf("st_shogun%d", getHoles()/S));
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
        else if (getHoles() != ownHole()) {
            // initial set of a new shogun stack
            int subHoles = getHoles() & ~ownHole();
            if (subHoles & M) {
                ShogunStone *s = dynamic_cast<ShogunStone *>(MakeObject("st_shogun_m"));
                subShogun = s;
                s->superShogun = this;
                s->setOwnerPos(p);
                s->addSubHoles(subHoles);
                subHoles &= ~M;
                if (Value v = getAttr("name_m"))
                    NameObject(s, v.to_string());
            }
            if (subHoles & S) {
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
    }

    void ShogunStone::on_removal(GridPos p) {
        if (subShogun != NULL)
            subShogun->setOwnerPos(GridPos(-1,-1));
        Stone::on_removal(p);
    }

    void ShogunStone::on_impulse(const Impulse& impulse) {
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
                fitsNeighborShogun = (nss->getHoles() & (2*ownHole() -1)) == 0;
        }

        if ((subShogun == NULL) && ((st == NULL) || fitsNeighborShogun)) {  // can we move?
            // first remove from current position
            if (!yieldShogun())
                return;            // being swapped or pulled

            sound_event("movesmall");

            // then put to new position
            if (st == NULL) {
                SetStone(newPos, this);
            } else {
                // register our hole to all super shoguns
                ShogunStone *s = nss;
                for (; s->subShogun != NULL; s = s->subShogun) {
                    s->addSubHoles(ownHole());
                }
                // register ourself to smallest shogun
                s->addSubHoles(ownHole());
                s->subShogun = this;
                superShogun = s;

                nss->init_model();     // display new hole
                setOwnerPos(newPos);   // the stone is owned at the new position
                TouchStone(newPos);
            }

            server::IncMoveCounter();
            ShatterActorsInsideField(newPos);
            if (server::GameCompatibility != GAMET_ENIGMA) {
                if (Item *it = GetItem(newPos)) {
                    ItemID theid = get_id(it);
                    if ((server::GameCompatibility != GAMET_OXYD1 && server::GameCompatibility != GAMET_OXYDMAGNUM) ||
                            (theid != it_cherry && theid != it_bomb))
                        it->on_stonehit(this);
                }
            }
        }
        propagateImpulse(impulse);
    }

    int ShogunStone::getHoles() const {
        return (objFlags & OBJBIT_HOLES) >> 24;
    }

    int ShogunStone::ownHole() const {
        int holes = getHoles();
        for (int check = ShogunStone::U; check > 0; check = check >> 1)
            if (holes & check)
                return check;
        throw XLevelRuntime("ShogunStone: internal error - no holes");
    }

    void ShogunStone::addSubHoles(int holes) {
        objFlags |= holes <<24;
    }

    void ShogunStone::removeSubHoles(int holes) {
        objFlags &= ~(holes <<24);
    }

    void ShogunStone::removeAllSubHoles() {
        int hole = ownHole();
        objFlags &= ~OBJBIT_HOLES;
        objFlags |= hole <<24;
    }

    bool ShogunStone::yieldShogun() {
        if (isDisplayable() && subShogun == NULL) {
            YieldStone(get_pos());
        } else if (isDisplayable()) {
            // top most shogun moved by wire or killed
            GridPos oldPos = get_pos();
            YieldStone(oldPos);
            subShogun->superShogun = NULL;
            SetStone(oldPos, subShogun);
            subShogun = NULL;
            removeAllSubHoles();
        } else {
            // a sub shogun
            ShogunStone *oss = dynamic_cast<ShogunStone *>(GetStone(getOwnerPos()));
            if (oss == NULL)   // we are swapped or pulled and impulsed by wire
                return false;        // forget impulse

            ASSERT(superShogun != NULL, XLevelRuntime, "Shogun: missing super shogun");
            superShogun->subShogun = subShogun;
            superShogun->removeSubHoles(ownHole());
            if (ShogunStone *superSuperShogun = superShogun->superShogun) {
                superSuperShogun->removeSubHoles(ownHole());
                superSuperShogun->init_model();
                TouchStone(getOwnerPos());
            } else
                superShogun->init_model();
                TouchStone(getOwnerPos());
            if (subShogun != NULL) subShogun->superShogun = superShogun;
            superShogun = NULL;
            subShogun = NULL;
            removeAllSubHoles();
        }
        return true;
    }

    FreezeStatusBits ShogunStone::get_freeze_bits() {
        return (getHoles() == S) ? FREEZEBIT_STANDARD : FREEZEBIT_NO_STONE;
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

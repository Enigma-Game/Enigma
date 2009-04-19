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
 *
 */

#include "items/BlockerItem.hh"
#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

    BlockerItem::BlockerItem(bool shrinked_recently) {
        state = shrinked_recently ? NEW : IDLE;
    }

    BlockerItem::~BlockerItem() {
        GameTimer.remove_alarm (this);
    }

    BlockerItem * BlockerItem::clone() {
        BlockerItem * dup = new BlockerItem(*this);
        NameObject(dup, "$BlockerItem#");  // auto name object to avoid problems with object values
        return dup;
    }
    
    void BlockerItem::dispose() {
        delete this;
    }

    Value BlockerItem::message(const Message &m) {
        if (m.message == "_init") { 
            if (Stone *st = GetStone(get_pos())) {
                if (st->is_kind("st_boulder"))
                    if (state == IDLE && server::GameCompatibility != GAMET_PEROXYD)
                        setIState(UNLOCKED);
                    else if (state == NEW || server::GameCompatibility != GAMET_PEROXYD)
                        setIState(LOCKED);
            }
            return Item::message(m);    // pass on init message
        } else if (m.message =="_performaction") {
            performAction(true);
            return Value();
        }
        return Item::message(m);
    }
    
    void BlockerItem::toggleState() {
        if (state == UNLOCKED) {  // revoke pending grow/close
            setIState(LOCKED);
        }
        else {
            setState(0);  // close
        }
    }
    
    int BlockerItem::externalState() const {
        return 1;   // always open -- st_blocker is closed
    }
    
    void BlockerItem::setState(int extState) {
        if (isDisplayable())
            stone_change(GetStone(get_pos()));
        if (extState == 1) {         // open (shrink)
            if (state == UNLOCKED)   //   revoke pending grow/close
                setIState(LOCKED);
        } else {                       // close (grow)
            ASSERT(isDisplayable(), XLevelRuntime, "Blocker item: attempt to set initial state CLOSED - use 'st_blocker'.");
            switch (state) {
                case LOCKED:
                    setIState(UNLOCKED);  // close when stone is removed
                    break;
                case UNLOCKED:
                    break;                // will close anyway when stone is removed
                case IDLE:
                case NEW:
                    grow();                    
                    break;
            }
        }
    }
    
    void BlockerItem::on_creation(GridPos p) {
        if (state == NEW) {
            GameTimer.set_alarm(this, 0.5, false);
        }
        Item::on_creation(p);
    }

    void BlockerItem::on_removal(GridPos p) {
        setIState(IDLE);
        Item::on_removal(p);
    }

    void BlockerItem::init_model() {
        set_model("it-blocker");
    }
    
    void BlockerItem::actor_leave(Actor *a) {
        if (Value v = getAttr("autoclose")) {
            if (v.to_bool()) {
                setState(0);     // close
            }
        }
    }
    
    void BlockerItem::stone_change(Stone *st) {
        if (st != NULL) {
            if (st->is_kind("st_boulder")) { // boulder arrived
                switch (state) {
                    case IDLE:
                        setIState(UNLOCKED);  // will grow when boulder moves away
                        break;
                    case NEW:
                        setIState(LOCKED);    // will not grow when boulder moves away
                        break;
                    case UNLOCKED:
                    case LOCKED:
                        // two BoulderStones running directly next to each other
                        // let second pass as well (correct? siegfried says yes)
                        // note: all stone moves are handled in a timestep before
                        //   the world informs the items about stone changes
                        break;
                }
            }
            else { // any other stone
                switch (state) {
                    case IDLE:
                    case NEW:
                    case LOCKED:
                        setIState(LOCKED);
                        break;
                    case UNLOCKED:
                        // case appears if a stone is pushed from a trigger to a neighboring
                        // idle blocker item with the releasing trigger toggeling the blocker, too
                        break;
                }
            }
        }
        else {              // stone disappeared
            switch (state) {
                case LOCKED:
                    setIState(IDLE);
                    break;
                case UNLOCKED:
                    grow();
                    break;
                case IDLE:
                case NEW:
                    // no action
                    break;
            }
        }
    }

    void BlockerItem::alarm() {
        if (state == NEW) { // BoulderStone did not arrive in time
            setIState(IDLE);
        }
    }

    void BlockerItem::setIState(iState newState) {
        if (state != newState) {
            if (state == NEW)
                GameTimer.remove_alarm(this);
            else if (newState == NEW)
                GameTimer.set_alarm(this, 0.5, false);
            state = newState;
        }
    }
    
    void BlockerItem::grow() {
        Stone *st = MakeStone("st_blocker_new");
        SetStone(get_pos(), st);
        transferIdentity(st);
        if (Value v = getAttr("autoclose"))
            st->setAttr("autoclose", v); 
        SendMessage(st, "_performaction");
        kill();
    }

    DEF_ITEMTRAITSF(BlockerItem, "it_blocker", it_blocker, itf_static | itf_indestructible);

    BOOT_REGISTER_START
        BootRegister(new BlockerItem(false), "it_blocker");
        BootRegister(new BlockerItem(true), "it_blocker_new");
    BOOT_REGISTER_END

} // namespace enigma

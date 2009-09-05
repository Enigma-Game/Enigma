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

#include "floors/FloodStream.hh"

//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {
    
    FloodStream::FloodStream(int subtyp, int model, bool framed, FloorFlags flags,  bool isFloodSource) :
                Floor("fl_floodstream", 0.0, 0.0, flags) {
        objFlags |= (subtyp << 24) | (model << 26);
        state = isFloodSource ? FLOODING : IDLE;
        Floor::setAttr("faces", framed ? "nesw" : "");
    }
    
    FloodStream::~FloodStream() {
        GameTimer.remove_alarm(this);
    }
    
    std::string FloodStream::getClass() const {
        switch (getTyp()) { 
            case WATER : return "fl_water";
            case WOOD :  return "fl_wood";
            case HAY :   return "fl_hay";
            case ROCK :  return "fl_rock";
        }   
    }

    Value FloodStream::message(const Message &m) {
        if (m.message == "_checkflood") {
            Item *it = GetItem(get_pos());
            if (state == FLOODING && (m.value.to_bool() != true || (it != NULL && it->isKind("it_vortex_open"))))
                GameTimer.set_alarm(this, (double)getAttr("interval"), false);
            return Value();
        } else
            return Floor::message(m);
    }

    void FloodStream::setState(int extState) {
        if (isDisplayable() && extState == FLOODING && state == IDLE) {
            if (getTyp() == WATER || getAttr("faces").to_string() == "nesw") {
                state = FLOODING;
                GameTimer.set_alarm(this, (double)getAttr("interval"), false);
            }
        } else if (extState == IDLE) {
            state == IDLE;
            GameTimer.remove_alarm(this);
        } else
            state = extState;
    }
    
    std::string FloodStream::getModelName() const {
        std::string modelbase = getClass();
        if (getAttr("faces").to_string() == "nesw")
             modelbase += "_framed";
        if (getTyp() == WOOD) {
            int modelnr = (objFlags & OBJBIT_MODEL) >> 26;
            modelbase += (modelnr == 0 ) ? "" : ecl::strf("%d", modelnr); 
        }
        return modelbase;
    }
    
    void FloodStream::on_creation(GridPos p) {
        Floor::on_creation(p);
        if (state == FLOODING)
            GameTimer.set_alarm(this, (double)getAttr("interval"), false);
    }
    
    bool FloodStream::is_destructible() const {
        return getTyp() != WATER;  // all flood stream floors besides wood can be destroyed
    }

    void FloodStream::get_sink_speed (double &sink_speed, double &raise_speed) const { 
        if (getTyp() == WATER) {
            sink_speed = server::WaterSinkSpeed;
            raise_speed = 1000; // always sink in water
        } else
            return Floor::get_sink_speed (sink_speed, raise_speed);
    }
    
    void FloodStream::stone_change(Stone *st) {
        if (getTyp() == WATER || getAttr("faces").to_string() == "nesw") {
            SendMessage(GetFloor(get_pos()), "_checkflood");
            for (Direction d = NORTH; d != NODIR; d = previous(d))
                SendMessage(GetFloor(move(get_pos(), d)), "_checkflood");
        } else
            Floor::stone_change(st);
    }

    void FloodStream::alarm() {
        if (isDisplayable()) {
            Stone *thisstone = GetStone(get_pos());
            for (Direction d = NORTH; d != NODIR; d = previous(d)) {
                if (thisstone == NULL || thisstone->allowsSpreading(d)) {
                    tryFlood(move(get_pos(), d), reverse(d));
                }
            }
            if (Item *thisItem = GetItem(get_pos())) {
                std::list<GridPos> results = thisItem->warpSpreadPos(true);
                for (std::list<GridPos>::iterator itr = results.begin(); itr != results.end(); ++itr)
                    tryFlood(*itr, NODIR);
            }
        }
    }
 
    void FloodStream::tryFlood(GridPos p, Direction from) {
        if (IsInsideLevel(p)) {
            Floor *f = GetFloor(p);
            if (f != NULL && f->isKind("fl_floodstream") && 
                    (f->getAttr("faces").to_string() == "nesw" || f->isKind("fl_water"))) {
                if (f->isKind("fl_water") && f->getAttr("state") == IDLE) {
                    init_model();  // make flood visible
                    Item *it = GetItem(p);
                    if (it != NULL && (it->isKind("it_crack") || (it->isKind("it_burnable") && !it->isKind("it_burnable_oil"))))
                        KillItem(p);
                }
                f->setAttr("interval", getAttr("interval"));
                f->setAttr("state", FLOODING);
            } else if (f != NULL && f->getAttr("floodable").to_bool()) {
                Stone *st = GetStone(p);
                if (st == NULL || from == NODIR || st->allowsSpreading(from, true)) {
                    Floor *newfloor = MakeFloor("fl_water_source");
                    newfloor->setAttr("interval", getAttr("interval"));
                    SetFloor(p, newfloor);
                    Item *it = GetItem(p);
                    if (it != NULL && (it->isKind("it_crack") || (it->isKind("it_burnable") && !it->isKind("it_burnable_oil"))))
                        KillItem(p);
                }
            }
        }
    }
       
    FloodStream::FloodStreamTyp FloodStream::getTyp() const {
        return (FloodStreamTyp)((objFlags & OBJBIT_SUBTYP) >> 24);
    } 
    
    BOOT_REGISTER_START
        BootRegister(new FloodStream(0, 0, false, flf_indestructible), "fl_water");
        BootRegister(new FloodStream(0, 0, false, flf_indestructible, true), "fl_water_source");
        BootRegister(new FloodStream(1, 0, false), "fl_wood");
        BootRegister(new FloodStream(1, 1, false), "fl_wood_h");
        BootRegister(new FloodStream(1, 2, false), "fl_wood_v");
        BootRegister(new FloodStream(1, 0, true), "fl_wood_framed");
        BootRegister(new FloodStream(1, 1, true), "fl_wood_framed_h");
        BootRegister(new FloodStream(1, 2, true), "fl_wood_framed_v");
        BootRegister(new FloodStream(2, 0, false), "fl_hay");
        BootRegister(new FloodStream(2, 0, true), "fl_hay_framed");
        BootRegister(new FloodStream(3, 0, false), "fl_rock");
        BootRegister(new FloodStream(3, 0, true), "fl_rock_framed");
    BOOT_REGISTER_END

} // namespace enigma

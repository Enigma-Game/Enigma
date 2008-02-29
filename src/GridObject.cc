/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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

#include "GridObject.hh"

#include "errors.hh"
#include "game.hh"
#include "laser.hh"
#include "lua.hh"
#include "main.hh"
#include "sound.hh"
#include "world.hh"

#include "ecl_util.hh"
#include "ecl_dict.hh"

#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <iomanip>


using namespace std;

namespace enigma {

// remove comment from define below to switch on verbose messaging
// note: VERBOSE_MESSAGES is defined in multiple source files!
// #define VERBOSE_MESSAGES


/* -------------------- GridObject implementation -------------------- */


    void GridObject::setOwner(int player) {
        ASSERT(pos.x < 0, XLevelRuntime,
            "GridObject: attempt to add object to owner inventory that is still on grid");
        pos.x = -1;
        pos.y = player;
    }
    
    Value GridObject::getOwner() {
        if (pos.x == -1 && pos.y != -1)
            return Value(pos.y);
        else
            return Value();
    }
    
    void GridObject::setOwnerPos(GridPos po) {
        ASSERT(pos.x < 0, XLevelRuntime,
            "GridObject: attempt to add object to owner inventory that is still on grid");
        if (po.x >= 0) {
            pos.x = -2 - po.x;
            pos.y = -2 - po.y;
        } else {
            pos.x = po.x;
            pos.y = po.y;
        }
    }
    
    GridPos GridObject::getOwnerPos() {
        if (pos.x <= -2)
            return GridPos(-2 - pos.x, -2 - pos.y);
        else 
            return pos;
    }
    
    bool GridObject::isDisplayable() {
        return pos.x >= 0;
    }
    
    display::Model *GridObject::set_anim (const std::string &mname) 
    {
        set_model (mname);
        display::Model *m = get_model();
        m->set_callback(this);
        return m;
    }
    
    bool GridObject::sound_event (const char *name, double vol)
    {
        return sound::EmitSoundEvent (name, get_pos().center(), getVolume(name, this, vol));
    }
 
    void GridObject::warning(const char *format, ...) const 
    {
        va_list        arg_ptr;
        const GridPos& position = get_pos();
    
        va_start(arg_ptr, format);
    
        fprintf(stderr, "%p \"%s\" at %i/%i: ", this, get_kind(), position.x, position.y);
        vfprintf(stderr, format, arg_ptr);
        fputc('\n', stderr);
    
        va_end(arg_ptr);
    }
    
    void GridObject::setAttr(const string& key, const Value &val) {
        if (key == "connections" || key == "faces") {
            int d = NODIRBIT;
            std::string vs(val);
            if (vs.find('n') != std::string::npos) d |= NORTHBIT;
            if (vs.find('e') != std::string::npos) d |= EASTBIT;
            if (vs.find('s') != std::string::npos) d |= SOUTHBIT;
            if (vs.find('w') != std::string::npos) d |= WESTBIT;
            if (key == "faces") d ^= ALL_DIRECTIONS;
            Object::setAttr("$connections", d);
            if (isDisplayable())
                init_model();
        } else if (key == "$connections") {
            Object::setAttr("$connections", val);
            if (isDisplayable())
                init_model();
        } else
            StateObject::setAttr(key, val);
    }
    
    Value GridObject::getAttr(const string &key) const {
        if (key == "connections" || (key == "faces")) {
            std::string result;
            DirectionBits db = (key == "connections") ? getConnections() :getFaces();
            if (db & NORTHBIT) result += "n";
            if (db & EASTBIT)  result += "e";
            if (db & SOUTHBIT) result += "s";
            if (db & WESTBIT)  result += "w";
            return result;
        } else
            return StateObject::getAttr(key);
    }
    
    void GridObject::setState(int extState) {
        StateObject::setState(extState);
        init_model();
    }
    
    std::string GridObject::getModelName() const {
        return get_kind();
    }
    
    void GridObject::init_model() {
        DirectionBits c = getConnections();
        if (c != NODIRBIT)
            set_model(getModelName() + ecl::strf("%d", c));
        else
            set_model(getModelName());
    }

    DirectionBits GridObject::getConnections() const {
        if (Value v = getAttr("$connections"))
            return DirectionBits((int)v);
        else
            return NODIRBIT;
    }
    
    DirectionBits GridObject::getFaces() const {
        return DirectionBits(ALL_DIRECTIONS ^ getConnections());
    }
    
    void GridObject::on_creation(GridPos p) {
        init_model();
    }
    
    void GridObject::on_removal(GridPos p) {
        kill_model (p);
        if (objFlags & OBJBIT_PHOTOACTIV)
            deactivatePhoto();
    }
    
    
    // GridObject laser light support
    
    std::list<GridObject *> GridObject::photoSensorList;

    void GridObject::preLaserRecalc() {
        for (list<GridObject *>::iterator itr = photoSensorList.begin(); itr != photoSensorList.end(); ++itr) {
            uint32_t flags = (*itr)->objFlags;
            (*itr)->objFlags = (flags & ~OBJBIT_LIGHTALLDIRS) | ((flags & OBJBIT_LIGHTNEWDIRS) << 4);  // remember last laser bits, clear current ones
        }
    }
    
    void GridObject::postLaserRecalc() {
        for (list<GridObject *>::iterator itr = photoSensorList.begin(); itr != photoSensorList.end(); ) {
            list<GridObject *>::iterator witr = itr;  // work iterator for possible deletion of object
            ++itr;                                    // main iterator does no longer point to critical object
            uint32_t flags = (*witr)->objFlags;
            DirectionBits newDirs = (DirectionBits)(flags & OBJBIT_LIGHTNEWDIRS);
            DirectionBits oldDirs = (DirectionBits)((flags & OBJBIT_LIGHTOLDDIRS) >> 4);
            if (newDirs != oldDirs) {
                (*witr)->lightDirChanged(oldDirs, newDirs);
            }
        }
    }
    
    void GridObject::prepareLevel() {
        photoSensorList.clear();
    }

    void GridObject::processLight(Direction dir) {
        objFlags |= to_bits(dir);
    }
    
    DirectionBits GridObject::emissionDirections() const {
        return NODIRBIT;
    }
    
    void GridObject::activatePhoto() {
        ASSERT((objFlags & OBJBIT_PHOTOACTIV) == 0, XLevelRuntime , "GridObject: double photo sensor activation");
        photoSensorList.push_back(this);
        objFlags |= OBJBIT_PHOTOACTIV;
    }

    void GridObject::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
    }
    
    void GridObject::deactivatePhoto() {
        std::list<GridObject *>::iterator itr = std::find(photoSensorList.begin(), photoSensorList.end(), this);
        if (itr != photoSensorList.end())
            photoSensorList.erase(itr);
        objFlags &= ~OBJBIT_PHOTOACTIV;
        
    }
    
    DirectionBits GridObject::updateCurrentLightDirs() {
        DirectionBits result = NODIRBIT;
        for (Direction dir = NORTH; dir != NODIR; dir = (Direction)(dir - 1)) {
            if (LightFrom(get_pos(), reverse(dir)))
                result = DirectionBits(result | to_bits(dir));
        }
        objFlags = (objFlags & ~OBJBIT_LIGHTNEWDIRS) | result;
        return result;
    }
} // namespace enigma

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
#include "errors.hh"
#include "laser.hh"
#include "SoundEffectManager.hh"
#include "stones_internal.hh"
#include "server.hh"
#include "stones/LaserStone.hh"
#include <algorithm>
#include <cassert>
#include <map>

using namespace std;

using ecl::V2;

namespace enigma {

/* -------------------- LaserBeam -------------------- */

    ItemTraits LaserBeam::traits = {"it-laserbeam", it_laserbeam, 
                                    itf_static | itf_indestructible, 0.0 };



/* -------------------- LaserBeam -------------------- */

// The implementation of laser beams is a little tricky because, in
// spite of being implemented as Items, lasers aren't localized and
// changes to any part of the beam can affect the beam elsewhere.  A
// `change' may be anything from moving a stone in or out of the beam,
// rotating or moving one of the mirrors, to making a stone in the
// beam transparent.
//
// Here are a couple of facts about laser beams in Enigma:
//
// - Laser beams are static. Once calculated they do not change until
//   they are completely recalculated
//
// - LaserBeam::emit_from() is the only way to emit laser beams. A new
//   beam will propagate automatically and stops only if it comes
//   across an item or a stone that returns `false' from
//   Stone::is_transparent().
//
// - `processLight()' is called for objects in the beam *whenever*
//   the beam is recalculated.  For objects that need to be notified
//   when the laser goes on or off, use the `PhotoStone'
//   mixin.

std::list<LaserBeam *> LaserBeam::beamList;


void LaserBeam::prepareLevel() {
    beamList.clear();
}

void LaserBeam::init_model()
{
    DirectionBits directions = (DirectionBits)(objFlags & 15);
//    Log << "LaserBeam init model " << directions << " - " << objFlags << "\n";
    
    if (directions & (EASTBIT | WESTBIT)) {
        if (directions & (NORTHBIT | SOUTHBIT))
            set_model("it-laserhv");
        else
            set_model("it-laserh");
    }
    else if (directions & (NORTHBIT | SOUTHBIT))
        set_model("it-laserv");
}

void LaserBeam::processLight(Direction dir) {
    DirectionBits dirbit = to_bits(dir);
    
    if ((objFlags & 15 & dirbit) == 0) {
        // new direction
        objFlags |= dirbit;
        emit_from(get_pos(), dir);        
    }
}

void LaserBeam::on_creation(GridPos p)
{
    beamList.push_back(this);
    DirectionBits directions = (DirectionBits)(objFlags & 15);
    
    if (directions & EASTBIT) emit_from(p, EAST);
    else if (directions & WESTBIT) emit_from(p, WEST);
    else if (directions & NORTHBIT) emit_from(p, NORTH);
    else if (directions & SOUTHBIT) emit_from(p, SOUTH);
}

void LaserBeam::on_removal(GridPos p) {
    if ((objFlags & 15) != 0) {    // extraordinary removal of a laser beam
        beamList.erase(find(beamList.begin(), beamList.end(), this));
    }
    Item::on_removal(p);
}

void LaserBeam::emit_from(GridPos p, Direction dir)
{
    bool may_pass = true;

    p.move(dir);
    if (Stone *st = GetStone(p)) {
        may_pass = st->is_transparent (dir);
        st->processLight(dir);
    }

    if (may_pass) {
        if (Item *it = GetItem(p))
            it->processLight(dir);
        else {
            LaserBeam *lb = new LaserBeam(dir);
            SetItem(p, lb);
        }
    }
}

bool LaserBeam::actor_hit(Actor *actor)
{
    DirectionBits directions = (DirectionBits)(objFlags & 15);

    double r = get_radius(actor);
    V2 p = actor->get_pos();
    GridPos gp = get_pos();

    // distance of actor from center of the grid
    double dx = fabs(p[0] - gp.x - 0.5) - r;
    double dy = fabs(p[1] - gp.y - 0.5) - r;

    if ((directions & (EASTBIT | WESTBIT) && dy<-0.1) ||
        (directions & (NORTHBIT | SOUTHBIT)) && dx<-0.1)
    {
        SendMessage(actor, "_laserhit");
    }

    return false; // laser beams can't be picked up
}

void LaserBeam::kill_all()
{    
    
    for (list<LaserBeam *>::iterator itr = beamList.begin(); itr != beamList.end(); ++itr) {
        uint32_t flags = (*itr)->objFlags;
        (*itr)->objFlags = (flags & ~255) | ((flags & 15) << 4);  // remember last laser bits, clear current ones
    }
}

void LaserBeam::all_emitted()
{
    double x = 0, y = 0;
    int    count = 0;
    
    for (list<LaserBeam *>::iterator itr = beamList.begin(); itr != beamList.end(); ) {
        list<LaserBeam *>::iterator witr = itr;  // work iterator for possible deletion of object
        ++itr;                                   // main iterator does no longer point to critical object
        uint32_t flags = (*witr)->objFlags;
//        Log << "LaserBeam allemit px " << (*witr)->get_pos().x << " y " << (*witr)->get_pos().y << " f " << flags << "\n";
        uint32_t newDirs = flags & 15;
        if (newDirs == 0) {
            // this grid is now free of laserbeam
            KillItem((*witr)->get_pos());
            beamList.erase(witr);
        } else if ((flags & 240) != (newDirs << 4)) {
            // the laser beam on grid changed or is new
            (*witr)->init_model();
            
            // sound position calculation
            if ((((flags & 240) >> 4) & newDirs) != newDirs) {
                // a beam has been added here
                GridPos p = (*witr)->getOwnerPos();
                x += p.x;
                y += p.y;
                ++count;
            } 
        }
    }

    if (count) {
        sound::EmitSoundEvent ("laseron", ecl::V2(x/count+.5, y/count+.5),
                               getVolume("laseron", NULL));
    }
}

void LaserBeam::dispose()
{
    delete this;
}


//----------------------------------------------------------------------
// FUNCTIONS
//----------------------------------------------------------------------
namespace
{
    /* This flag is true iff all lasers should be recalculated at the
       end of the next tick. */
    bool light_recalc_scheduled = false;
}

void InitLasers() {


}


void MaybeRecalcLight(GridPos p) {
    light_recalc_scheduled |=
        (LightFrom(p, NORTH) || LightFrom(p, SOUTH) ||
         LightFrom(p, WEST) || LightFrom(p, EAST));
}

void RecalcLight() {
    light_recalc_scheduled = true;
}

bool LightFrom (GridPos p, Direction dir) {
    p.move(dir);
    if (GridObject *obj = GetStone(p)) 
        if (has_dir(obj->emissionDirections(), reverse(dir)))
            return true;
    if (GridObject *obj = GetItem(p))
        return (has_dir(obj->emissionDirections(), reverse(dir)));
    
    return false;
}

void PerformRecalcLight(bool isInit) {
    if (light_recalc_scheduled) {
        light_recalc_scheduled = false;    // this is the right place - but we have first to fix some object like hammer,...
        GridObject::preLaserRecalc();
        LaserBeam::kill_all();
        LaserStone::reemit_all();
        LaserBeam::all_emitted();
        if (!isInit)
            // do not cause actions on initial laser beam generation
            GridObject::postLaserRecalc();
//        light_recalc_scheduled = false;
    }
}

} // namespace enigma

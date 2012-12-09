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

#include "items/StripItem.hh"
#include "actors.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {
    StripItem::StripItem(std::string connections) {
        setAttr("connections", connections);
    }

    std::string StripItem::getClass() const {
        return "it_strip";
    }

    void StripItem::setState(int extState) {
        // block state set
    }

    std::string StripItem::getModelName() const {
        return getClass() +  ecl::strf("%d", getConnections());
    }

    void StripItem::init_model() {
        // need to bypass Item's implementation
        GridObject::init_model();
    }

    bool StripItem::covers_floor(ecl::V2 position, Actor *a) const {
        if (GridPos(position) != get_pos())
            return false;

        double velocity = 0;
        if (a != NULL)
            velocity = ecl::length(a->get_actorinfo()->vel);

        // calculate the maximal horizontal or vertical distance from the center:
        // gurantee that a large marble can touch a neighboring stone at speed 0,
        // and a fast marble does not step off the strip on hitting a neighbor stone.
        // A fast marble might not reach the stone in one timestep while being
        // on top of the strip and is afterwards moved forward off the strip.
        // Thus we must expand the strip with increasing speed of the actor - call
        // it a relativistic effect :-)
        // d = 1/2 - 19/64 - 0.02 + velocity/400
        //  = 1/2 - radius large marble - epsilon + velocity/(1/2.5 ms)
        // we need to limit the speed to avoid a fast marble traversing the gap in
        // two timesteps!
        double MAXDIST = 0.183125 + ecl::Clamp(velocity/400, 0.0, 0.125);

        double ycenter = get_pos().y + 0.5;
        double xcenter = get_pos().x + 0.5;
        DirectionBits cbits = getConnections();

        return (((fabs(position[1] - ycenter) <= MAXDIST) && ((fabs(position[0] - xcenter) <= MAXDIST)  ||
                   ((position[0] <= xcenter + MAXDIST) && (cbits & WESTBIT)) || ((position[0] >= xcenter - MAXDIST) && (cbits & EASTBIT))))
                || ((fabs(position[0] - xcenter) <= MAXDIST)
                && (((position[1] <= ycenter + MAXDIST) && (cbits & NORTHBIT)) || ((position[1] >= ycenter - MAXDIST) && (cbits & SOUTHBIT)))))
                ? true : false;
    }

    double StripItem::getFriction(ecl::V2 position, double defaultFriction, Actor *a) {
        Value v = getAttr("friction");
        if (v && covers_floor(position, a))
            return v;
        else
            return defaultFriction;
    }

    ecl::V2 StripItem::calcMouseforce(Actor *a, ecl::V2 mouseForce, ecl::V2 floorForce) {
        Value v = getAttr("adhesion");
        if (v && covers_floor(a->get_pos(), a))
            return mouseForce * (double)v ;
        else
            return floorForce;
    }

    int StripItem::traitsIdx() const {
        return getConnections();
    }

    ItemTraits StripItem::traits[16] = {
        {"it_strip",  it_strip,  itf_static | itf_freezable},
        {"it_strip_w",  it_strip_w,  itf_static | itf_freezable},
        {"it_strip_s",  it_strip_s,  itf_static | itf_freezable},
        {"it_strip_sw",  it_strip_sw,  itf_static | itf_freezable},
        {"it_strip_e",  it_strip_e,  itf_static | itf_freezable},
        {"it_strip_ew",  it_strip_ew,  itf_static | itf_freezable},
        {"it_strip_es",  it_strip_es,  itf_static | itf_freezable},
        {"it_strip_esw",  it_strip_esw,  itf_static | itf_freezable},
        {"it_strip_n",  it_strip_n,  itf_static | itf_freezable},
        {"it_strip_nw",  it_strip_nw,  itf_static | itf_freezable},
        {"it_strip_ns",  it_strip_ns,  itf_static | itf_freezable},
        {"it_strip_nsw",  it_strip_nsw,  itf_static | itf_freezable},
        {"it_strip_ne",  it_strip_ne,  itf_static | itf_freezable},
        {"it_strip_new",  it_strip_new,  itf_static | itf_freezable},
        {"it_strip_nes",  it_strip_nes,  itf_static | itf_freezable},
        {"it_strip_nesw",  it_strip_nesw,  itf_static | itf_freezable}
    };

    BOOT_REGISTER_START
        BootRegister(new StripItem(""), "it_strip");
        BootRegister(new StripItem("w"), "it_strip_w");
        BootRegister(new StripItem("s"), "it_strip_s");
        BootRegister(new StripItem("sw"), "it_strip_sw");
        BootRegister(new StripItem("e"), "it_strip_e");
        BootRegister(new StripItem("ew"), "it_strip_ew");
        BootRegister(new StripItem("es"), "it_strip_es");
        BootRegister(new StripItem("esw"), "it_strip_esw");
        BootRegister(new StripItem("n"), "it_strip_n");
        BootRegister(new StripItem("nw"), "it_strip_nw");
        BootRegister(new StripItem("ns"), "it_strip_ns");
        BootRegister(new StripItem("nsw"), "it_strip_nsw");
        BootRegister(new StripItem("ne"), "it_strip_ne");
        BootRegister(new StripItem("new"), "it_strip_new");
        BootRegister(new StripItem("nes"), "it_strip_nes");
        BootRegister(new StripItem("nesw"), "it_strip_nesw");
    BOOT_REGISTER_END

} // namespace enigma

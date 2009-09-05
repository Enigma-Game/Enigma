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

#include "items/PipeItem.hh"

//#include "main.hh"
#include "world.hh"

namespace enigma {

    /** 
     * About recursion detection while finding the end of a mailpipe
     *  
     * Since there are no possibilities for forking a mailpipe, there is only one 
     * cause that may lead to a closed, circular mailpipe. This is if a pipeitem is
     * placed exactly under the mailstone. But not every pipepiece is dangerous,
     * there are some that can be placed under the mailstone without problems.
     * 
     * The mailpipe is only closed to a circular one if the pipepiece under the
     * mailstone has the same 'output'-direction as the stone.
     */
    GridPos PipeItem::findPipeEndpoint(GridPos start, Direction dir) {
        GridPos p = start;
        Direction move_dir = dir;
    
        while (move_dir != NODIR) {
            p.move(move_dir);
            if (Item *it = GetItem(p)) {
                if (it->getClass() == "it_pipe") {
                    DirectionBits dbits = it->getConnections();
                    if (has_dir(dbits, reverse(move_dir))) {
                        dbits = (DirectionBits) (dbits & ~to_bits(reverse(move_dir)));
                        switch (dbits) {
                            case WESTBIT : move_dir = WEST; break;
                            case SOUTHBIT : move_dir = SOUTH; break;
                            case EASTBIT : move_dir = EAST; break;
                            case NORTHBIT : move_dir = NORTH; break;
                            default : return GridPos(-1, -1);         // illegal position outside of world
                        }
                    } else
                        move_dir = NODIR;
                } else
                    move_dir = NODIR;
            } else
                move_dir = NODIR;
    
            if (p == start && move_dir == dir)  // recursion detection
                return GridPos(-1, -1);         // illegal position outside of world
        }
        return p;
    }

    PipeItem::PipeItem(std::string connections) {
        setAttr("connections", connections);
    }
    
    std::string PipeItem::getClass() const {
        return "it_pipe";
    }
        
    int PipeItem::traitsIdx() const {
        static int idx[] = { 0, 0, 1, 2, 3, 4, 5, 0, 6, 7, 8, 0, 9, 0, 0, 0};
        return idx[getConnections()];
    }
    
    ItemTraits PipeItem::traits[10] = {
        {"it_pipe_w",  it_pipe_w,  itf_none, 0.0},
        {"it_pipe_s",  it_pipe_s,  itf_none, 0.0},
        {"it_pipe_sw", it_pipe_sw, itf_none, 0.0},
        {"it_pipe_e",  it_pipe_e,  itf_none, 0.0},
        {"it_pipe_ew", it_pipe_ew, itf_none, 0.0},
        {"it_pipe_es", it_pipe_es, itf_none, 0.0},
        {"it_pipe_n",  it_pipe_n,  itf_none, 0.0},
        {"it_pipe_nw", it_pipe_nw, itf_none, 0.0},
        {"it_pipe_ns", it_pipe_ns, itf_none, 0.0},
        {"it_pipe_ne", it_pipe_ne, itf_none, 0.0}
    };
    
    BOOT_REGISTER_START
        BootRegister(new PipeItem("ew"), "it_pipe");
        BootRegister(new PipeItem("w"),  "it_pipe_w");
        BootRegister(new PipeItem("s"),  "it_pipe_s");
        BootRegister(new PipeItem("sw"), "it_pipe_sw");
        BootRegister(new PipeItem("e"),  "it_pipe_e");
        BootRegister(new PipeItem("ew"), "it_pipe_ew");
        BootRegister(new PipeItem("es"), "it_pipe_es");
        BootRegister(new PipeItem("n"),  "it_pipe_n");
        BootRegister(new PipeItem("nw"), "it_pipe_nw");
        BootRegister(new PipeItem("ns"), "it_pipe_ns");
        BootRegister(new PipeItem("ne"), "it_pipe_ne");
    BOOT_REGISTER_END

} // namespace enigma

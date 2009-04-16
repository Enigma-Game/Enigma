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
#ifndef FLOODSTREAM_HH
#define FLOODSTREAM_HH

#include "floors.hh"

namespace enigma {

    /** 
     * 
     */
    class FloodStream : public Floor, public TimeHandler {
        CLONEOBJ(FloodStream);
                
    private:
        enum iState {
            IDLE,       ///< 
            FLOODING    ///< 
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_SUBTYP    =   3<<24,   ///< the FloodStream typ
            OBJBIT_MODEL     =   3<<26,   ///< the model typ (h/v for wood)
        };
        
        enum FloodStreamTyp {
            WATER = 0,
            WOOD,
            HAY,
            ROCK
        };
    public:
        FloodStream(int subtyp, int model, bool framed, 
                FloorFlags flags = flf_default, bool isFloodSource = false);
        ~FloodStream();

        // Object interface
        virtual std::string getClass() const;
        virtual const char *get_kind() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
                
        // Floor interface
        virtual bool is_destructible() const;
        virtual void get_sink_speed (double &sinkspeed, double &raisespeed) const;
        virtual void stone_change(Stone *st);

        // TimeHandler interface
        virtual void alarm();
    
    private:
        // Private methods.
        void tryFlood(GridPos p, Direction from =NODIR);
        FloodStreamTyp getTyp() const;
    };

} // namespace enigma

#endif

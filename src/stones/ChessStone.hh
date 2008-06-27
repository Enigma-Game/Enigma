/*
 * Copyright (C) 2007 Andreas Lochmann
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
#ifndef CHESSSTONE_HH
#define CHESSSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     * Note: this stone is made safe for swap/pull grid take offs by restriction of
     * all critical operations to the IDLE state.
     */
    class ChessStone : public Stone, public TimeHandler {
        CLONEOBJ(ChessStone);
        DECL_TRAITS;

    private:
        // Variables and Constants
        // (APPEARING and DISAPPEARING are used when moving the stone.)
        enum State {IDLE, APPEARING, DISAPPEARING, CAPTURING,
                    CAPTURED, FALLING, SINKING};

        // The first three boolean flags are meant as buffers to
        // remember state-changes. When the horse is just jumping
        // or capturing, no state-change can be done at once, but
        // only after jumping. To avoid shortcuts, this is
        // consequently done for floor-changes as well, which
        // means that a horse can (retroactively) sink or fall
        // though standing on normal floor after a jump, if the
        // origin floor became swamp or abyss in meanwhile.
        // The final 5 bits are used in the following way:
        // During a capture, the capturing stone has to give the
        // captured stone enough time to play its animation.
        // So 20 alarms of 0.1 seconds each are scheduled, to
        // wait before finally cancelling the capture.
        // OBJBIT_CAPTURE_RETRY is the corresponding counter.
        enum ObjectPrivatFlagsBits {
            OBJBIT_NEWCOLOR      =  1<<24, ///< change to this color next time possible
            OBJBIT_FALL          =  1<<25, ///< fall next time possible
            OBJBIT_SINK          =  1<<26, ///< sink next time possible
            OBJBIT_CAPTURE_RETRY = 31<<27  ///< count retries for capturing
        };

    public:
        ChessStone(int color);
        virtual ~ChessStone();
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual void on_impulse(const Impulse& impulse);
        virtual bool is_removable() const;
        virtual void on_floor_change();

        // TimeHandler interface
        void alarm();
        
    private:
        static double capture_interval;
        static double hit_threshold;
        static const int max_capture_retry = 20; // note definition of OBJBIT_CAPTURE_RETRY

        // Methods
        string get_model_name();
        Value maybe_move_to(Direction dir1, Direction dir2);
        Value message_move(Value moveDir);
        bool try_state(State newstate);
        void set_color(int color);
    };

} // namespace enigma

#endif

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
#ifndef PUZZLESTONE_HH
#define PUZZLESTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     *  Puzzle stones can be connected to other stones of the same type.  Any
     *  of the four faces of the stone can have ``socket''.  If the adjoining
     *  faces of two neighboring stones both have a socket, the two stones
     *  link up and henceforth move as group.
     *  
     *  A cluster of puzzle stones may for example look like this:
     *  
     *  +---+---+---+---+
     *  |   |   |   |   |
     *  | --+-+-+---+-+ |
     *  |   | | |   | | |
     *  +---+-+-+---+-+-+
     *      | | |   | | |
     *      | | |   | | |
     *      |   |   |   |
     *      +---+   +---+
     *  
     *  This example actually presents the special case of a ``complete''
     *  cluster.  A cluster is complete if none of its stones has an
     *  unconnected socket.
     *  
     *  When touched with a magic wand the puzzle stones rotate
     *  row- or columnwise.
     *   
     * fragment = connected
     * cluster = connected and complete
     * huddle  = adjacent
     */
    class PuzzleStone : public Stone, public TimeHandler {
        CLONEOBJ(PuzzleStone);
        DECL_TRAITS;
        
    private:
        enum iState {
            IDLE,       ///<
            FALLING,    ///< a cluster falling into water or abyss
            FALLEN,     ///< a stone that is fallen an is vanishing
            PENDING,    ///< a cluster gets locked on a pending explosion to avoid further actions
            IGNITED,    ///< 
            EXPLODING   ///< 
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_INIT     =   1<<24,   ///< we cluster just on first grid set and mark initialization
            OBJBIT_HOLLOW   =   1<<25,   ///< 
            OBJBIT_VISITED  =   1<<26,   ///< 
            OBJBIT_SINGLE   =   1<<27    ///< a single, unconnected stone - flag is continuously not updated! 
        };
        
        typedef std::list<PuzzleStone *> PuzzleList;

    public:
        PuzzleStone(int color, std::string connections, bool hollow = false);
        virtual ~PuzzleStone();
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
//        virtual int externalState() const;
        virtual void setState(int extState);
//        virtual void toggleState();

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void init_model();
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Stone interface
        virtual bool is_transparent(Direction d) const;
        virtual bool is_sticky(const Actor *a) const;
        virtual bool allowsSpreading(Direction dir) const;
        virtual bool is_removable() const;        
        virtual bool isConnectable(Stone *other) const;
        virtual StoneResponse collision_response(const StoneContact &sc);
        virtual void actor_hit(const StoneContact &sc);
        virtual void actor_contact(Actor *a);
        virtual void on_impulse(const Impulse& impulse);
        virtual bool on_move(const GridPos &origin);
        virtual void on_floor_change();
        
        // TimeHandler interface
        virtual void alarm();
        
    private:
        void markPuzzle(bool visited);
        void markList(PuzzleList &list, bool visited);
        bool checkConnected(PuzzleList &todo, PuzzleStone *source, Direction dir, int color);
        bool findFragment(PuzzleList &cluster, bool keepMarked = false);
        void checkNeighbor(PuzzleList &todo, PuzzleStone *source, Direction dir, int color);
        void findAdjacents(PuzzleList &huddle);
        int  findLine(PuzzleList &line, Direction dir, int color);
        
        bool moveCluster(bool waterSink, Direction dir, bool checkOnly = false);
        bool explodeCluster();
        void explodeStone();
        void triggerExplosion(GridPos p, double delay, int color);
        void ignite(double delay);
        void rotateLine(PuzzleList &line, bool forward = true);
        void rotateLine(Direction dir, bool forward = true);
        
        // volatile:
        GridPos sourcePos;   // source position on move
    };

} // namespace enigma

#endif

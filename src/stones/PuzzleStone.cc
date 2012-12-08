/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008,2009 Ronald Lamprecht
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

#include "stones/PuzzleStone.hh"
#include "errors.hh"
#include "lua.hh"
#include "main.hh"
#include "player.hh"
#include "server.hh"
#include "world.hh"
#include <algorithm>

namespace enigma {
    PuzzleStone::PuzzleStone(int color, std::string connections, bool hollow) : Stone() {
        setAttr("color", color);
        setAttr("connections", connections);
        setAttr("hollow", hollow);
    }

    PuzzleStone::~PuzzleStone() {
        GameTimer.remove_alarm (this);
    }

    std::string PuzzleStone::getClass() const {
        return "st_puzzle";
    }

    void PuzzleStone::setAttr(const string& key, const Value &val) {
        if (key == "hollow") {
            if (val.to_bool() != (bool)(objFlags & OBJBIT_HOLLOW)) {
                objFlags ^= OBJBIT_HOLLOW;
            }
            return;
        } else if (key == "connections") {
            Stone::setAttr(key, val);
            return;
        }
        Stone::setAttr(key, val);
    }

    Value PuzzleStone::getAttr(const std::string &key) const {
        if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
        }
        return Stone::getAttr(key);
    }

    Value PuzzleStone::message(const Message &m) {
        if (m.message == "_scramble") {
            // oxyd levels contain explicit information on how to
            // scramble puzzle stones. According to that information
            // a "scramble" message is send to specific puzzle stones
            // together with information about the direction.
            //
            // enigma levels may create scramble messages using
            // AddScramble() and SetScrambleIntensity()

            Direction dir = to_direction(m.value);
            PuzzleList c;
            int size = findLine(c, dir, (int)getAttr("color"));

            // warning("received 'scramble'. dir=%s size=%i", to_suffix(dir).c_str(), size);

            if (size >= 2) {
                int count = IntegerRand(0, static_cast<int> (size-1));
                while (count--)
                    rotateLine(c);
            }
            else {
                warning("useless scramble (line size=%i)", size);
            }
            return Value();
        } else if (m.message == "get_adjacents") {
            // need a message instead of getAttr as we write mark puzzles what contradicts "const"
            PuzzleList huddle;
            findAdjacents(huddle);
            ObjectList adj;
            for (PuzzleList::iterator itr = huddle.begin(); itr != huddle.end(); ++itr)
                adj.push_back(*itr);
            return adj;
        } else if (m.message == "get_cluster") {
            // need a message instead of getAttr as we write mark puzzles what contradicts "const"
            PuzzleList fragment;
            findFragment(fragment);
            ObjectList adj;
            for (PuzzleList::iterator itr = fragment.begin(); itr != fragment.end(); ++itr)
                adj.push_back(*itr);
            return adj;
        } else if (m.message == "is_complete") {
            // need a message instead of getAttr as we write mark puzzles what contradicts "const"
            PuzzleList fragment;
            return findFragment(fragment);
        } else if (m.message == "pull_rotate") {
            rotateLine(to_direction(m.value), false);
            return Value();
        } else if (m.message == "push_rotate") {
            rotateLine(to_direction(m.value), true);
            return Value();
        } else if (m.message == "_init") {
            updateCurrentLightDirs();
            return Value();
        } else if (m.message == "_cannonball") {
            explodeCluster();
            return Value();
        }
        return Stone::message(m);
    }

    void PuzzleStone::setState(int extState) {
        // block all write attempts
    }

    void PuzzleStone::init_model() {
        if (state == EXPLODING) {
            set_anim("it_explosion");
            return;
        }
        int m = getConnections();
        if (getAttr("color") == YELLOW) { m += 16; }
        if ((objFlags & OBJBIT_HOLLOW)) { m += 32; }
        set_model(ecl::strf("st_puzzle%d", m));
    }

    void PuzzleStone::on_creation(GridPos p) {
        if (!(objFlags & OBJBIT_INIT)) {
            autoJoinCluster();
            objFlags |= OBJBIT_INIT;
        }
        activatePhoto();
        updateCurrentLightDirs();
        Stone::on_creation(p);
    }

    void PuzzleStone::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (state == IDLE && !(objFlags & OBJBIT_HOLLOW)) {
            DirectionBits addDirs = added_dirs(oldDirs, newDirs);
            if (addDirs != NODIRBIT && !explodeCluster() && getAttr("color") == YELLOW) {
                GridPos p = get_pos();
                if (addDirs & SOUTHBIT) rotateLine(SOUTH);
                if (p == get_pos() && (addDirs & WESTBIT))  rotateLine(WEST);
                if (p == get_pos() && (addDirs & NORTHBIT)) rotateLine(NORTH);
                if (p == get_pos() && (addDirs & EASTBIT))  rotateLine(EAST);
                
                if (p != get_pos() && GetStone(p)->getAttr("hollow").to_bool()) {
                    // we are still enlighted via hollow successor
                    objFlags |= to_bits(direction_fromto(p, get_pos()));
                }
            }
        }
    }

    void PuzzleStone::animcb() {
        KillStone(get_pos());
    }

    bool PuzzleStone::is_transparent(Direction d) const {
        return objFlags & OBJBIT_HOLLOW;
    }

    bool PuzzleStone::is_sticky(const Actor *a) const {
        return !(objFlags & OBJBIT_HOLLOW);
    }

    bool PuzzleStone::allowsSpreading(Direction dir) const {
        return objFlags & OBJBIT_HOLLOW;
    }

    bool PuzzleStone::is_removable() const {
        return state == IDLE;
    }

    bool PuzzleStone::isConnectable(Stone *other) const {
        return Stone::isConnectable(other) && (other->getAttr("color") == getAttr("color"));
    }

    StoneResponse PuzzleStone::collision_response(const StoneContact &sc) {
        if ((objFlags & OBJBIT_HOLLOW) || state == EXPLODING)
            return STONE_PASS;
        return STONE_REBOUND;
    }

    void PuzzleStone::actor_hit(const StoneContact &sc) {
        if (objFlags & OBJBIT_HOLLOW)
            return;                 // Puzzle stone is hollow

        if (state != IDLE)
            return;

        Direction rotate_dir = reverse(contact_face(sc));
        Direction move_dir = get_push_direction(sc);

        if (getAttr("color") == YELLOW) {
            if (explodeCluster())
                return;

            // 1) If unconnected puzzle stones -> try to move it
            if ((objFlags & OBJBIT_SINGLE) && move_dir != NODIR) {
                // if cluster contains single stone
                // -> move it if dest pos is free
                GridPos dest = move(get_pos(), move_dir);
                if (GetStone(dest) == NULL) {
                    // move without falling in water or abyss
                    Stone *puzz = YieldStone(get_pos());
                    SetStone(dest, puzz);
                    puzz->on_move(get_pos());
                    sound_event ("movesmall");
                } else
                    rotateLine(rotate_dir);
            }
            // 2) If more than one stone,
            else
                rotateLine(rotate_dir);
        }
        else {
            // BLUE
            bool has_magic_wand = player::WieldedItemIs (sc.actor, "it_magicwand");

            // 1) Try to start explosion of complete cluster
            if (has_magic_wand && explodeCluster())
                return;

            // 2) Failed? Try to move the cluster
            if (move_dir != NODIR && moveCluster(true, move_dir, true)) {
                sc.actor->send_impulse(get_pos(), move_dir);
                return;
            }

            // 3) Last chance: try to rotate the row or column
            if (has_magic_wand) {
                rotateLine(rotate_dir);
            } else if (move_dir != NODIR) {
                // we can not move - otherwise we would have send the impulse in 2)
                // but we need to propagate the impulse
                sc.actor->send_impulse(get_pos(), move_dir);
            }
        }
    }

    void PuzzleStone::actor_contact (Actor *a) {
        if (state == EXPLODING || state == IGNITED)
            SendMessage(a, "_shatter");
    }

    void PuzzleStone::on_impulse(const Impulse& impulse) {
        if (state == IDLE) {
            bool    actor_with_wand = false;
            if (Actor *ac = dynamic_cast<Actor*>(impulse.sender))
                actor_with_wand = player::WieldedItemIs(ac, "it_magicwand");

            moveCluster(!actor_with_wand, impulse.dir);
        }
        propagateImpulse(impulse);
    }

    bool PuzzleStone::on_move(const GridPos &origin) {
        if (!(objFlags & OBJBIT_HOLLOW))
            ShatterActorsInsideField (get_pos());
        return true;
    }

    void PuzzleStone::on_floor_change() {
        if (state == FALLING) {
            GridPos p = get_pos();
            state = FALLEN;   // we need to mark the puzzle as the floor set recalls this methods
            if (server::FallenPuzzle.find('=') == 0) {
                if (lua::CallFunc(lua::LevelState(), "enigma.settile", server::FallenPuzzle.substr(1), this)) {
                    throw XLevelRuntime(std::string("fallen puzzle set tile failed:\n")+lua::LastError(lua::LevelState()));
                }
            } else if (server::FallenPuzzle == "it_strip" || server::FallenPuzzle == "it_pipe") {
                std::string con = getAttr("connections").to_string();
                if (Item * it = GetItem(p)) {
                    if (it->getClass() == server::FallenPuzzle) {
                        if (server::FallenPuzzle == "it_strip")
                            con += it->getAttr("connections").to_string();
                        else  // pipe - keep old connections
                            con = it->getAttr("connections").to_string();                        
                    }
                }
                Item *it = MakeItem(server::FallenPuzzle.c_str());
                it->setAttr("connections", con);
                SetItem(p, it);
            } else if (server::FallenPuzzle.find("fl_") == 0) {
                SetFloor(p, MakeFloor(server::FallenPuzzle.c_str()));
            }
            KillStone(p);
        }
    }
    
    void PuzzleStone::alarm() {
        explodeStone();
    }

    void PuzzleStone::markPuzzle(bool visited) {
        if (visited != (bool)(objFlags & OBJBIT_VISITED))
            objFlags ^= OBJBIT_VISITED;
    }

    void PuzzleStone::markList(PuzzleList &list, bool visited) {
        for (PuzzleList::iterator itr = list.begin(); itr != list.end(); ++itr)
            (*itr)->markPuzzle(visited);
    }

    bool PuzzleStone::checkConnected(PuzzleList &todo, PuzzleStone * source, Direction dir, int color) {
        GridPos newpos = move(source->get_pos(), dir);
        PuzzleStone *pz = dynamic_cast<PuzzleStone*>(GetStone(newpos));

        if ((pz ==  NULL) || (pz->getAttr("color") != color))
            return false;

        DirectionBits cfaces = pz->getConnections();

        if (has_dir(cfaces, reverse(dir))) {
            // Puzzle stone at newpos is connected to stone at curpos
            if (!(pz->objFlags & OBJBIT_VISITED)) {
                pz->markPuzzle(true);
                todo.push_back(pz);
            }
            return true;
        } else {
            // The two stones are adjacent but not connected
            return false;
        }
    }

    bool PuzzleStone::findFragment(PuzzleList &cluster, bool keepMarked) {
        // Use a depth first search to determine the group of all stones that
        // are connected to the current stone.  Returns true if the cluster is
        // ``complete'' in the sense defined above.

        bool is_complete = true;
        PuzzleList todo;
        todo.push_back(this);
        markPuzzle(true);
        int color = getAttr("color");

        while (!todo.empty()) {
            PuzzleStone *pz = todo.back();
            todo.pop_back();

            cluster.push_back(pz);
            DirectionBits cfaces = pz->getConnections();

            if (has_dir(cfaces, NORTH))
                is_complete &= checkConnected(todo, pz, NORTH, color);
            if (has_dir(cfaces, EAST))
                is_complete &= checkConnected(todo, pz, EAST, color);
            if (has_dir(cfaces, SOUTH))
                is_complete &= checkConnected(todo, pz, SOUTH, color);
            if (has_dir(cfaces, WEST))
                is_complete &= checkConnected(todo, pz, WEST, color);
        }

        if (!keepMarked)
            markList(cluster, false);
        return is_complete;
    }

    void PuzzleStone::checkNeighbor(PuzzleList &todo, PuzzleStone *source, Direction dir, int color) {
        GridPos newpos = move(source->get_pos(), dir);
        if (PuzzleStone *pz = dynamic_cast<PuzzleStone*>(GetStone(newpos))) {
            if (!(pz->objFlags & OBJBIT_VISITED) && (pz->getAttr("color") == color)) {
                pz->markPuzzle(true);
                todo.push_back(pz);
            }
        }
    }

    void PuzzleStone::findAdjacents(PuzzleList &huddle) {
        PuzzleList todo;
        todo.push_back(this);
        markPuzzle(true);
        int color = getAttr("color");

        while (!todo.empty()) {
            PuzzleStone *pz = todo.back();
            todo.pop_back();

            huddle.push_back(pz);
            checkNeighbor(todo, pz, NORTH, color);
            checkNeighbor(todo, pz, SOUTH, color);
            checkNeighbor(todo, pz, EAST, color);
            checkNeighbor(todo, pz, WEST, color);
        }
        markList(huddle, false);
    }

    int PuzzleStone::findLine(PuzzleList &line, Direction dir, int color) {
        ASSERT(dir != NODIR, XLevelRuntime,
            "PuzzleStone: no direction in find_row_or_column_cluster");

        int size = 0;
        GridPos p = get_pos();
        while (PuzzleStone *puzz = dynamic_cast<PuzzleStone*>(GetStone(p))) {
            if (puzz->getAttr("color") != color)
                break; // stop when an unrequested puzzle stone type is readed
            line.push_back(puzz);
            p.move(dir);
            size += 1;
        }
        return size;
    }

    bool PuzzleStone::moveCluster(bool waterSink, Direction dir, bool checkOnly) {
        if (dir == NODIR) 
            return false;
        
        PuzzleList cluster;
        bool isComplete = findFragment(cluster, true);

        // Now check whether all stones can be placed at their new position
        bool move_ok = true;
        for (PuzzleList::iterator itr = cluster.begin(); itr != cluster.end(); ++itr) {
            Stone *st = GetStone(move((*itr)->get_pos(), dir));
            if (st != NULL) {
                // check if is the stone at destination a part of the cluster
                PuzzleStone *pz = dynamic_cast<PuzzleStone*>(st);
                if ((pz == NULL) || !(pz->objFlags & OBJBIT_VISITED)) {
                    move_ok = false;
                    break;
                }
            }
        }

        markList(cluster, false);
        if (checkOnly || !move_ok) {
            return move_ok;
        }

        // If the floor at a complete cluster's new position consists
        // exclusively of abyss or water, create a bridge instead of
        // moving the cluster.
        //
        // For partial clusters build bridges only on water and if the
        // wielded item is NOT the magic wand.

        bool createBridge = true;

        for (PuzzleList::iterator itr = cluster.begin(); itr != cluster.end(); ++itr) {
            if (Floor *fl = GetFloor(move((*itr)->get_pos(), dir))) {
                if (!((fl->getClass() == "fl_abyss" && isComplete) || (fl->getClass() == "fl_water" && (isComplete || waterSink)))) {
                    createBridge = false;
                    break;
                }
            }
        }

        // Finally, either move the whole cluster or create a bridge
        sound_event("movebig");
        int size = cluster.size();

        for (PuzzleList::iterator itr = cluster.begin(); itr != cluster.end(); ++itr) {
           (*itr)->sourcePos = (*itr)->get_pos();
           YieldStone((*itr)->sourcePos);
        }

        for (PuzzleList::iterator itr = cluster.begin(); itr != cluster.end(); ++itr) {
            SetStone(move((*itr)->sourcePos, dir), *itr);
            (*itr)->on_move((*itr)->sourcePos);
        }
        server::IncMoveCounter(size);

        if (createBridge) {
            for (PuzzleList::iterator itr = cluster.begin(); itr != cluster.end(); ++itr) {
                (*itr)->state = FALLING;
            }
        }
        return true;
    }

    bool PuzzleStone::explodeCluster() {
        bool exploded = false;

        PuzzleList cluster;
        if (findFragment(cluster)) {
            PuzzleList huddle;
            findAdjacents(huddle);

            // If all adjacent stones build one complete cluster explode it
//            Log << "Huddle " << huddle.size() << " Cluster "<< cluster.size() << "\n";
            if (huddle.size() == cluster.size()) {
                exploded = true;
            }
            else if ((server::GameCompatibility == GAMET_PEROXYD) && (getAttr("color") == BLUE)) {
                // check if 'huddle' is made up of complete clusters :

                while (!huddle.empty()) {
                    // remove one complete cluster from 'huddle'
                    markList(cluster, true);
                    for (PuzzleList::iterator itr = huddle.begin(); itr != huddle.end(); ) {
                        if ((*itr)->objFlags & OBJBIT_VISITED) {
                            (*itr)->objFlags &= ~OBJBIT_VISITED;
                            PuzzleList::iterator ditr = itr;
                            ++itr;
                            huddle.erase(ditr);
                        } else
                           ++itr;
                    }

                    if (huddle.empty()) { // none left -> all were complete
                        exploded = true;
                        break;
                    }

                    // look for next complete cluster :
                    cluster.clear();
                    PuzzleStone *pz = huddle.front();
                    if (!pz->findFragment(cluster)) {
                        break; // incomplete cluster found -> don't explode
                    }
                }
            }
        }

        if (exploded) {
            if (getAttr("color") == BLUE)
                findAdjacents(cluster);
            else
                findFragment(cluster);
            for (PuzzleList::iterator itr = cluster.begin(); itr != cluster.end(); ++itr)
                (*itr)->state = PENDING;
            explodeStone();
        } else if (getAttr("color") == YELLOW){
            if (cluster.size() == 1)
                objFlags |= OBJBIT_SINGLE;
            else
                objFlags &= ~OBJBIT_SINGLE;
        }

        return exploded;
    }

    void PuzzleStone::explodeStone() {
        GridPos p = get_pos();
        int color = getAttr("color");

        // exchange puzzle stone with explosion
        sound_event("stonedestroy");
        state = EXPLODING;
        init_model();

        // trigger all adjacent puzzle stones :
        const double DEFAULT_DELAY = 0.2;
        triggerExplosion(move(p, NORTH), DEFAULT_DELAY, color);
        triggerExplosion(move(p, SOUTH), DEFAULT_DELAY, color);
        triggerExplosion(move(p, EAST),  DEFAULT_DELAY, color);
        triggerExplosion(move(p, WEST),  DEFAULT_DELAY, color);
    }

    void PuzzleStone::triggerExplosion(GridPos p, double delay, int color) {
        PuzzleStone *puzz = dynamic_cast<PuzzleStone*>(GetStone(p));
        if (puzz && puzz->state == PENDING && puzz->getAttr("color") == color ) {
            // explode adjacent puzzle stones of same type
            puzz->ignite(delay);
        }
    }

    void PuzzleStone::ignite(double delay) {
        if (state == PENDING) {
            state = IGNITED;
            GameTimer.set_alarm(this, delay, false);
        }
    }

    void PuzzleStone::rotateLine(PuzzleList &line, bool forward) {
        PuzzleList::iterator itr = line.begin();
        PuzzleStone *pz1 = *itr;
        ++itr;
        if (itr != line.end()) {
            GridPos p1 = pz1->get_pos();
            YieldStone(p1);
            for (; itr != line.end(); ++itr) {
                if (forward) {
                    PuzzleStone *pz2 = *itr;
                    GridPos p2 = pz2->get_pos();
                    YieldStone(p2);
                    SetStone(p2, pz1);
                    pz1 = pz2;
                } else {
                    PuzzleStone *pz2 = *itr;
                    GridPos p2 = pz2->get_pos();
                    YieldStone(p2);
                    SetStone(p1, pz2);
                    p1 = p2;
                }
            }
            SetStone(p1, pz1);
        }
    }

    void PuzzleStone::rotateLine(Direction dir, bool forward) {
        if (dir != NODIR) {
            PuzzleList line;
            int size = findLine(line, dir, (int)getAttr("color"));
            if (size >= 2) {
                rotateLine(line, forward);
            }
        }
    }

    DEF_TRAITSM(PuzzleStone, "st_puzzle", st_puzzle, MOVABLE_IRREGULAR);

    BOOT_REGISTER_START
        BootRegister(new PuzzleStone(BLUE, ""), "st_puzzle");
        BootRegister(new PuzzleStone(BLUE, ""), "st_puzzle_blue");
        BootRegister(new PuzzleStone(YELLOW, ""), "st_puzzle_yellow");
        BootRegister(new PuzzleStone(BLUE, "w"),   "st_puzzle_blue_w");
        BootRegister(new PuzzleStone(BLUE, "s"),   "st_puzzle_blue_s");
        BootRegister(new PuzzleStone(BLUE, "sw"),  "st_puzzle_blue_sw");
        BootRegister(new PuzzleStone(BLUE, "e"),   "st_puzzle_blue_e");
        BootRegister(new PuzzleStone(BLUE, "ew"),  "st_puzzle_blue_ew");
        BootRegister(new PuzzleStone(BLUE, "es"),  "st_puzzle_blue_es");
        BootRegister(new PuzzleStone(BLUE, "esw"), "st_puzzle_blue_esw");
        BootRegister(new PuzzleStone(BLUE, "n"),   "st_puzzle_blue_n");
        BootRegister(new PuzzleStone(BLUE, "nw"),  "st_puzzle_blue_nw");
        BootRegister(new PuzzleStone(BLUE, "ns"),  "st_puzzle_blue_ns");
        BootRegister(new PuzzleStone(BLUE, "nsw"), "st_puzzle_blue_nsw");
        BootRegister(new PuzzleStone(BLUE, "ne"),  "st_puzzle_blue_ne");
        BootRegister(new PuzzleStone(BLUE, "new"), "st_puzzle_blue_new");
        BootRegister(new PuzzleStone(BLUE, "nes"), "st_puzzle_blue_nes");
        BootRegister(new PuzzleStone(BLUE, "nesw"), "st_puzzle_blue_nesw");
        BootRegister(new PuzzleStone(BLUE, "nesw", true), "st_puzzle_blue_nesw_hollow");
        BootRegister(new PuzzleStone(YELLOW, "w"),   "st_puzzle_yellow_w");
        BootRegister(new PuzzleStone(YELLOW, "s"),   "st_puzzle_yellow_s");
        BootRegister(new PuzzleStone(YELLOW, "sw"),  "st_puzzle_yellow_sw");
        BootRegister(new PuzzleStone(YELLOW, "e"),   "st_puzzle_yellow_e");
        BootRegister(new PuzzleStone(YELLOW, "ew"),  "st_puzzle_yellow_ew");
        BootRegister(new PuzzleStone(YELLOW, "es"),  "st_puzzle_yellow_es");
        BootRegister(new PuzzleStone(YELLOW, "esw"), "st_puzzle_yellow_esw");
        BootRegister(new PuzzleStone(YELLOW, "n"),   "st_puzzle_yellow_n");
        BootRegister(new PuzzleStone(YELLOW, "nw"),  "st_puzzle_yellow_nw");
        BootRegister(new PuzzleStone(YELLOW, "ns"),  "st_puzzle_yellow_ns");
        BootRegister(new PuzzleStone(YELLOW, "nsw"), "st_puzzle_yellow_nsw");
        BootRegister(new PuzzleStone(YELLOW, "ne"),  "st_puzzle_yellow_ne");
        BootRegister(new PuzzleStone(YELLOW, "new"), "st_puzzle_yellow_new");
        BootRegister(new PuzzleStone(YELLOW, "nes"), "st_puzzle_yellow_nes");
        BootRegister(new PuzzleStone(YELLOW, "nesw"), "st_puzzle_yellow_nesw");
        BootRegister(new PuzzleStone(YELLOW, "nesw", true), "st_puzzle_yellow_nesw_hollow");
    BOOT_REGISTER_END

} // namespace enigma

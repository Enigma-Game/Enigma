/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#include "server.hh"
#include "player.hh"
#include "client.hh"
#include "main.hh"
#include "Inventory.hh"
#include "items/GlassesItem.hh"

#include "stones_internal.hh"

using namespace std;

namespace enigma {


/* -------------------- DummyStone -------------------- */

namespace
{
    /*! Used for debugging; Prints its own oxyd code when hit. */
    class DummyStone : public Stone {
        CLONEOBJ(DummyStone);
    public:
        DummyStone() : Stone("st-dummy") {}
    private:

        StoneResponse collision_response(const StoneContact &/*sc*/) {
            static int lastCode = -1;
            int        code     = getAttr("code");
            if (code != lastCode) {
                fprintf(stderr, "Collision with stone 0x%02x\n", code);
                lastCode = code;
            }
            return STONE_REBOUND;
        }
    };
}


/* -------------------- EasyModeStone -------------------- */

/** \page st-easymode Easy-Mode Stone

In easy game mode this stone converts the floor at its
position to fl-normal.
In normal game mode the stone kills any item at its position.
Then in both modes it kills itself.

E.g. it can be used to hide water-barriers or to insert helper
items that make the level easier in easy game mode.

\subsection easye Example
\verbatim
set_stone("st-easymode", 10,10)
\endverbatim

\ref it-easymode
*/

namespace
{
    class EasyModeStone : public Stone {
        CLONEOBJ(EasyModeStone);
        DECL_TRAITS;

        virtual Value message(const Message &m) {
            if (m.message == "_init") {
                if (server::GetDifficulty() == DIFFICULTY_EASY) {
                    SetFloor (get_pos(), MakeFloor ("fl_metal_7n"));
                } else {
                    KillItem (get_pos());
                }
                KillStone (get_pos());
                return Value();
            }
            return Stone::message(m);
        }
    public:
        EasyModeStone() 
        {}
    };
    DEF_TRAITSM(EasyModeStone, "st-easymode", st_easymode, MOVABLE_BREAKABLE);
}


/* -------------------- Grates -------------------- */

namespace
{
    class GrateBase : public Stone {
    public:
        GrateBase(const char *kind) : Stone(kind) {}
    private:
        bool is_floating() const { return true; }
        bool is_transparent (Direction) const { return true; }

        virtual StoneResponse collision_response(const StoneContact &sc) {
            if (server::GameCompatibility == GAMET_OXYD1) {
                return STONE_PASS;
            }
            // tested with per.oxyd
            return sc.actor->is_on_floor() ? STONE_PASS : STONE_REBOUND;
        }
    };

    /*! Horses and small marbles can move through this stone, but
      normal marbles can't. */
    class Grate3 : public GrateBase {
        CLONEOBJ(Grate3);
    public:
        Grate3() : GrateBase("st-grate3") {}

        StoneResponse collision_response(const StoneContact &sc) {
            ActorID id = get_id(sc.actor);
            if (id == ac_horse || id == ac_pearl_white || id == ac_killer)
                return STONE_PASS;
            else
                return STONE_REBOUND;
        }
    };
}


/* -------------------- Chameleon Stone -------------------- */

namespace
{
    class ChameleonStone : public Stone {
        CLONEOBJ(ChameleonStone);
        DECL_TRAITS;

        Value message(const Message &m) {
            if (m.message == "_model_reanimated") {
                init_model();
            }
            return Stone::message(m);
        }
        
        void init_model() {
            if (Floor *fl = GetFloor(get_pos())) {
                set_model(fl->get_kind());
            } else
                ASSERT(false, XLevelRuntime, "Chameleon Stone: Floor must be set before stone");
        }

        bool is_floating() const 
        { return true; }

        StoneResponse collision_response(const StoneContact &)
        { return STONE_PASS; }

    public:
        ChameleonStone() 
        {}
    };
    DEF_TRAITS(ChameleonStone, "st-chameleon", st_chameleon);
}

/* -------------------- BombStone -------------------- */

namespace
{
    /*! This stone add a bomb to the player's inventory when touched. */
    class BombStone : public Stone {
        CLONEOBJ(BombStone);
        DECL_TRAITS;
        const char *collision_sound() {return "stone";}
    public:
        BombStone(const char* kind_, const char* itemkind_) :
            Stone(kind_), state(IDLE), itemkind(itemkind_) {}
    private:
        enum State { IDLE, BREAK };
        State state;
        const char* itemkind;
        void actor_hit (const StoneContact &sc);
        void change_state (State newstate);
        void animcb();
        virtual Value message(const Message &m);
    };
}
DEF_TRAITSM(BombStone, "INVALID", st_INVALID, MOVABLE_BREAKABLE);
    
void BombStone::change_state (State newstate) 
{
    if (state == IDLE && newstate==BREAK) {
        string model = get_kind();
        state = newstate;
        sound_event ("stonedestroy");
        set_anim(model + "-anim");
    }
}

void BombStone::animcb() 
{
    ASSERT(state == BREAK, XLevelRuntime, "BombStone: animcb called with inconsistent state");
    GridPos p = get_pos();
    SendExplosionEffect(p, EXPLOSION_BOMBSTONE);
    KillStone(p);
    if(Item *it = GetItem(get_pos())) {
        SendMessage(it, "ignite");
    } else
        SetItem(p, MakeItem("it-explosion1"));
}

Value BombStone::message(const Message &m) 
{
    if (m.message =="_explosion" || m.message =="_bombstone") {
        change_state(BREAK);
        return Value();
    }
    return Stone::message(m);
}

void BombStone::actor_hit(const StoneContact &sc) 
{
    if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
        if (!inv->is_full()) {
            Item *it = MakeItem(itemkind);
            inv->add_item(it);
            player::RedrawInventory (inv);
        }
    }
}


/* -------------------- MagicStone -------------------- */
namespace
{
    class MagicStone : public Stone {
        CLONEOBJ(MagicStone);
        DECL_TRAITS;
        void actor_hit(const StoneContact &sc) {
            if (sc.actor->getAttr("owner") && 
                sc.actor->get_vel() * sc.normal < -4)
            {
                KillStone(get_pos());
                client::Msg_ShowText ("We don't sell books..", false, 2.0);
            }
        }
    public:
        MagicStone()
        {}
    };
    DEF_TRAITSM(MagicStone, "st-magic", st_magic, MOVABLE_BREAKABLE);
}


/* -------------------- Functions -------------------- */

void Init_simple()
{
    Register(new BombStone("st-bombs", "it-blackbomb"));
    //Register(new BombStone("st-dynamite", "it-dynamite"));
    //Register(new BombStone("st-whitebombs", "it-whitebomb"));

    Register(new ChameleonStone);

    Register(new DummyStone);
    Register(new EasyModeStone);
    Register(new Grate3);
    Register(new MagicStone);
}

} // namespace enigma

/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008 Ronald Lamprecht
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
#include "main.hh"
#include "display.hh"
#include "player.hh"
#include "client.hh"
#include "SoundEffectManager.hh"
#include "server.hh"
#include "world.hh"
#include "Inventory.hh"
#include "ItemHolder.hh"
#include "lev/Proxy.hh"

#include "ecl_util.hh"

#include <algorithm>
#include <cmath>
#include <iostream>


using namespace std;

using enigma::GridPos;
using enigma::Value;
using enigma::DoubleRand;
using ecl::V2;


/* -------------------- Macros -------------------- */

#define DEF_ITEM(classname, kindname, kindid)   \
    class classname : public Item {             \
        CLONEOBJ(classname);                    \
        DECL_TRAITS;                            \
    public:                                     \
        classname() {}                          \
    };                                          \
    DEF_TRAITS(classname, kindname, kindid)

#define DEF_ITEMF(classname, kindname, kindid, flags)   \
    class classname : public Item {             \
        CLONEOBJ(classname);                    \
        DECL_TRAITS;                            \
    public:                                     \
        classname() {}                          \
    };                                          \
    DEF_TRAITSF(classname, kindname, kindid, flags)

#define DECL_TRAITS                                             \
        static ItemTraits traits;                               \
        const ItemTraits &get_traits() const { return traits; }

#define DECL_TRAITS_ARRAY(n, subtype_expr)                                      \
        static ItemTraits traits[n];                                            \
        const ItemTraits &get_traits() const { return traits[subtype_expr]; }

#define DEF_TRAITS(classname, name, id)         \
    ItemTraits classname::traits = { name, id, itf_none, 0.0 }

#define DEF_TRAITSF(classname, name, id, flags)         \
    ItemTraits classname::traits = { name, id, flags, 0.0 }

#define DEF_TRAITSR(classname, name, id, radius)         \
    ItemTraits classname::traits = { name, id, 0, radius }


namespace enigma {

/* -------------------- Item implementation -------------------- */

Item::Item()
{}

void Item::kill() {
    KillItem(get_pos());
}

void Item::replace(ItemID id)
{
    Item *newitem = MakeItem (id);
    transferName(newitem);          // TODO check where transfer of identity is better
    setup_successor(newitem);           // hook for subclasses
    SetItem (get_pos(), newitem);
}

void Item::transform(std::string kind) {
    Item *newitem = MakeItem(kind.c_str());
    transferIdentity(newitem);          // subclasses may hook this call
    SetItem(get_pos(), newitem);
}

const char *Item::get_kind() const
{
    return get_traits().name;
}

string Item::get_inventory_model()
{
    return get_kind();
}

void Item::init_model()
{
    const ItemTraits &tr = get_traits();
    if (tr.flags & itf_invisible)
        set_model("invisible");
    else if (tr.flags & itf_animation)
        set_anim(tr.name);
    else
        set_model(tr.name);
}

void Item::stone_change (Stone * /*st*/) {
}

void Item::on_stonehit (Stone * /*st*/) {
}

void Item::processLight(Direction d) {
    if (get_traits().flags & itf_inflammable) {
        replace (it_explosion1);
    } else
        GridObject::processLight(d);
}


void Item::on_drop (Actor * /*a*/) {
}

void Item::drop (Actor *a, GridPos p) {
    SetItem (p, this);
    on_drop(a);
}


void Item::on_pickup (Actor * /*a*/) {
}

bool Item::can_drop_at (GridPos p) {
    return GetItem(p) == 0;
}

ItemAction Item::activate(Actor* /*a*/, GridPos /*p*/) {
    return ITEM_DROP;
}

bool Item::isStatic() const {
    return get_traits().flags & itf_static;
}

void Item::add_force(Actor *, V2 &) {
}

bool Item::actor_hit(Actor *actor)
{
    const ItemTraits &tr = get_traits();
    if (tr.flags & itf_static)
        return false;
    else {
        double radius = 0.3;
        if (tr.radius != 0.0)
            radius = tr.radius;
        return length(actor->get_pos()-get_pos().center()) < radius;
    }
}

/* -------------------- Various simple items -------------------- */

namespace
{
    DEF_ITEM(MagicWand, "it-magicwand", it_magicwand);
    DEF_ITEM(Floppy,    "it_floppy", it_floppy);
    DEF_ITEM(Key,       "it_key", it_key);
    DEF_ITEM(Odometer,  "it-odometer", it_odometer);
    DEF_ITEM(Wrench,    "it_wrench", it_wrench);
    DEF_ITEM(BrokenGlasses, "it-glasses-broken", it_glasses_broken);
    DEF_ITEMF(Coffee,   "it-coffee", it_coffee, itf_inflammable);
}


/* -------------------- DummyItem -------------------- */
    class Dummyitem : public Item {
        CLONEOBJ(Dummyitem);
        DECL_TRAITS;

        void on_pickup(Actor *) {
            int code = getAttr("code");
            fprintf(stderr, "Picked up item 0x%x\n", code);
        }
        void on_drop(Actor *) {
            int code = getAttr("code");
            fprintf(stderr, "Dropped item 0x%x\n", code);
        }
    public:
        Dummyitem() {}
    };
    DEF_TRAITSF(Dummyitem, "it-dummy", it_dummy, itf_fireproof);

/* -------------------- Cherry -------------------- */

    class Cherry : public Item {
        CLONEOBJ(Cherry);
        DECL_TRAITS;
        ItemAction activate(Actor *actor, GridPos) {
            SendMessage (actor, "invisibility");
            return ITEM_KILL;
        }

        void on_stonehit(Stone *) {
            replace(it_squashed);
        }
    public:
        Cherry() {
        }
    };
    DEF_TRAITS(Cherry, "it-cherry", it_cherry);

/* -------------------- Squashed Cherry -------------------- */

    class Squashed : public Item {
        CLONEOBJ(Squashed);
        DECL_TRAITS;

        virtual Value message (const Message &m) {
            if (enigma_server::GameCompatibility == GAMET_ENIGMA) {
                if (m.message == "_brush") {
                    KillItem(this->get_pos());
                    return Value();
                }
            }
            return Item::message(m);
        }


    public:
        Squashed() {
        }
    };
    DEF_TRAITSF(Squashed, "it-squashed", it_squashed, itf_static);


/* -------------------- Weight -------------------- */

    class Weight : public Item {
        CLONEOBJ(Weight);
        DECL_TRAITS;

        void on_pickup(Actor *a) {
           ActorInfo *ai = a->get_actorinfo();
           ai->mass += 10.0;
        }
        ItemAction activate(Actor *, GridPos) {
            return ITEM_KEEP;
        }
    public:
	Weight() {}
    };
    DEF_TRAITS(Weight, "it-weight", it_weight);

/* -------------------- Pin -------------------- */

    class Pin : public Item {
        CLONEOBJ(Pin);
        DECL_TRAITS;

        void on_pickup(Actor *a) {
            a->set_spikes(true);
        }
        void on_drop(Actor *a) {
            a->set_spikes(false);
        }
    public:
        Pin() {}
    };
    DEF_TRAITS(Pin, "it-pin", it_pin);

/* -------------------- Banana -------------------- */

    class Banana : public Item {
        CLONEOBJ(Banana);
        DECL_TRAITS;

        void processLight(Direction d) {
            sound_event ("itemtransform");
            replace(it_cherry);
        }

        void on_stonehit(Stone *) {
            replace(it_squashed);
        }

    public:
        Banana() {}
    };
    DEF_TRAITS(Banana, "it-banana", it_banana);

/* -------------------- Sword -------------------- */

    class Sword : public Item, public TimeHandler {
        CLONEOBJ(Sword);
        DECL_TRAITS;

    public:
        Sword(bool isNew);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);

        // TimeHandler interface
        virtual void alarm();
    };

    Sword::Sword(bool isNew) : Item() {
        if (isNew) {
            objFlags |= ALL_DIRECTIONS;
        }
    }

    void Sword::on_creation(GridPos p) {
        if ((objFlags & ALL_DIRECTIONS) == ALL_DIRECTIONS) {
            // a new transformed hammer
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            updateCurrentLightDirs();
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void Sword::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        objFlags &= ~ALL_DIRECTIONS;
        Item::on_removal(p);
    }

    
    void Sword::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0) {
            sound_event ("itemtransform");
            transform("it_hammer_new");
        }
    }
    
    void Sword::alarm() {
            updateCurrentLightDirs();
            activatePhoto();        
    }
    
    DEF_TRAITS(Sword, "it_sword", it_sword);

/* -------------------- Hammer -------------------- */

    class Hammer : public Item, public TimeHandler {
        CLONEOBJ(Hammer);
        DECL_TRAITS;

    public:
        Hammer(bool isNew);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);

        // TimeHandler interface
        virtual void alarm();
    };
    
    Hammer::Hammer(bool isNew) : Item() {
        if (isNew) {
            objFlags |= ALL_DIRECTIONS;
        }
    }
    
    void Hammer::on_creation(GridPos p) {
        if ((objFlags & ALL_DIRECTIONS) == ALL_DIRECTIONS) {
            // a new transformed hammer
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            updateCurrentLightDirs();
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void Hammer::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        objFlags &= ~ALL_DIRECTIONS;
        Item::on_removal(p);
    }
    
    void Hammer::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0 && server::GameCompatibility != enigma::GAMET_PEROXYD) {
            sound_event ("itemtransform");
            transform("it_sword_new");
        }
    }
    
    void Hammer::alarm() {
            DirectionBits db = updateCurrentLightDirs();
            activatePhoto();        
    }
    
    DEF_TRAITS(Hammer, "it_hammer", it_hammer);


/* -------------------- ExtraLife -------------------- */

    class ExtraLife : public Item, public TimeHandler {
        CLONEOBJ(ExtraLife);
        DECL_TRAITS;
        
    public:
        ExtraLife(bool isNew);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // Item interface
        virtual string get_inventory_model();

        // TimeHandler interface
        virtual void alarm();
    };

    ExtraLife::ExtraLife(bool isNew) : Item() {
        if (isNew) {
            objFlags |= ALL_DIRECTIONS;
        }
    }
    
    std::string ExtraLife::get_inventory_model() {
        if (player::CurrentPlayer()==0)
            return "inv-blackball";
        else
            return "inv-whiteball";
    }

    void ExtraLife::on_creation(GridPos p) {
        if ((objFlags & ALL_DIRECTIONS) == ALL_DIRECTIONS) {
            // a new transformed extralife
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            updateCurrentLightDirs();
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void ExtraLife::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        objFlags &= ~ALL_DIRECTIONS;
        Item::on_removal(p);
    }
    
    void ExtraLife::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0) {
            sound_event ("itemtransform");
            replace(it_glasses);
        }
    }
    
    void ExtraLife::alarm() {
            DirectionBits db = updateCurrentLightDirs();
            activatePhoto();        
    }

    DEF_TRAITS(ExtraLife, "it_extralife", it_extralife);

/* -------------------- Umbrella -------------------- */

    class Umbrella : public Item, public TimeHandler {
        CLONEOBJ(Umbrella);
        DECL_TRAITS;

    public:
        Umbrella(bool isNew);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);

        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
        
        // TimeHandler interface
        virtual void alarm();
    };

    Umbrella::Umbrella(bool isNew) : Item() {
        if (isNew) {
            objFlags |= ALL_DIRECTIONS;
        }
    }
    
    void Umbrella::on_creation(GridPos p) {
        if ((objFlags & ALL_DIRECTIONS) == ALL_DIRECTIONS) {
            // a new transformed umbrella
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            updateCurrentLightDirs();
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void Umbrella::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        objFlags &= ~ALL_DIRECTIONS;
        Item::on_removal(p);
    }
    
    void Umbrella::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0 && server::GameCompatibility != enigma::GAMET_PEROXYD) {
            sound_event ("itemtransform");
            replace(it_explosion1);
        }
    }
    
    ItemAction Umbrella::activate(Actor *a, GridPos) {
        SendMessage(a, "shield");
        return ITEM_KILL;
    }
    
    void Umbrella::alarm() {
            DirectionBits db = updateCurrentLightDirs();
            activatePhoto();        
    }

    DEF_TRAITS (Umbrella, "it_umbrella", it_umbrella);

/* -------------------- Spoon -------------------- */
namespace
{
    class Spoon : public Item {
        CLONEOBJ(Spoon);
        DECL_TRAITS;

        ItemAction activate(Actor *a, GridPos) {
            SendMessage(a, "suicide");
            return ITEM_DROP;
        }
    public:
        Spoon()
        {}
    };
    DEF_TRAITS(Spoon, "it-spoon", it_spoon);
}

/* -------------------- Booze -------------------- */

namespace
{
    class Booze : public Item {
	CLONEOBJ(Booze);
        DECL_TRAITS;
    public:
	Booze() {
	}
    private:
	ItemAction activate(Actor *a, GridPos) {
	    SendMessage(a, "booze");
	    return ITEM_DROP;
	}
    void on_stonehit(Stone *) {
        sound_event("shatter");
        replace(it_booze_broken);
    }
    };
    DEF_TRAITS(Booze, "it-booze", it_booze);
}

/* -------------------- Broken Booze -------------------- */
namespace
{
    class BrokenBooze : public Item {
        CLONEOBJ(BrokenBooze);
        DECL_TRAITS;

        bool actor_hit(Actor *a) {
            ActorInfo &ai = * a->get_actorinfo();
            if (!ai.grabbed && a->is_on_floor()) {
                SendMessage(a, "shatter");
            }
            return false;
        }

        virtual Value message (const Message &m) {
            if (enigma_server::GameCompatibility == GAMET_ENIGMA) {
                if (m.message == "_brush") {
                    KillItem(this->get_pos());
                    return Value();
                }
            }
            return Item::message(m);
        }

    public:
        BrokenBooze() {}
    };

    DEF_TRAITSF(BrokenBooze, "it-booze-broken", it_booze_broken, itf_static | itf_indestructible);
}

/* -------------------- Brush -------------------- */
    /* Can "paint" some stones and remove ash. */
    class Brush : public Item {
        CLONEOBJ(Brush);
        DECL_TRAITS;

    public:
        Brush();
        
        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };
    
    Brush::Brush() {
    }
    
    ItemAction Brush::activate(Actor *a, GridPos p) {
        if (Item *it = GetItem(p))
            SendMessage (it, "_brush");
        return ITEM_DROP;
    }
    
    DEF_TRAITSF(Brush, "it_brush", it_brush, itf_inflammable);



/* -------------------- Coins -------------------- */

// TODO id renaming when names are stable

    class Coin : public Item {
        CLONEOBJ(Coin);
        DECL_TRAITS_ARRAY(3, state);

    public:
        Coin(int type);
        static void setup();
        
        // Object interface
        virtual std::string getClass() const;

        // GridObject interface
        virtual void processLight(Direction d);
        
        // Item interface
        virtual void on_stonehit(Stone *st);
    };
    
    void Coin::setup() {
        RegisterItem (new Coin(0));
        RegisterItem (new Coin(1));
        RegisterItem (new Coin(2));
    }

    Coin::Coin(int type) {
        state = type;
        setAttr("coin_value", state == 0 ? 3.0 : (state == 1 ? 6.0 : 12.0));
    }
    
    std::string Coin::getClass() const {
        return "Coin";
    }
    
    void Coin::processLight(Direction d) {
        sound_event("itemtransform");
        switch (state) {
            case 0 : 
                transform("it_umbrella_new"); break;
            case 1 :
                transform("it_hammer_new"); break;
            case 2 :
                transform("it_extralife_new"); break;
        }
    }

    void Coin::on_stonehit(Stone *) {
        if (state <= 1) {
            state += 1;
            setAttr("coin_value", (state == 1 ? 6.0 : 12.0));  // API 1 compatibility
            init_model();
        }

    }
    
    ItemTraits Coin::traits[3] = {
        {"it_coin_s",  it_coin1,  itf_none, 0.0},
        {"it_coin_m",  it_coin2,  itf_none, 0.0},
        {"it_coin_l",  it_coin4,  itf_none, 0.0},
    };
    


/* -------------------- Hills and Hollows -------------------- */

/** \page it-hills Hills and Hollows

Hills and hollows are placed on the floor and can
make the movement difficult.

\subsection hillsm Messages
- \b trigger	will convert a hill to a hollow and vice versa
- \b shovel     decreases the size of the hill/hollow

\image html it-hill.png
*/
namespace
{
    class HillHollow : public Item {
    public:
        // Object interface.
        virtual Value message(const Message &m);
    protected:
        enum Type { HILL, HOLLOW, TINYHILL, TINYHOLLOW };

        HillHollow(Type t);

        void transmute(Type newtype);
        V2 vec_to_center (V2 v);
        double get_radius() const { return m_radius[m_type]; }

        Type get_type() const { return m_type; }

    private:
        double get_forcefac() const {
            return m_forcefac[m_type] * server::HoleForce;
        }
        void shovel();

        // Item interface
        void add_force(Actor *a, V2 &f);
        void on_stonehit(Stone *st);


        // Variables.
        static double m_radius[4], m_forcefac[4];
        Type m_type;
    };

    class Hill : public HillHollow {
        CLONEOBJ(Hill);
        DECL_TRAITS;
    public:
        Hill() : HillHollow(HILL) {}
    };
    DEF_TRAITSF(Hill, "it-hill", it_hill, itf_static | itf_fireproof);

    class TinyHill : public HillHollow {
        CLONEOBJ(TinyHill);
        DECL_TRAITS;
    public:
        TinyHill() : HillHollow(TINYHILL) {}
    };
    DEF_TRAITSF(TinyHill, "it-tinyhill", it_tinyhill, itf_static | itf_fireproof);

    /*
     * Hollows are special in that they can end the current level
     * if they have each a small white marble inside them.
     */
    class Hollow : public HillHollow {
        CLONEOBJ(Hollow);
        DECL_TRAITS;
    public:
        Hollow(Type t = HOLLOW);
        // Object interface.
        virtual Value message(const Message &m);
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
    protected:
        virtual void setup_successor(Item *newitem);
    private:
        // Item interface.
        bool actor_hit(Actor *a);
        void actor_leave(Actor *a);

        // Functions.
        bool near_center_p (Actor *a);

        // Variables.
        Actor *whiteball;   // The small white ball that is currently being tracked
        double enter_time;  // ... when did it enter the hollow?
    };
    DEF_TRAITSF(Hollow, "it-hollow", it_hollow, itf_static | itf_fireproof);


    class TinyHollow : public Hollow {
        CLONEOBJ(TinyHollow);
        DECL_TRAITS;
    public:
        TinyHollow() : Hollow(TINYHOLLOW) {}
    };
    DEF_TRAITSF(TinyHollow, "it-tinyhollow", it_tinyhollow, itf_static | itf_fireproof);

}


/* ---------- HillHollow ---------- */

double HillHollow::m_radius[4] = {0.5, 0.5, 0.3, 0.3};
double HillHollow::m_forcefac[4] = {90,-90, 90, -90};


HillHollow::HillHollow (Type t)
: m_type(t)
{}

void HillHollow::on_stonehit(Stone *)
{
    shovel();
}

void HillHollow::shovel() {
    switch (get_id (this)) {
    case it_hollow: transmute (TINYHOLLOW); break;
    case it_hill:   transmute (TINYHILL); break;
    default:        kill(); break;
    }
}

Value HillHollow::message(const Message &m)
{
    if (m.message == "flip") {
        Type flippedkind[] = {HOLLOW,HILL, TINYHOLLOW,TINYHILL};
        transmute(flippedkind[m_type]);
        return Value();
    } else if (m.message == "signal") {
        if (m.value != 0) {
            Type flippedkind[] = {HILL,HILL, TINYHILL,TINYHILL};
            transmute(flippedkind[m_type]);
        } else {
            Type flippedkind[] = {HOLLOW,HOLLOW, TINYHOLLOW,TINYHOLLOW};
            transmute(flippedkind[m_type]);
        }
        return Value();
    } else if (m.message == "shovel") {
        shovel();
        return Value();
    } 
    return Item::message(m);
}

V2 HillHollow::vec_to_center (V2 v)
{
    return v-get_pos().center();
}

void HillHollow::add_force(Actor *a, V2 &f)
{
    V2     v    = vec_to_center(a->get_pos());
    double dist = length(v);

    if (dist > get_radius())
        return;

    if (dist <= 0) { // exactly on hill-top
        ActorInfo *ai = a->get_actorinfo();
        if (length(ai->vel) <= 0) { // no velocity
            // we are never "exactly" on the top!
            v = ecl::V2(DoubleRand(0.01, 0.05), DoubleRand(0.01, 0.05));
        }
    }

    f += get_forcefac()*v; // get the force
}

void HillHollow::transmute(Type newtype)
{
    static ItemID newmodel[] = { it_hill, it_hollow, it_tinyhill, it_tinyhollow };
    replace (newmodel[newtype]);
}


/* ---------- Hollow ---------- */

// TODO handle set of essential attribute

Hollow::Hollow(Type t) : HillHollow(t), whiteball(0), enter_time (-1) {
}

Value Hollow::message(const Message &m) {
    Log << "Hollow - init time " << server::LevelTime << "\n";
    if (m.message == "_init") {
        vector<Actor*> actors;
        GetActorsInsideField(get_pos(), actors);
        for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
            Log << "Hollow - initial meditatist  typ " << get_id(*it) << "\n";
            if (!(*it)->is_flying() &&  whiteball==NULL && get_id(*it)==ac_meditation && near_center_p(*it)) {
                 // meditatist entered a free hollow
                whiteball  = *it;
                enter_time = server::LevelTime;
                Log << "Hollow - initial meditatist time " << enter_time << "\n";
                break;
            }
        }
        return Value();
    }
    return HillHollow::message(m);
}

void Hollow::on_creation(GridPos p) {
    if (getDefaultedAttr("essential", 0) == 1)
        ChangeMeditation(0, +1, 0, 0);
    HillHollow::on_creation(p);
}

void Hollow::on_removal(GridPos p) {   //TODO a change from Hollow to TinyHollow is critical -> single class with state reengineering
    if (getDefaultedAttr("essential", 0) == 1)
        ChangeMeditation(0, -1, 0, 0);
    HillHollow::on_removal(p);
}

bool Hollow::near_center_p (Actor *a)
{
    return (length(vec_to_center(a->get_pos())) < get_radius()*0.8);
}

bool Hollow::actor_hit(Actor *a)
{
    const double MINTIME = 1.0;
    ItemID id = get_id (this);

    if (id == it_hollow || id == it_tinyhollow) {
        if (whiteball==NULL && get_id(a)==ac_meditation && near_center_p(a)) {
            // meditatist entered a free hollow
            whiteball  = a;
            enter_time = server::LevelTime;
        } else if (whiteball == a) {
            if (!near_center_p(a)) {
                // meditatist left hollow
                whiteball = NULL;
                if (enter_time == -1) {   // meditatist is registered
                    bool indispensable = (getDefaultedAttr("essential", 0) == 1);
                    ChangeMeditation(0, 0, indispensable ? -1 : 0, indispensable ? 0 : -1);
                }
            } else  if (enter_time != -1 && (server::LevelTime - enter_time) >= MINTIME) {
                    // just meditated enough to mark hollow as engaged
                    bool indispensable = (getDefaultedAttr("essential", 0) == 1);
                    ChangeMeditation(0, 0, indispensable ? +1 : 0, indispensable ? 0 : +1);
                    enter_time = -1;  // mark as registered
            }
        }
    }

    return false;
}

void Hollow::actor_leave(Actor *a) {
    if (whiteball == a) {
        // meditatist left hollow (warp, ...)
        whiteball = NULL;
        if (enter_time == -1) {   // meditatist is registered
            bool indispensable = (getDefaultedAttr("essential", 0) == 1);
            ChangeMeditation(0, 0, indispensable ? -1 : 0, indispensable ? 0 : -1);
        }
    }
}


void Hollow::setup_successor(Item *newitem) {
    newitem->setAttr("essential", getAttr("essential"));
}


/* -------------------- Springs -------------------- */

/** \page it-spring Spring

Activating a spring will make the marble jump.
A jumping marble does not fall into abyss or water.

Springs come in two flavors: it-spring1 stays in the inventory,
whereas it-spring2 drops to the floor when you activate it.

\image html it-spring1.png
*/
namespace
{
    class Spring1 : public Item {
        CLONEOBJ(Spring1);
        DECL_TRAITS;
    public:
        Spring1() {}
    private:
        ItemAction activate(Actor *a, GridPos)
        {
            SendMessage(a, "jump");
            return ITEM_KEEP;
        }
    };
    DEF_TRAITS(Spring1, "it-spring1", it_spring1);

    class Spring2 : public Item {
        CLONEOBJ(Spring2);
        DECL_TRAITS;
    public:
        Spring2() {}
    private:
        ItemAction activate(Actor *a, GridPos p)
        {
            Item *it = GetItem(p);
            if (!it || has_flags(it, itf_static)) {
                SendMessage(a, "jump");
                return ITEM_DROP;  // drop if grid has no item
            } else {
                // don't jump if a regular item is on the grid
                return ITEM_KEEP;
            }
        }
    };
    DEF_TRAITS(Spring2, "it-spring2", it_spring2);
}


/* -------------------- Springboard -------------------- */
namespace
{
    class Springboard : public Item {
        CLONEOBJ(Springboard);
        DECL_TRAITS;

        bool actor_hit(Actor *a) {
            const double ITEM_RADIUS = 0.3;
            ecl::V2 item_center(get_pos().x + 0.5, get_pos().y + 0.5);
            double dist = length(a->get_pos() - item_center);
            if (dist < ITEM_RADIUS) {
                set_anim("it-springboard_anim");
                SendMessage(a, "jump");
            }
            return false;
        }

        void animcb() {
            set_model("it-springboard");
        }
    public:
        Springboard() {}
    };
    DEF_TRAITSF(Springboard, "it-springboard", it_springboard, itf_static);
}


/* -------------------- Brake -------------------- */

// Brake is only used to hold st-brake while it is in an inventory

// TODO transfer identity !!
namespace
{
    class Brake : public Item {
        CLONEOBJ(Brake);
        DECL_TRAITS;
    public:
        Brake() {}

        void on_creation (GridPos p) {
            SetStone(p, MakeStone("st-brake"));
            kill();
        }

        bool can_drop_at (GridPos p) {
            return GetStone(p) == 0;
        }

        void drop (Actor *, GridPos p) {
            SetStone(p, MakeStone("st-brake"));
            kill();
        }

        string get_inventory_model() {
            return "st-brake";
        }

        ItemAction activate(Actor *, GridPos) {
            return ITEM_DROP;
        }
    };
    DEF_TRAITS(Brake, "it-brake", it_brake);
}


/* -------------------- Explosion -------------------- */
namespace
{
    class Explosion : public Item {
    public:
        Explosion ()
        {}

    private:
        void init_model() {set_anim("expl");}
        bool actor_hit(Actor *actor) {
            SendMessage(actor, "shatter");
            return false;
        }
    };

    // Explode but do nothing else.
    class Explosion1 : public Explosion {
        CLONEOBJ(Explosion1);
        DECL_TRAITS;

        void animcb() {
            kill();
        }
    public:
        Explosion1()
        {}
    };
    DEF_TRAITSF(Explosion1, "it-explosion1", it_explosion1, itf_static |
                itf_animation | itf_indestructible | itf_norespawn | itf_fireproof);

    // Explode and leave a hole in the ground.
    class Explosion2 : public Explosion {
        CLONEOBJ(Explosion2);
        DECL_TRAITS;

        void animcb() {
            if (Floor *fl = GetFloor(get_pos()))
                if (fl->is_destructible())
                    replace(it_hollow);
                else
                    kill();
        }
    public:
        Explosion2()
        {}
    };
    DEF_TRAITSF(Explosion2, "it-explosion2", it_explosion2, itf_static |
                itf_animation | itf_indestructible | itf_norespawn | itf_fireproof);


    // Explode and shatter the floor.
    class Explosion3 : public Explosion {
        CLONEOBJ(Explosion3);
        DECL_TRAITS;

        void animcb() {
            if (Floor *fl = GetFloor(get_pos()))
                if (fl->is_destructible())
                    replace(it_debris);
                else
                    kill();
        }
    public:
        Explosion3()
        {}
    };
    DEF_TRAITSF(Explosion3, "it-explosion3", it_explosion3, itf_static |
                itf_animation | itf_indestructible | itf_norespawn | itf_fireproof);
}




/* -------------------- Document -------------------- */
namespace
{
    class Document : public Item {
        CLONEOBJ(Document);
        DECL_TRAITS;

        ItemAction activate(Actor *, GridPos)
        {
            if (Value v = getAttr("text")) {
                std::string txt(v);
                lev::Proxy *level = lev::Proxy::loadedLevel();
                // after complete switch to Proxy as levelloader the following
                // conditional can be abolished
                if (level)
                    // translate text
                    txt = level->getLocalizedString(txt);
                client::Msg_ShowText (txt, true);
            }
            return ITEM_KILL;	       // remove from inventory
        }
        virtual Value message(const Message &m) {
            bool explode = false;

            if (m.message == "ignite") {
                // dynamite does not blow up Documents in Oxyd1
                explode = server::GameCompatibility != GAMET_OXYD1;
            } else if (m.message == "_explosion" || m.message == "_bombstone") {
                explode = true;
            } else {
                return Item::message(m);
            }

            if (explode)
                replace (it_explosion1);
            return Value();
        }
    public:
        Document() {
            setAttr("text", "");
        }
    };
    DEF_TRAITSF(Document, "it-document", it_document, itf_inflammable);
}


/* -------------------- Dynamite -------------------- */
namespace
{
    class Dynamite : public Item {
        CLONEOBJ(Dynamite);
        DECL_TRAITS;
    public:
        Dynamite() : state(IDLE) {}
    private:
        enum State { IDLE, BURNING };
        State state;

        void change_state(State newstate) {
            if (newstate==BURNING && state==IDLE) {
                state = BURNING;
                set_anim("it-dynamite-burning");
            }
        }

        virtual bool isStatic() const {
            return state == BURNING;  // burning dynamite cannot be picked up
        }

        void explode () {
            GridPos p = get_pos();
            SendExplosionEffect(p, EXPLOSION_DYNAMITE);
            sound_event ("dynamite");
            //SetItem(p, it_explosion2);
            Floor *fl = GetFloor(p);
            string model = fl->get_kind();
            // SetItem(p, it_explosion2) only used by it-dynamite?
            // If Yes, the following block could be places in the explosion class:
            if (model == "fl-space") {
                // In space, an it-dynamite explodes to an it-sherd:
                // HOT FIX
                //replace(it_sherd);
                replace(it_hollow);
            } else if (model == "fl-ice") {
                // In ice, an it-dynamite explodes to an it-crack2:
                replace(it_crack2);
            } else {
                SetItem(p, it_explosion2);
            }
        }

        virtual Value message(const Message &m) {
            if (m.message == "ignite" || m.message == "_explosion" || m.message == "_bombstone") {
                change_state(BURNING);
                return Value();
            } else if (m.message == "explode") { // currently unused in c++ code
                explode();
                return Value();
            } else if (m.message == "heat") {  // used by fire-system
                change_state(BURNING);
                return true;  // caught message -> no fire!
            }
            return Item::message(m);
        }
        void animcb() { explode(); }
        void processLight(Direction d) {
            change_state(BURNING);
        }
        void on_drop(Actor *) { change_state(BURNING); }
        bool actor_hit(Actor *a) {
            if (state == BURNING)
                return false;   // don't pick up burning dynamite

            return Item::actor_hit(a);
        }
    };
    DEF_TRAITSF(Dynamite, "it-dynamite", it_dynamite,
                itf_indestructible | itf_fireproof);
}

// ----------------------------
//      BombBase
// ----------------------------
// base class for BlackBomb and WhiteBomb

namespace
{
    class BombBase : public Item {
    public:
        BombBase (bool burning = false)
        : m_burning(burning)
        {}

    protected:
        virtual Value message(const Message &m) {
            if (m.message == "ignite"  || m.message == "_explosion") {
                burn();
                return Value();
            } else if (m.message == "explode" ) {
                explode();
                return Value();
            } else if (m.message == "heat") {  // used by fire-system
                burn();
                return true;  // caught message -> no fire!
            }
            return Item::message(m);
        }

    private:
        // Variables
        bool m_burning;

        // Private methods
        virtual void explode() = 0;

        void init_model() {
            if (m_burning)
                set_anim(burn_anim());
            else
                Item::init_model();
        }

        void burn() {
            if (!m_burning) {
                m_burning = true;
                init_model();
            }
        }

        void animcb() { explode (); }

        void processLight(Direction d) {
            explode();
        }

        void on_stonehit(Stone *st) {
            switch (server::GameCompatibility) {
            case GAMET_OXYD1:
            case GAMET_OXYDMAGNUM:
                if (!st->is_kind("st-wood?"))
                    // st-wood does not cause bombs to explode
                    explode();
                break;
            default :
                explode();
                break;
            }
        }

        virtual const char *burn_anim() const = 0;
    };
}

/* -------------------- Black Bomb -------------------- */

/** \page it-blackbomb Black Bomb

When black bombs explode, they destroy the floor tile underneath them.

\image html it-blackbomb.png
*/

namespace
{
    class BlackBomb : public BombBase  {
        CLONEOBJ(BlackBomb);
        DECL_TRAITS;
    public:
        BlackBomb (bool burning=false)
        : BombBase(burning)
        {}
    private:
        const char *burn_anim() const { return "it-blackbomb-burning"; }
        void explode() {
            GridPos p = get_pos();
            sound_event ("blackbomb");
            SendExplosionEffect(p, EXPLOSION_BLACKBOMB);
            replace (it_explosion3);
        }
    };
    DEF_TRAITSF(BlackBomb, "it-blackbomb", it_blackbomb,
                itf_static | itf_indestructible | itf_fireproof);

    class BlackBombBurning : public BlackBomb {
        CLONEOBJ(BlackBombBurning);
        DECL_TRAITS;
    public:
        BlackBombBurning() : BlackBomb(true) {}
    };
    DEF_TRAITSF(BlackBombBurning, "it-blackbomb_burning",
                it_blackbomb_burning,
                itf_static | itf_indestructible | itf_norespawn | itf_fireproof);
}


/* -------------------- White bomb -------------------- */

/*! When a white bombs explode, they destroy the floor tile underneath
them and neighboring floors. */

namespace
{
    class WhiteBomb : public BombBase  {
        CLONEOBJ(WhiteBomb);
        DECL_TRAITS;

        const char *burn_anim() const { return "it-whitebomb-burning"; }
        void explode() {
            GridPos p = get_pos();
            sound_event ("whitebomb");
            replace (it_explosion3);
            SendExplosionEffect(p, EXPLOSION_WHITEBOMB);
        }

    public:
        WhiteBomb()
        {}
    };
    DEF_TRAITSF(WhiteBomb, "it-whitebomb", it_whitebomb,
                itf_static | itf_indestructible | itf_fireproof);
}


/* -------------------- Trigger -------------------- */
    /**
     * A switch that is triggered by actors crossing it on the floor and stones
     * being pushed onto it. The external state is read only - value 1 (ON) is
     * reported if the trigger is pressed either by an actor or a stone, a value
     * 0 (OFF) otherwise.<p>
     * Note that due to the Enigma 1.10 standards no actions will be performed
     * on initialization or setting of a trigger, even if actors or a stone are
     * positioned on the grid. But state and model are set according to the other
     * objects.<p>
     * The internal state stores the current number of actors and the presence
     * of a stone. The stone is flagged as the least significant bit, the actor
     * count is stored in the bits above, what is simply an addition of 2 times
     * the actor count to the state. This coding simplifies requests and avoids
     * usage of additional ivars for the complete logical state.
     */
    class Trigger : public Item {
        CLONEOBJ(Trigger);
        DECL_TRAITS;
    public:
        Trigger();
        
        // Object interface
        virtual Value message (const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void init_model();
        
        // Item interface
        virtual void actor_enter(Actor *a);
        virtual void actor_leave(Actor *a);
        virtual void stone_change(Stone *st);
    private:
        // Methods
        int countActors();
        void updateIState(int diffActors = 0, bool refuseAction = false);

    };

    DEF_TRAITSF(Trigger, "it_trigger", it_trigger, itf_static | itf_indestructible);
    
    Trigger::Trigger() {
        setAttr("invisible", false);
    }
    
    Value Trigger::message (const Message &m) {
        if (m.message == "signal" && (server::GameCompatibility != GAMET_ENIGMA || 
                server::EnigmaCompatibility < 1.10)) {
            performAction(m.value.to_bool());  // convert 1/0 values to true/false
            return Value();
        } else if (m.message == "_init") {
            // the state count at init is wrong as some actors on the grid may
            // already have existed on_creation, but all are reported via
            // actor_enter due to actors on_creation.
            // Thus we need to reset and recount the actors:
            state = 0;
            
            // old Enigma versions did issue performAction what is incompatible
            updateIState(countActors(),
                    server::EnigmaCompatibility >= 1.10 || server::GameCompatibility != GAMET_ENIGMA); 
            return Value();
        } else if (m.message == "_jumping" ) {
            updateIState(m.value.to_bool() ? -1 : +1);
        }
        return Item::message(m);
    }
        
    int Trigger::externalState() const {
        return state != 0 ? 1 : 0;
    }
    
    void Trigger::setState(int extState) {
        return;   // ignore any write attempts
    }
    
    void Trigger::on_creation(GridPos p) {
        state = 0;
        updateIState(countActors(), true);
        init_model();
    }
        
    void Trigger::init_model() {
        if (getAttr("invisible").to_bool())
            set_model("invisible");
        else if (state != 0)
            set_model("it-trigger1");
        else
            set_model("it-trigger");
    }
    
    void Trigger::actor_enter(Actor *a) {
        if (!a->is_flying())
            updateIState(+1, !server::WorldInitialized);
    }
    
    void Trigger::actor_leave(Actor *a) {
        if (!a->is_flying())
            updateIState(-1, !server::WorldInitialized);
    }
    
    void Trigger::stone_change(Stone *) {
        updateIState(0);
    }
    
    int Trigger::countActors() {
        vector<Actor*> actors;
        GetActorsInsideField (get_pos(), actors);
        int count = 0;
        for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
            if (!(*it)->is_flying()) count++;
        return count;
    }
    
    void Trigger::updateIState(int diffActors, bool refuseAction) {
        int oldState = state;
        
        state += 2 * diffActors;
        
        Stone *st = GetStone(get_pos());
        state &= ~1;  // delete stone pressure bit
        if (st != NULL && (!st->is_floating() || st->is_kind("st-puzzle*"))) {
            // Hack to make hollow puzzle stones press triggers
            state |= 1;   // add stone pressure bit
        }
                
//        Log << "Trigger update old state " << oldState << " - new state " << state << " refuse action " << refuseAction << "\n";
    
        if ((oldState == 0 && state != 0) || (oldState != 0 && state == 0)) {
            init_model();
            if (!refuseAction) {
                if (state != 0) {
                    sound_event ("triggerdown");
                    performAction(true);
                } else {
                    sound_event ("triggerup");
                    performAction(false);
                }
            }
        }
    }


/* -------------------- Seed -------------------- */
namespace
{
    class Seed : public Item {
        bool activep;

        bool actor_hit(Actor *a) {
            if (activep)
                return false;   // do not pickup growing seed
            return Item::actor_hit(a);
        }
        void on_drop (Actor *) {start_growing();}
        void on_stonehit (Stone *) {start_growing();}
        void processLight(Direction d) {start_growing();}

        virtual Value message(const Message &m) {
            if (m.message == "grow" || m.message == "signal") {
                start_growing();
                return Value();
            }
            return Item::message(m);
        }

        void start_growing() {
            if (!activep) {
                activep = true;
                sound_event ("seedgrow");
                set_anim("it-seed-growing");
            }
        }

        void animcb() {
            GridPos p= get_pos();
            if ((server::GameCompatibility == GAMET_OXYDMAGNUM || server::GameCompatibility == GAMET_OXYD1) &&
                (get_stone_name() == "st-wood-growing" && GetStone(p))) {
                string model = GetStone(p)->get_kind();
                if (model == "st-grate1") {
                    SetFloor(p, MakeFloor("fl-stwood"));
                    kill();
                    return;
               }
           }
           Stone *st = MakeStone (get_stone_name());
           transferName(st);
           SetStone (p, st);
           kill();
        }
        
        virtual bool isStatic() const {
            return activep;  // active seed is static
        }

        virtual const char* get_stone_name() = 0;
    public:
        Seed ()
        : activep(false)
        {}
    };

    class SeedWood : public Seed {
        CLONEOBJ(SeedWood);
        DECL_TRAITS;

        const char* get_stone_name() {
            return "st-wood-growing";
        }

    public:
        SeedWood()
        {}
    };
    DEF_TRAITSR(SeedWood, "it-seed", it_seed, 0.2f);

    class SeedNowood : public Seed {
        CLONEOBJ(SeedNowood);
        DECL_TRAITS;

        const char* get_stone_name() {
            return "st-greenbrown-growing";
        }
    public:
        SeedNowood()
        {}
    };
    DEF_TRAITSR(SeedNowood, "it-seed_nowood", it_seed_nowood, 0.2f);

    class SeedVolcano : public Seed {
        CLONEOBJ(SeedVolcano);
        DECL_TRAITS;

        const char* get_stone_name() {
            return "st-volcano-growing";
        }
    public:
        SeedVolcano()
        {}
    };
    DEF_TRAITSR(SeedVolcano, "it-seed_volcano", it_seed_volcano, 0.2f);
}


/* -------------------- Shogun dot -------------------- */

/** \page it-shogun Shogun Dot

\subsection shogundota Attributes
- \b size:            1..3  (smallest..largest)
- \b target, \b action:   as usual
*/
namespace
{
    class ShogunDot : public Item {
        CLONEOBJ(ShogunDot);
        DECL_TRAITS_ARRAY(3, subtype);
    public:
        static void setup() {
            RegisterItem (new ShogunDot(SMALL));
            RegisterItem (new ShogunDot(MEDIUM));
            RegisterItem (new ShogunDot(LARGE));
        }
    private:
        enum SubType { SMALL, MEDIUM, LARGE };
        ShogunDot(SubType size);

        virtual Value message(const Message &m);
        void stone_change(Stone *st);

        // Variables.
        bool activated;
        SubType subtype;
    };

    ItemTraits ShogunDot::traits[3] = {
        { "it-shogun-s", it_shogun_s, itf_static, 0.0 },
        { "it-shogun-m", it_shogun_m, itf_static, 0.0 },
        { "it-shogun-l", it_shogun_l, itf_static, 0.0 }
    };
}

ShogunDot::ShogunDot (SubType size)
: activated(false), subtype(size)
{
}

void ShogunDot::stone_change(Stone *st) {
    if (activated) {
        if (st == 0) {
            warning("stone_change: Stone disappeared w/o sending me a proper message!");
            activated = false;
            performAction(false);
        }
    }
}

Value ShogunDot::message(const Message &m) {
    if (m.message == "noshogun") {
        if (activated) {
            activated = false;
            performAction(false);
        }
        return Value();
    } else {
        const char *s = m.message.c_str();
        bool size_matches =
            (strncmp(s, "shogun", 6) == 0)    &&
            ((s[6]-'1')              == subtype) &&
            (s[7] == 0);

        if (size_matches != activated) {
            activated = size_matches;
            performAction(activated);
            return Value();
        }
    }
    return Item::message(m);
}


/* -------------------- Magnet -------------------- */

    class Magnet : public Item, public ForceField {
    private:
        enum iState {
            OFF,
            ON
        };
       
    public:
        CLONEOBJ(Magnet);
        DECL_TRAITS_ARRAY(2, state);

        Magnet(bool isOn);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
        virtual Value message(const Message &m);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();

        // ForceField interface
        virtual void add_force(Actor *a, V2 &f);
        
    private:
        double correctedStrength;     // 0.6 * strength
        double squareRange;
    };
    
    Magnet::Magnet(bool isOn) : Item(), correctedStrength (0.6 * 30), squareRange (1000 * 1000) {
        state = isOn ? ON : OFF;
    }

    std::string Magnet::getClass() const {
        return "it_magnet";
    }
    
    void Magnet::setAttr(const std::string &key, const Value &val) {
        if (key == "range") {
            double range = (val.getType() == Value::NIL) ? server::MagnetRange : (double)val;
            squareRange = range * range;
        } else if (key == "strength") {
            correctedStrength = 0.6 * ((val.getType() == Value::NIL) ? server::MagnetForce : (double)val);
        }
        Item::setAttr(key, val);
    }
    
    Value Magnet::message(const Message &m) {
        if (m.message == "_updateglobals" && m.value.to_string() == "it_magnet") {
            if (getAttr("range").getType() == Value::DEFAULT) {
                squareRange = server::MagnetRange * server::MagnetRange;
            }
            if (getAttr("strength").getType() == Value::DEFAULT) {
                correctedStrength = 0.6 * server::MagnetForce;
            }
            return Value();
        }
        return Item::message(m);
    }
    void Magnet::init_model() {
        set_model(ecl::strf("it-magnet%s", state == ON ? "-on" : "-off"));
    }
    
    void Magnet::on_creation(GridPos p) {
        if (getAttr("range").getType() == Value::DEFAULT) {
            squareRange = server::MagnetRange * server::MagnetRange;
        }
        if (getAttr("strength").getType() == Value::DEFAULT) {
            correctedStrength = 0.6 * server::MagnetForce;
        }

        AddForceField(this);
        Item::on_creation(p);
    }
    
    void Magnet::on_removal(GridPos p) {
        Item::on_removal(p);
        RemoveForceField(this);
    }
    
    void Magnet::add_force(Actor *a, V2 &f) {
        if (state == ON) {
            V2 dv = get_pos().center() - a->get_pos_force();
            double squareDist = square(dv);

            if (squareDist >= 0.04 && squareDist < squareRange)
                f += (correctedStrength / squareDist) * dv;
        }
    }
    
    ItemTraits Magnet::traits[2] = {
        { "it_magnet_off", it_magnet_off, itf_static | itf_indestructible, 0.0 },
        { "it_magnet_on",  it_magnet_on,  itf_static | itf_indestructible, 0.0 },
    };


/* -------------------- Wormhole -------------------- */

/** \page it-wormhole Worm hole

Worm holes teleport actors to another place.  They have a variable
attracting force field.

\subsection wormholea Attributes
- \b targetx:       X coordinate of the destination
- \b targety:       Y coordinate of the destination
- \b strength:      Strength of the force field (default: 50)
- \b range:         Range of the force field

\subsection wormholee Example
\verbatim
set_item("it-wormhole", 1,1, {targetx=5.5, targety=10.5, strength=50, range=5})
\endverbatim
*/


    class WormHole : public Item, public ForceField, public TimeHandler {
    private:
        /**
         * warping as bit 2 of state
         */
        enum iState {
            OFF_IDLE,        ///< force off, ready to teleport
            ON_IDLE,         ///< force on,  ready to teleport
            OFF_ENGAGED,     ///< force off, rejecting teleport
            ON_ENGAGED       ///< force on,  rejecting teleport
        };
        
    public:
        CLONEOBJ(WormHole);
        DECL_TRAITS_ARRAY(2, state & 1);
        
        WormHole(bool isOn);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();
        
        // Item interface
        virtual bool actor_hit(Actor *a);

        // ForceField interface
        virtual void add_force(Actor *a, V2 &f);
        
        // TimeHandler interface
        virtual void alarm();
        
    private:
        bool near_center_p(Actor *a);

        double correctedStrength;     ///< 0.6 * strength
        double squareRange;           ///< range of the force squared
    };

    WormHole::WormHole(bool isOn) : Item(), correctedStrength (0.6 * 30), squareRange (1000 * 1000) {
        state = isOn ? ON_IDLE : OFF_IDLE;    // includes warping false
    }
    
    std::string WormHole::getClass() const {
        return "it_wormhole";
    }
    
    void WormHole::setAttr(const std::string &key, const Value &val) {
        if (key == "range") {
            double range = (val.getType() == Value::NIL) ? server::WormholeRange : (double)val;
            squareRange = range * range;
        } else if (key == "strength") {
            correctedStrength = 0.6 * ((val.getType() == Value::NIL) ? server::WormholeForce : (double)val);
        }
        Item::setAttr(key, val);
    }
    
    Value WormHole::message(const Message &m) {
        if (m.message == "_updateglobals" && m.value.to_string() == "it_wormhole") {
            if (getAttr("range").getType() == Value::DEFAULT) {
                squareRange = server::WormholeRange * server::WormholeRange;
            }
            if (getAttr("strength").getType() == Value::DEFAULT) {
                correctedStrength = 0.6 * server::WormholeForce;
            }
            return Value(); 
        }
        return Item::message(m);
    }

    int WormHole::externalState() const {
        return state % 2;
    }
    
    void WormHole::setState(int extState) {
        // switch force on and off
        if (extState != state % 2) {          // react only on force changes
            state = (state & ~1) + extState;  // keep other flags
            if (isDisplayable()) {
                if (extState == 1)
                    AddForceField(this);
                else
                    RemoveForceField(this);
            }
        }
    }
    
    void WormHole::on_creation (GridPos p) {
        Item::on_creation (p);
        if (getAttr("range").getType() == Value::DEFAULT) {
            squareRange = server::WormholeRange * server::WormholeRange;
        }
        if (getAttr("strength").getType() == Value::DEFAULT) {
            correctedStrength = 0.6 * server::WormholeForce;
        }
        if (state % 2 == 1)
            AddForceField(this);
    }

    void WormHole::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        state &= ~2;  // remove teleport engaged flag
        if (state % 2 == 1)
            RemoveForceField(this);
        Item::on_removal(p);
        ASSERT((state & 4) == 0, XLevelRuntime, "Tried to kill a busy wormhole. Please use another way.");
    }
    
    void WormHole::init_model() {
        if ((state & 3) == ON_IDLE)
            set_anim("it-wormhole");
        else
            set_model("it-wormhole-off");
    }
    
    bool WormHole::actor_hit(Actor *actor) {
        ASSERT((state & 4) == 0, XLevelRuntime, "WormHole:: Recursion detected!");
        if ((state & 2) == 0 && near_center_p(actor)) {   // may teleport
            client::Msg_Sparkle (get_pos().center());
            V2 targetpos;
            if (getDestinationByIndex(0, targetpos)) {
                sound_event ("warp");
                double latency = getAttr("interval");
                if(latency > 0) {
                    state |= 2;  // mark engaged
                    GameTimer.set_alarm(this, latency, false);
                    init_model();
                }
                state |= 4;  // mark warping
                bool isScissor = to_bool(getAttr("scissor")) || server::GameCompatibility != GAMET_ENIGMA;
                if (isScissor)
                    KillRubberBands(actor);
                WarpActor(actor, targetpos[0], targetpos[1], false);
                state &= ~4; // release warping
            }
        }
        return false;
    }
    
    void WormHole::add_force(Actor *a, V2 &f) {
        V2 dv = get_pos().center() - a->get_pos_force();
        double squareDist = square(dv);

        if (squareDist >= 0.025 && squareDist < squareRange)
            f += (correctedStrength / squareDist) * dv;
    }
    
    void WormHole::alarm() {
        state &= ~2;  // remove teleport engaged flag
        init_model();
    }
    
    bool WormHole::near_center_p(Actor *a) {
        return (square(a->get_pos() - get_pos().center()) < 0.015625);  // 0.125 ^ 2
    }
    
    ItemTraits WormHole::traits[2] = {
        { "it_wormhole_off", it_wormhole_off, itf_static, 0.0 },
        { "it_wormhole_on",  it_wormhole_on,  itf_static, 0.0 }
    };



/* -------------------- Vortex -------------------- */

/** \page it-vortex Vortex

Vortexes teleport actors to another place.

They may be opened or closed. Is a vortex is closed, the actor cannot enter.


\subsection vortexm Messages
- \b open       opens the vortex
- \b close      closes the vortex
- \b trigger, openclose
- \b signal     signal value: 1 -> "open"; 0 -> "close"
*/

    class Vortex : public Item, public TimeHandler {
    private:
        enum iState {
            OPEN,          ///< TODO exchange OPEN, CLOSED to match external state 
            CLOSED,        ///<
            OPENING,       ///<
            CLOSING,       ///<
            WARPING,       ///< an open vortex occupied in process of sending a marble
            EMITTING,      ///< an open vortex occupied, waiting for the destination to open
            SWALLOWING     ///< an open vortex eats up a marble
        };

    public:
        CLONEOBJ(Vortex);
        DECL_TRAITS_ARRAY(2, is_open());
        
        Vortex(bool opened);
        virtual ~Vortex();

        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);
        virtual void toggleState();

        // GridObject interface
        virtual void init_model();
        virtual void on_removal(GridPos p);
        
        // ModelCallback interface
        virtual void animcb();

        // Item interface
        virtual bool actor_hit(Actor*);

        // TimeHandler interface
        virtual void alarm();

    private:
        void prepare_for_warp(Actor *actor);
        void emit_actor(Vortex *destVortex);

        void perform_warp();    // warp swallowed actor(s)
        void warp_to(const V2 &target);

        bool is_open() const { return externalState() == 1 ; }

    };

    ItemTraits Vortex::traits[2] = {
        {"it_vortex_closed", it_vortex_closed, itf_static | itf_fireproof, 0.0},
        {"it_vortex_open", it_vortex_open,     itf_static | itf_fireproof, 0.0}
    };

    Vortex::Vortex(bool open) : Item() {
        state = open ? OPEN : CLOSED;
        setAttr("$dest_idx", 0);
        setAttr("$dest_vortex", (Object *)NULL);
        setAttr("$grabbed_actor", (Object *)NULL);
    }

    Vortex::~Vortex() {
        GameTimer.remove_alarm(this);
        if (Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"))) {
            // release an actor that is grabbed on behalf of this vortex - actor state FALLING_VORTEX
            SendMessage(actor, "rise");
        }
    }

    std::string Vortex::getClass() const {
        return "it_vortex";
    }
    
    Value Vortex::message(const Message &m) {
        if (m.message == "_passed" && getAttr("autoclose").to_bool()) {
            setState(0);
            performAction(getAttr("$grabbed_actor"));
            return Value();
        }
        return Item::message(m);
    }

    int Vortex::externalState() const {
        return (state >= CLOSED && state <= CLOSING) ? 0 : 1 ;
    }
    
    void Vortex::setState(int extState) {
        if (isDisplayable()) {
            if (extState == 1) {  // open
                if (state == CLOSING) {
                    state = OPENING;
                    get_model()->reverse(); // reverse animation
                }
                else if (state == CLOSED) {
                    state = OPENING;
                    sound_event ("vortexopen");
                    init_model();
                }
            } else if (extState == 0) { // close
                if (state == OPENING) {
                    state = CLOSING;
                    get_model()->reverse(); // reverse animation
                }
                else if (state == OPEN) {
                    state = CLOSING;
                    sound_event ("vortexclose");
                    init_model();
                }
            }
        } else {
            if (extState <= 1 && extState >= 0)
                state = 1 - extState;
        }
    }
        
    void Vortex::toggleState() {
        if (state == OPEN || state == OPENING)
            setState(0);
        else
            setState(1);
    }
    
    void Vortex::init_model() {
        switch(state) {
            case WARPING:
            case OPEN:
            case EMITTING:
            case SWALLOWING:
                set_model("it-vortex-open");
                break;
            case CLOSED: 
                set_model("it-vortex-closed"); break;
            case OPENING: 
                set_anim("it-vortex-opening"); break;
            case CLOSING: 
                set_anim("it-vortex-closing"); break;
        }
    }
    
    void Vortex::on_removal(GridPos p) {
        Item::on_removal(p);
        ASSERT(state != WARPING && state != SWALLOWING && state != EMITTING,
            XLevelRuntime, "Tried to kill a busy vortex. Please use another way.");
    }
    
    void Vortex::animcb() {
        if (state == CLOSING) {
            state = CLOSED;
            init_model();
        }
        else if (state == OPENING) {
            state = OPEN;
            init_model();
        }
    }
    
    bool Vortex::actor_hit (Actor *actor) {
        if (state == OPEN && (length((actor->get_pos()) - get_pos().center()) < 0.25) && actor->can_be_warped())
            prepare_for_warp (actor);
        return false;
    }
    
    void Vortex::alarm() {
        if (state == WARPING) {
            perform_warp();
        } else if (state == EMITTING) {
            emit_actor(dynamic_cast<Vortex *>((Object *)getAttr("$dest_vortex")));
        } else if (state == SWALLOWING) {
            state = WARPING;
            sound_event ("hitfloor");
            perform_warp();
        } else
            ASSERT (0, XLevelRuntime, "Vortex: alarm called with inconsistent state");
    }
    
    void Vortex::prepare_for_warp (Actor *actor) {
        SendMessage(actor, "fallvortex");
        setAttr("$dest_idx", 0);
        setAttr("$grabbed_actor", actor);
        state = SWALLOWING;
    
        GameTimer.set_alarm(this, 0.4, false);
    }
    
    void Vortex::emit_actor(Vortex *destVortex) {
        if (destVortex == NULL)   // destination vortex got killed in meantime
            destVortex = this;    // reemit from source vortex 
        V2 v(destVortex->get_pos().center());
        if (Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"))) {
            WarpActor(actor, v[0], v[1], false);
            SendMessage(actor, "rise");
            if (destVortex != this) {
                bool isScissor = to_bool(getDefaultedAttr("scissor", 
                        (server::EnigmaCompatibility >= 1.10) || server::GameCompatibility != GAMET_ENIGMA));
                if (isScissor)
                    KillRubberBands(actor);
            }
        }
        state = OPEN;
        if (this != destVortex && getAttr("autoclose").to_bool())  // do not close source vortex if destination is currently blocked
            setState(0);
        if (this != destVortex)
            performAction(getAttr("$grabbed_actor"));
    
        setAttr("$grabbed_actor", (Object *)NULL);
    }

    void Vortex::warp_to(const V2 &target) {
        client::Msg_Sparkle (target);
        if (Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"))) {
            WarpActor(actor, target[0], target[1], false);
            SendMessage(actor, "appear");
            bool isScissor = to_bool(getDefaultedAttr("scissor", 
                    (server::EnigmaCompatibility >= 1.10) || server::GameCompatibility != GAMET_ENIGMA));
            if (isScissor)
                KillRubberBands(actor);
        }
        state = OPEN;
        if (getAttr("autoclose").to_bool())
            setState(0);
    
        performAction(getAttr("$grabbed_actor"));
        setAttr("$grabbed_actor", (Object *)NULL);
    }

    void Vortex::perform_warp() {
        Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"));
        if (actor == NULL)
            return;
    
        ASSERT (state == WARPING, XLevelRuntime, "Vortex: perform_warp called with inconsistent state");
    
        V2 v_target;
    
        // is another target position defined?
        int dest_idx = getAttr("$dest_idx");
        if (Object::getDestinationByIndex(dest_idx, v_target)) {
            GridPos  p_target(v_target);
    
            Vortex *v = dynamic_cast<Vortex*>(GetItem(p_target));
    
            if (v) {                // Destination is also a vortex
                Stone *st = GetStone(p_target);
    
                if (st && st->is_sticky(actor)) {
                    // is destination vortex blocked? redirect
                    setAttr("$dest_idx", dest_idx + 1);
                    client::Msg_Sparkle(v_target);
                    WarpActor(actor, v_target[0], v_target[1], false);
                    GameTimer.set_alarm(this, 0.4, false);
                }
                else {
                    switch (v->state) {
                        case OPEN:
                        case OPENING:
                            // destination is open
                            emit_actor(v);
                            break;
        
                        case CLOSED:
                        case CLOSING:
                            // destination is closed
                            SendMessage(v, "open");
                            setAttr("$dest_vortex", v);
                            state = EMITTING;
                            GameTimer.set_alarm(this, 0.4, false);
                            break;
                        case SWALLOWING:
                        case WARPING:
                        case EMITTING:
                            // destination is busy -> don't warp actor, emit
                            // it where it has started
                            emit_actor(this);
                    }
                }
            } else {
                warp_to(v_target);
            }
        }
        else {
            // if no target defined, don't warp actor
            emit_actor(this);
        }
    }


/* -------------------- YinYang item -------------------- */
namespace
{
    class YinYang : public Item {
        CLONEOBJ(YinYang);
        DECL_TRAITS;
    public:
        YinYang()
        {}

        string get_inventory_model() {
            if (player::CurrentPlayer()==0)
                return "it-yinyang";
            else
                return "it-yanying";
        }

        ItemAction activate(Actor *, GridPos p) {
            // Switch to other marble
            player::SwapPlayers();
            // play_sound("switch");   // don't! wrong coordinates!
            sound::EmitSoundEvent ("switchplayer", p.center());
            return ITEM_KEEP;
        }
    };
    DEF_TRAITS(YinYang, "it-yinyang", it_yinyang);
}


/* -------------------- Spade -------------------- */
namespace
{
    class Spade : public Item {
        CLONEOBJ(Spade);
        DECL_TRAITS;

        ItemAction activate(Actor *, GridPos p) {
            if (Item *it=GetItem(p)) {
                sound::EmitSoundEvent ("spade", p.center());
                SendMessage(it, "shovel");
                return ITEM_KEEP;
            }
            return ITEM_DROP;
        }
    public:
        Spade() {}
    };
    DEF_TRAITS(Spade, "it-spade", it_spade);
}


/* -------------------- Pipes -------------------- */
namespace
{
    class Pipe : public Item {
        CLONEOBJ(Pipe);
        int subtype;
        DECL_TRAITS_ARRAY(10, subtype);

        Pipe(int stype) : subtype(stype) {}
        virtual Value message(const Message &m) {
            if (m.message == "_explosion") {
                replace (it_explosion1);
                return Value();
            }
            return Item::message(m);
        }
    public:
        static void setup() {
            for (int i=0; i<10; ++i)
                RegisterItem (new Pipe (i));
        }
    };

    ItemTraits Pipe::traits[] = {
        {"it-pipe-e",  it_pipe_e,  itf_none, 0.0 },
        {"it-pipe-w",  it_pipe_w,  itf_none, 0.0 },
        {"it-pipe-s",  it_pipe_s,  itf_none, 0.0 },
        {"it-pipe-n",  it_pipe_n,  itf_none, 0.0 },
        {"it-pipe-es", it_pipe_es, itf_none, 0.0 },
        {"it-pipe-ne", it_pipe_ne, itf_none, 0.0 },
        {"it-pipe-sw", it_pipe_sw, itf_none, 0.0 },
        {"it-pipe-wn", it_pipe_wn, itf_none, 0.0 },
        {"it-pipe-h",  it_pipe_h,  itf_none, 0.0 },
        {"it-pipe-v",  it_pipe_v,  itf_none, 0.0 },
    };
}


/* -------------------- Pullers -------------------- */
namespace
{
    class Puller : public Item {
        CLONEOBJ (Puller);
        DECL_TRAITS_ARRAY(4, get_orientation());

        bool active;
        Direction m_direction;

        bool actor_hit(Actor *a) {
            if (active)
                return false;
            return Item::actor_hit(a);
        }

        void on_drop(Actor *) { activate(); }

        void activate() {
            active=true;
            set_anim("it-puller-active");
            sound_event ("puller");
        }
        void animcb() {
            Direction dir = get_orientation();
            
            // usage within a st-window
            Stone *stone = GetStone(get_pos());
            if (stone && (stone->get_traits().id == st_window) &&
                    to_bool(SendMessage(stone, "inner_pull", dir))) {
            }
            
            // usage in front of a stone
            else {
                GridPos   stonepos = move(get_pos(), reverse(dir));
                send_impulse(stonepos, dir);
            }
            
            sound_event ("dynamite");
            replace (it_explosion1);
        }

	Direction get_orientation() const {
	    return m_direction;
	}

        Puller(Direction dir)
        : active(false), m_direction(dir)
	{ }
    
        virtual bool isStatic() const {
            return active;  // active puller is static
        }
        
    public:
        static void setup() {
            RegisterItem (new Puller(NORTH));
            RegisterItem (new Puller(EAST));
            RegisterItem (new Puller(SOUTH));
            RegisterItem (new Puller(WEST));
        }
    };

    ItemTraits Puller::traits[4] = {
        { "it-puller-w", it_puller_w, itf_none, 0.0 },
        { "it-puller-s", it_puller_s, itf_none, 0.0 },
        { "it-puller-e", it_puller_e, itf_none, 0.0 },
        { "it-puller-n", it_puller_n, itf_none, 0.0 },
    };
}


/* -------------------- Cracks -------------------- */
namespace
{
    class Crack : public Item {
        CLONEOBJ(Crack);
        DECL_TRAITS_ARRAY(4, get_type());

    public:
        static void setup() {
            RegisterItem (new Crack(0));
            RegisterItem (new Crack(1));
            RegisterItem (new Crack(2));
            RegisterItem (new Crack(3));
        }

    private:
        Crack(int type=0)
        : state(IDLE), anim_end(false)
            {
                setAttr("type", type);
                setAttr("fixed", 0.0);
             }

        enum State { IDLE, CRACKING1, CRACKING2 } state;
        bool anim_end;

        int get_type() const {
            int t = getAttr("type");
            return ecl::Clamp(t, 0, 4);
        }
	    bool is_fixed() const { return getAttr("fixed") != 0; }

        void init_model() {
            if (int t=get_type()) {
                if (t > 3) {
                    state = CRACKING1;
                    sound_event ("floordestroy");
                    set_anim("it-crack_anim1");
                //SetItem(get_pos(), MakeItem("it-debris"));
                }else {
                    set_model(ecl::strf("it-crack%d", t));
                }
            }
            else
                set_model("invisible");
        }
        void animcb() {
            if (state == CRACKING2) {
                GridPos p= get_pos();
                SetFloor(p, MakeFloor("fl-abyss"));
                KillItem(p);
            } else {
                state = CRACKING2;
                set_anim("it-crack_anim2");
            }
        }

        void crack(const GridPos &p) {
            if (Floor *fl = GetFloor(p)) {
                if (fl->is_destructible()) {
                    if (Item *it = GetItem(p))
                        SendMessage (it, "crack");
                    else if (do_crack())
                        SetItem(p, it_crack0);
                }
            }
        }

        void actor_enter(Actor *a) {
            if (a->is_on_floor()) {
                SendMessage(this, "crack");

                if (get_type() <= 3) {
                    GridPos p = get_pos();
                    crack (move(p, NORTH));
                    crack (move(p, EAST));
                    crack (move(p, SOUTH));
                    crack (move(p, WEST));
                }
            }
        }
        bool actor_hit(Actor *a) {
            if (anim_end)
                SendMessage(a, "fall");
            return false;
        }
        virtual Value message(const Message &m) {
            if (m.message == "crack" && state==IDLE && !is_fixed()) {
                int type = get_type();
                if ((type == 0 && do_crack()) || (type > 0)) {
                    setAttr("type", Value((int)getAttr("type") + 1));
                    sound_event ("crack");
                    init_model();
                return Value();
                }
            } else if (m.message == "heat") {
                sound_event ("crack");
                replace(it_debris);
                return true;
            }
            return Item::message(m);
        }

        bool do_crack() {
            if (!is_fixed()) {
                double brittleness = getDefaultedAttr("brittleness", server::Brittleness);
                double rnd = DoubleRand(0, 1);
                return rnd < brittleness;
    	    } else
                return false;
        }
    };
    ItemTraits Crack::traits[4] = {
        {"it-crack0", it_crack0, itf_static | itf_indestructible | itf_fireproof, 0.0},
        {"it-crack1", it_crack1, itf_static | itf_indestructible | itf_fireproof, 0.0},
        {"it-crack2", it_crack2, itf_static | itf_indestructible | itf_fireproof, 0.0},
        {"it-crack3", it_crack3, itf_static | itf_indestructible | itf_fireproof, 0.0}
    };
}

/* -------------------- Debris -------------------- */
namespace
{
    class Debris : public Item {
        CLONEOBJ(Debris);
        DECL_TRAITS;

        bool actor_hit(Actor *a) {
            SendMessage(a, "fall");
            return false;
        }
        void animcb() {
            GridPos p = get_pos();
            SetFloor(p, MakeFloor("fl-abyss"));
            KillItem(p);
        }
    public:
        Debris() {}
    };
    DEF_TRAITSF(Debris, "it-debris", it_debris,
                itf_static | itf_animation | itf_indestructible | itf_fireproof);
}


/* -------------------- Burning floor -------------------- */

namespace
{
    /* Used for animations and interfaces of fire. Study    */
    /* floors.hh and floors.cc for the fire implementation. */

    class Burnable : public Item {
        CLONEOBJ(Burnable);
        DECL_TRAITS_ARRAY(6, state);
    public:
        static void setup() {
            RegisterItem (new Burnable(IDLE));
            RegisterItem (new Burnable(IGNITE));
            RegisterItem (new Burnable(BURNING));
            RegisterItem (new Burnable(FIREPROOF));
            RegisterItem (new Burnable(ASH));
            RegisterItem (new Burnable(OIL));
        }
    private:
        enum State { IDLE, IGNITE, BURNING, FIREPROOF, ASH, OIL };
        Burnable (State initstate) {
            state = initstate;
        }
        State state;

        virtual Value message(const Message &m);
        void animcb();
        bool actor_hit(Actor *a);
        void init_model();
    };

    ItemTraits Burnable::traits[6] = {
        {"it-burnable",           it_burnable,           itf_static, 0.0},
        {"it-burnable_ignited",   it_burnable_ignited,
             itf_static | itf_animation | itf_fireproof, 0.0},
        {"it-burnable_burning",   it_burnable_burning,
             itf_static | itf_animation | itf_fireproof, 0.0},
        {"it-burnable_fireproof", it_burnable_fireproof,
             itf_static | itf_fireproof, 0.0},
        {"it-burnable_ash",       it_burnable_ash,
             itf_static | itf_fireproof, 0.0},
        {"it-burnable_oil",       it_burnable_oil,       itf_static, 0.0},
    };
}

Value Burnable::message(const Message &m) {
    if (m.message == "extinguish") {   // stop / never start burning
        state = FIREPROOF;
        init_model();
        return Value();
    } else if (m.message == "_brush" && (state == ASH || state == FIREPROOF)) {
        kill();   // The brush cleans the floor
        return Value();
    } else if (Floor *fl = GetFloor(get_pos())) {
        if (m.message == "ignite" || m.message == "_explosion")
            return SendMessage(fl, "ignite");
    }
    return Item::message(m);
}

void Burnable::animcb() {
    if(Floor *fl = GetFloor(get_pos()))
        fl->on_burnable_animcb(state == IGNITE);
}

bool Burnable::actor_hit(Actor *a) {
    if (state == IGNITE || state == BURNING)
        SendMessage(a, "shatter");
    return false;
}

void Burnable::init_model() {
    if(state == OIL) {
        string mymodel = "it-burnable_oil";
        mymodel = mymodel + ecl::strf("%d", IntegerRand(1, 4));
        set_model(mymodel.c_str());
    } else
        Item::init_model();
}


/* -------------------- Fire Extinguisher -------------------- */
namespace
{
    /*! This items can extinguish burning floor. */
    class Extinguisher : public Item {
        CLONEOBJ(Extinguisher);
        DECL_TRAITS_ARRAY(3, get_load());
    public:
        static void setup() {
            RegisterItem (new Extinguisher(0));
            RegisterItem (new Extinguisher(1));
            RegisterItem (new Extinguisher(2));
        }

    private:
        Extinguisher (int load) {
    	    setAttr("load", load);
        }

        int get_load() const { return ecl::Clamp<int>(getAttr("load"),0,2); }
        void set_load (int load) { setAttr("load", ecl::Clamp<int>(load,0,2)); }

        void extinguish (GridPos p) {
            if (Item *it = GetItem(p)) {
                SendMessage (it, "extinguish");
            } else {
                SetItem (p, it_burnable_fireproof);
            }
        }

        /* ---------- Item interface ---------- */

        ItemAction activate(Actor *a, GridPos p);
    };

    ItemTraits Extinguisher::traits[3] = {
        {"it-extinguisher_empty",  it_extinguisher_empty,  itf_none, 0.0},
        {"it-extinguisher_medium", it_extinguisher_medium, itf_fireproof, 0.0},
        {"it-extinguisher",        it_extinguisher,        itf_fireproof, 0.0},
    };
}

ItemAction Extinguisher::activate(Actor *a, GridPos p)
{
    int load = get_load();
    if (load > 0) {
        extinguish (p);
        extinguish (move(p, NORTH));
        extinguish (move(p, SOUTH));
        extinguish (move(p, EAST));
        extinguish (move(p, WEST));
        if (load > 1) {
            // full extinguisher has a larger range
            extinguish (move(p, NORTH, NORTH));
            extinguish (move(p, NORTH, EAST));
            extinguish (move(p, SOUTH, SOUTH));
            extinguish (move(p, SOUTH, WEST));
            extinguish (move(p, EAST, EAST));
            extinguish (move(p, EAST, SOUTH));
            extinguish (move(p, WEST, WEST));
            extinguish (move(p, WEST, NORTH));
        }
        set_load(load - 1);

        // Redraw the player's inventory, the visual representation of
        // the extinguisher has changed.
        player::RedrawInventory();
    }
    return ITEM_DROP;
}


/* -------------------- Flags -------------------- */

namespace
{
    /*! Flags can be used to set a new respawn point for the black or
      white marble. */
    class FlagBlack : public Item {
        CLONEOBJ(FlagBlack);
        DECL_TRAITS;

        void on_drop(Actor *) {
            player::SetRespawnPositions(get_pos(), true);
        }
        void on_pickup(Actor *) {
            player::RemoveRespawnPositions(true);
        }

    public:
        FlagBlack() {}
    };
    DEF_TRAITS(FlagBlack, "it-flagblack", it_flagblack);

    class FlagWhite : public Item {
        CLONEOBJ(FlagWhite);
        DECL_TRAITS;

        void on_drop(Actor *) {
            player::SetRespawnPositions(get_pos(), false);
        }
        void on_pickup(Actor *) {
            player::RemoveRespawnPositions(false);
        }

    public:
        FlagWhite()
        {}
    };

    DEF_TRAITS(FlagWhite, "it-flagwhite", it_flagwhite);
}



/* -------------------- Blocker item -------------------- */


    /**
     * A door like object that can be opened and closed by a BoulderStone. This item
     * represents the open state of the door. The closed state is represented by a
     * BlockerStone.<p>
     * If a BoulderStone moves over a BlockerItem the BlockerItem starts growing and
     * replaces itself by a BlockerStone. But other Stones can be pushed over a
     * BlockerItem without causing its transformation. <p>
     * It fully supports the door messages "open", "close", "toggle", "signal" and
     * attribute "state". If the item is blocked by a stone the messages will not
     * cause an instant growing. But the message will be remembered in the internal
     * state and the item acts as soon as the stone moves away.<p>
     * An initial BlockerItem with a BoulderStone on top will grow as soon as it moves
     * away. An initial it_blocker_new with a BloulderStone on top will not grow when
     * the stone moves away.<p>
     * Note that this is the only door object that allows a stone to be pushed through.
     * A BlockerItem can be killed by a BrakeItem being dropped.
     */
    class BlockerItem : public Item, public TimeHandler {
        DECL_TRAITS;
    private:
        enum iState {
            IDLE,       ///< neutral state
            NEW,        ///< a new BlockerItem that replaced a recently shrinked BlockerStone -
                        ///< a BoulderStone is awaited, but it has to arrive in time 
            LOCKED,     ///< a stone covers the BlockerItem and locked it, so it will not grow
                        ///< when the stone moves away. This is the state that a BoulderStone causes
                        ///< on its first visit immediatly after shrinking the BlockerStone
            UNLOCKED    ///< a stone covers the BlockerItem and unlocked it, so it will grow
                        ///< when the stone moves away. This is the state that a BoulderStone causes
                        ///< on its second visit when moving onto an idle BlockerItem. The blocker
                        ///< will grow when the stones moves away
        };

    public:
        BlockerItem(bool shrinked_recently);
        ~BlockerItem();

        // Object interface
        virtual BlockerItem *clone();
        virtual void dispose();
        virtual Value message(const Message &m);

        // StateObject interface
        virtual void toggleState();
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();
        virtual void actor_leave(Actor *a);

        // Item interface
        virtual void stone_change(Stone *st);

        // TimeHandler interface
        virtual void alarm();
    
    private:
        void setIState(iState newState);
        void grow();
    };
    
    DEF_TRAITSF(BlockerItem, "it_blocker", it_blocker, itf_static);


    BlockerItem::BlockerItem(bool shrinked_recently) {
        state = shrinked_recently ? NEW : IDLE;
    }

    BlockerItem::~BlockerItem() {
        GameTimer.remove_alarm (this);
    }

    BlockerItem * BlockerItem::clone() {
        BlockerItem * dup = new BlockerItem(*this);
        NameObject(dup, "$BlockerItem#");  // auto name object to avoid problems with object values
        return dup;
    }
    
    void BlockerItem::dispose() {
        delete this;
    }

    Value BlockerItem::message(const Message &m) {
        if (m.message == "_init") { 
            if (Stone *st = GetStone(get_pos())) {
                if (st->is_kind("st_boulder"))
                    if (state == IDLE && server::GameCompatibility != GAMET_PEROXYD)
                        setIState(UNLOCKED);
                    else if (state == NEW || server::GameCompatibility != GAMET_PEROXYD)
                        setIState(LOCKED);
            }
            return Item::message(m);    // pass on init message
        } else if (m.message =="_performaction") {
            performAction(true);
            return Value();
        }
        return Item::message(m);
    }
    
    void BlockerItem::toggleState() {
        if (state == UNLOCKED) {  // revoke pending grow/close
            setIState(LOCKED);
        }
        else {
            setState(0);  // close
        }
    }
    
    int BlockerItem::externalState() const {
        return 1;   // always open -- st_blocker is closed
    }
    
    void BlockerItem::setState(int extState) {
        if (extState == 1) {         // open (shrink)
            if (state == UNLOCKED)   //   revoke pending grow/close
                setIState(LOCKED);
        }
        else {                       // close (grow)
            switch (state) {
                case LOCKED:
                    setIState(UNLOCKED);  // close when stone is removed
                    break;
                case UNLOCKED:
                    break;                // will close anyway when stone is removed
                case IDLE:
                case NEW:
                    grow();                    
                    break;
            }
        }
    }
    
    void BlockerItem::on_creation(GridPos p) {
        if (state == NEW) {
            GameTimer.set_alarm(this, 0.5, false);
        }
        Item::on_creation(p);
    }

    void BlockerItem::on_removal(GridPos p) {
        setIState(IDLE);
        Item::on_removal(p);
    }

    void BlockerItem::init_model() {
        set_model("it-blocker");
    }
    
    void BlockerItem::actor_leave(Actor *a) {
        if (Value v = getAttr("autoclose")) {
            if (v.to_bool()) {
                setState(0);     // close
            }
        }
    }
    
    void BlockerItem::stone_change(Stone *st) {
        if (st != NULL) {
            if (st->is_kind("st_boulder")) { // boulder arrived
                switch (state) {
                    case IDLE:
                        setIState(UNLOCKED);  // will grow when boulder moves away
                        break;
                    case NEW:
                        setIState(LOCKED);    // will not grow when boulder moves away
                        break;
                    case UNLOCKED:
                    case LOCKED:
                        // two BoulderStones running directly next to each other
                        // let second pass as well (correct? siegfried says yes)
                        // note: all stone moves are handled in a timestep before
                        //   the world informs the items about stone changes
                        break;
                }
            }
            else { // any other stone
                setIState(LOCKED);
            }
        }
        else {              // stone disappeared
            switch (state) {
                case LOCKED:
                    setIState(IDLE);
                    break;
                case UNLOCKED:
                    grow();
                    break;
                case IDLE:
                case NEW:
                    // no action
                    break;
            }
        }
    }

    void BlockerItem::alarm() {
        if (state == NEW) { // BoulderStone did not arrive in time
            setIState(IDLE);
        }
    }

    void BlockerItem::setIState(iState newState) {
        if (state != newState) {
            if (state == NEW)
                GameTimer.remove_alarm(this);
            else if (newState == NEW)
                GameTimer.set_alarm(this, 0.5, false);
            state = newState;
        }
    }
    
    void BlockerItem::grow() {
        Stone *st = MakeStone("st_blocker_new");
        SetStone(get_pos(), st);
        transferIdentity(st);
        if (Value v = getAttr("autoclose"))
            st->setAttr("autoclose", v); 
        SendMessage(st, "_performaction");
        kill();
    }


/* -------------------- Ring -------------------- */
namespace
{
    class Ring : public Item {
        CLONEOBJ(Ring);
        DECL_TRAITS;
    public:
        Ring() {}

        ItemAction activate(Actor *a, GridPos) {
            if (ExchangeMarbles(a)) {
                sound_event ("switchmarbles");
            }
            else {
                RespawnActor(a);
            }
            return ITEM_DROP;
        }
    };
    DEF_TRAITS(Ring, "it-ring", it_ring);
}

//----------------------------------------
// Bridge item (for Oxyd compatibility)
//
// Floor tiles seem to be static in Oxyd and cannot change dynamically
// or be animated.  For this reason, every bridge floor in Oxyd has to
// be combined with a bridge "item" that receives the signals, shows
// the animation and sets or removes the floor.
//----------------------------------------
namespace
{
    class OxydBridge : public Item {
        CLONEOBJ(OxydBridge);
        DECL_TRAITS;

        virtual Value message(const Message &m) {
            if (m.message == "signal") {
                int ival = m.value;
                Floor *floor = GetFloor (get_pos());
                if (ival > 0)
                    SendMessage (floor, "close");
                else
                    SendMessage (floor, "open");
                return Value();
            }
            return Item::message(m);
        }
    public:
        OxydBridge() {}
    };
    DEF_TRAITSF(OxydBridge, "it-bridge-oxyd", it_bridge_oxyd,
                itf_static | itf_indestructible | itf_invisible | itf_fireproof);

    class OxydBridgeActive : public OxydBridge {
        CLONEOBJ(OxydBridgeActive);
        DECL_TRAITS;

        void on_creation (GridPos p) {
            Floor *floor = GetFloor (p);
            SendMessage (floor, "close");
        }
    public:
        OxydBridgeActive() {}
    };
    DEF_TRAITSF(OxydBridgeActive, "it-bridge-oxyd_active", it_bridge_oxyd_active,
                itf_static | itf_indestructible | itf_invisible | itf_fireproof);
}


/* -------------------- Sensors -------------------- */


/* Basically behave like regular triggers, but they are invisible and can be
   activated only once. */
    class Sensor : public Item {
        CLONEOBJ(Sensor);
        DECL_TRAITS_ARRAY(2, traitsIdx());
    public:
        static void setup();
            
        Sensor(bool inverse);
        
        // Object interface
        virtual Value message(const Message &m);
        
        // GridObject interface
        virtual void init_model();

        // ModelCallback interface
        virtual void animcb();

        // Item interface
        virtual void actor_enter(Actor *a);
    
    private:
        int traitsIdx() const;
    };
    
    void Sensor::setup() {
        RegisterItem (new Sensor(false));
        RegisterItem (new Sensor(true));
    }
    
    Sensor::Sensor(bool inverse) {
        Object::setAttr("inverse", inverse);
    }
    
    Value Sensor::message(const Message &m) {
        if (m.message == "_hit") {   // door knocking forward to black/whitballstone
            setAttr("$hitactor", m.value);
            performAction(true);
            setAttr("$hitactor", (Object *)NULL);
            return Value();
        } else if (m.message == "_hitactor") {
            return getAttr("$hitactor");
        }
        return Item::message(m);
    }
    
    void Sensor::init_model() {
        if (getAttr("invisible").to_bool())
            set_model("invisible");
        else
            set_model("it_sensor");
    }

    void Sensor::animcb() {
        init_model();
    }

    void Sensor::actor_enter(Actor *) {
        if (!getAttr("invisible").to_bool())
            set_anim("it_sensor_hit");
        performAction(true);
    }
    
    int Sensor::traitsIdx() const {
        return getAttr("inverse").to_bool() ? 1 : 0;
    }
//    DEF_TRAITSF(Sensor, "it_sensor", it_sensor, itf_static);
    ItemTraits Sensor::traits[2] = {
        {"it_sensor",  it_sensor,  itf_static},
        {"it_sensor",  it_inversesensor,  itf_static}
    };

/* -------------------- Signal filters -------------------- */
namespace
{
    class SignalFilterItem : public Item {
        CLONEOBJ(SignalFilterItem);
        DECL_TRAITS_ARRAY(2, type);

        SignalFilterItem(int type_) : type(type_) {
            ASSERT(type >= 0 && type <= 1, XLevelRuntime, "SignalFilterItem: type unknown");
        }

        virtual Value message(const Message &m) {
            if (m.message == "signal") {
                int value = m.value;
//                 warning("received signal with value %i", value);
                if (value)
                    performAction(type != 0);
                return Value();
            }
            return Item::message(m);
        }

        // type of signal filter
        // 0 : receive 1 -> send 0
        // 1 : receive 1 -> send 1
        int type;

    public:
        static void setup() {
            RegisterItem (new SignalFilterItem(0));
            RegisterItem (new SignalFilterItem(1));
        }
    };
    ItemTraits SignalFilterItem::traits[2] = {
        {"it-signalfilter0", it_signalfilter0,
            itf_static | itf_invisible | itf_fireproof, 0.0},
        {"it-signalfilter1", it_signalfilter1,
            itf_static | itf_invisible | itf_fireproof, 0.0},
    };
}


/* -------------------- EasyKillStone -------------------- */

/*
This item is never visible during the game.  Its only purpose is to
modify the level if the difficulty mode is set to easy *before* the
game starts.

In easy game mode this item removes the stone at its position.  Then
in both modes it kills itself.

E.g. it can be used to hide walls in easy game mode.

\ref st-easymode
*/

namespace
{
    class EasyKillStone : public Item {
        CLONEOBJ(EasyKillStone);
        DECL_TRAITS;

        virtual Value message(const Message &m);
    public:
        EasyKillStone() {}
    };
    DEF_TRAITSF(EasyKillStone, "it-easykillstone",
                it_easykillstone, itf_invisible | itf_fireproof);
}

Value EasyKillStone::message(const Message &m)
{
    if (m.message == "_init") {
        // does not work in on_creation() because items are created
        // before stones are created.
        if (server::GetDifficulty() == DIFFICULTY_EASY) {
            if (Stone *st = GetStone (get_pos())) {
                if (st->is_kind ("st-death") ||
                    st->is_kind ("st-flash") ||
                    st->is_kind ("st-thief"))
                {
                    SetStone (get_pos(), MakeStone ("st-plain"));
                }
                else
                    KillStone(get_pos());
            }
        }
        kill();
        return Value();
    }
    return Item::message(m);
}

/* -------------------- EasyKeepStone -------------------- */
namespace
{
    class EasyKeepStone : public Item {
        CLONEOBJ(EasyKeepStone);
        DECL_TRAITS;

        virtual Value message(const Message &m) {
            if (m.message == "_init") {
                // does not work in on_creation() because items are created
                // before stones are created.
                if (server::GetDifficulty() == DIFFICULTY_HARD)
                    KillStone(get_pos());
                kill();
                return Value();
            }
            return Item::message(m);
        }
    public:
        EasyKeepStone() {}
    };
    DEF_TRAITSF(EasyKeepStone, "it-easykeepstone", it_easykeepstone,
                itf_invisible | itf_fireproof);
}

/* -------------------- SingleKillStone -------------------- */
namespace
{
    class OnePKillStone : public Item {
        CLONEOBJ (OnePKillStone);
        DECL_TRAITS;

        virtual Value message (const Message &m) {
            if (m.message == "_init") {
                if (server::SingleComputerGame)
                    KillStone (get_pos());
                kill();
                return Value();
            }
            return Item::message(m);
        }
    public:
        OnePKillStone () {}
    };
    DEF_TRAITSF(OnePKillStone, "it-1pkillstone", it_1pkillstone,
                itf_invisible | itf_fireproof);

    class TwoPKillStone : public Item {
        CLONEOBJ (TwoPKillStone);
        DECL_TRAITS;

        virtual Value message (const Message &m) {
            if (m.message == "_init") {
                if (!server::SingleComputerGame)
                    KillStone (get_pos());
                kill();
                return Value();
            }
            return Item::message(m);
        }
    public:
        TwoPKillStone () {}
    };
    DEF_TRAITSF(TwoPKillStone, "it-2pkillstone", it_2pkillstone,
                itf_invisible | itf_fireproof);
}


/* -------------------- Glasses -------------------- */
namespace
{
    class Glasses : public Item {
        CLONEOBJ(Glasses);
        DECL_TRAITS;

        static bool wears_glasses(Actor *a) {
            return player::GetInventory(a)->find("it-glasses") != -1;
        }

        void on_drop(Actor *a) {
            if (!wears_glasses(a)) // 'this' was the only it-glasses
                BroadcastMessage("glasses", 0.0, GRID_STONES_BIT);
        }
        void on_pickup(Actor *a) {
            if (!wears_glasses(a)) // no glasses before
                BroadcastMessage("glasses", 1.0, GRID_STONES_BIT);
        }
        void on_stonehit(Stone *) {
            sound_event ("shatter");
            replace (it_glasses_broken);
        }
    public:
        Glasses()
        {}
    };
    DEF_TRAITS(Glasses, "it-glasses", it_glasses);
}


/* -------------------- Invisible abyss -------------------- */
namespace
{
    class InvisibleAbyss : public Item {
        CLONEOBJ(InvisibleAbyss);
        DECL_TRAITS;
        bool actor_hit(Actor *a) {
            SendMessage(a, "fall");
            return false;
        }
    public:
        InvisibleAbyss() {}
    };
    DEF_TRAITSF(InvisibleAbyss, "it-abyss", it_abyss,
                itf_static | itf_invisible | itf_fireproof);
}


/* -------------------- Landmine -------------------- */
namespace
{
    class Landmine : public Item {
        CLONEOBJ(Landmine);
        DECL_TRAITS;

	void explode() {
            sound_event ("landmine");
	    replace (it_explosion2);
	}

        bool actor_hit (Actor *a) {
            const double ITEM_RADIUS = 0.3;
            double dist = length(a->get_pos() - get_pos().center());
            if (dist < ITEM_RADIUS)
		explode();
            return false;
        }

        void on_stonehit(Stone *) { explode(); }
    public:
        Landmine()
        {}
    };
    DEF_TRAITSF(Landmine, "it-landmine", it_landmine, itf_static);
}


/* -------------------- Cross -------------------- */
    class Cross : public Item, public TimeHandler {
        CLONEOBJ(Cross);
        DECL_TRAITS;

    public:
        virtual ~Cross();
        
        // Object interface
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);
        
        // Item interface
        virtual void actor_enter(Actor *a);
        virtual void actor_leave(Actor *a);

        // TimeHandler interface
        virtual void alarm();
    };
        
    Cross::~Cross() {
        GameTimer.remove_alarm(this);
    }
    
    Value Cross::message(const Message &m) {
        if (server::GameCompatibility == enigma::GAMET_PEROXYD) {
            // Crosses can be used to invert signals in Per.Oxyd
            if (m.message == "signal") {
                performAction(!m.value.to_bool()); // convert 1/0 values to true/false
                return Value();
            }
        } else if (enigma_server::GameCompatibility == GAMET_ENIGMA) {
            if (m.message == "_brush") {
                KillItem(this->get_pos());
                return Value();
            }
        }
        return Item::message(m);
    }
    
    void Cross::setState(int extState) {
        return;   // ignore any write attempts
    }

    void Cross::actor_enter(Actor *a) {
        if ((state == 0) && a->getAttr("player")) {
            state = 1;
            GameTimer.set_alarm (this, getAttr("interval"));
        }
    }

    void Cross::actor_leave(Actor *) {
        if (state == 1) {
            GameTimer.remove_alarm (this);
            state = 0;
        }
    }

    void Cross::alarm() {
        state = 0;
        performAction(true);
    }
    
    DEF_TRAITSF(Cross, "it_cross", it_cross, itf_static);



/* -------------------- Bag -------------------- */
namespace
{
    class Bag : public Item, public enigma::ItemHolder {
        DECL_TRAITS;

        enum { BAGSIZE = 13 };
        vector<Item *> m_contents;

        // Item interface
        bool actor_hit (Actor *a) {
            if (Item::actor_hit(a)) {
                if (Inventory *inv = player::MayPickup(a, NULL)) {
                    std::vector<Item *>::size_type oldSize = m_contents.size();
                    inv->takeItemsFrom(this);
                    if (oldSize != m_contents.size() && !inv->willAddItem(this)) {
                        // some items have been picked up but the bag will not
                        // be picked up (and cause the following actions)
                        player::RedrawInventory (inv);
                        sound_event ("pickup");
                    }
                    return true;
                }
            }
            return false;
        }

    public:
        virtual Bag * clone() {
            ASSERT(is_empty(), XLevelRuntime, "Bag:: Clone of a full bag!");
            return new Bag(*this);
        }

        virtual void dispose() {
            Item * it = yield_first();
            while (it != NULL) {
                DisposeObject(it);
                it = yield_first();
            }
            delete this;
        }

        virtual void on_creation (GridPos p) {
            GridObject::on_creation(p);
            for (vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
                (*itr)->setOwnerPos(p);
        }
    
        virtual void on_removal (GridPos p) {
            GridObject::on_removal(p);
            for (vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
                (*itr)->setOwner(-1);            
        }
        
        virtual void setOwner(int player) {
            GridObject::setOwner(player);
            for (vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
                (*itr)->setOwner(player);            
        }
    
        virtual void setOwnerPos(GridPos p) {
            GridObject::setOwnerPos(p);
            for (vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
                (*itr)->setOwnerPos(p);
        }
    	
        // ItemHolder interface
        virtual bool is_full() const {
            return m_contents.size() >= BAGSIZE;
        }
        virtual void add_item (Item *it) {
            // thieves may add items beyond pick up limit BAGSIZE
            m_contents.insert (m_contents.begin(), it);
            it->setOwnerPos(get_pos());  // item is at same position as bag
        }

        virtual bool is_empty() const {
            return m_contents.size() == 0;
        }

        virtual Item *yield_first() {
            if (m_contents.size() > 0) {
                Item *it = m_contents[0];
                m_contents.erase (m_contents.begin());
                it->setOwner(-1);  // no owner
                return it;
            }
            return NULL;
        }

        Bag()
        {}

        ~Bag() {
            // Bags on the grid are disposed, but bags in the inventory need to be
            // delete their contents on the destructor
            ecl::delete_sequence (m_contents.begin(), m_contents.end());
        }
    };
    DEF_TRAITS(Bag, "it-bag", it_bag);
}

/* -------------------- pencil -------------------- */
namespace
{
    class Pencil : public Item {
        CLONEOBJ(Pencil);
        DECL_TRAITS;

        ItemAction activate(Actor * a, GridPos p) {
            if (enigma_server::GameCompatibility == GAMET_ENIGMA) {
                if (Item *it=GetItem(p)) {
                    return ITEM_KEEP;
                }
                // If the actor is flying and tries to make a cross, drop the it-pencil
                if (a->is_flying()) {
                    return ITEM_DROP;
                }

                Floor *fl = GetFloor(p);
                string model = fl->get_kind();

                /* do not allow markings on this floortypes:
                   fl-abyss, fl-water, fl-swamp
                   fl-bridge[{-closed,-open}]?
                   markings on fl-ice will result as it-crack1
                */
                if (model == "fl-abyss" || model == "fl-water" || model == "fl-swamp") {
                    return ITEM_KEEP;
                } else  if (model == "fl-ice") {
                    SetItem (p, it_crack1);
                } else {
                    SetItem (p, it_cross);
                }
                return ITEM_KILL;
            }
        }

    public:
        Pencil() {}
    };

    DEF_TRAITS(Pencil, "it-pencil", it_pencil);
}

/* -------------------- it-death -------------------- */
namespace
{
    class Death : public Item {
        CLONEOBJ(Death);
        DECL_TRAITS;

        bool active;

        bool actor_hit(Actor *a) {
            ActorInfo &ai = * a->get_actorinfo();
            if (!ai.grabbed) {
                SendMessage(a, "shatter");
                if (!active) {
                    active=true;
                    set_anim("it-death-anim");
                }
            }
            return false;
        }

    protected:
        void animcb() { set_model("it-death"); active=false; }

    public:
        Death() : active(false) {}
    };

    DEF_TRAITSF(Death, "it-death", it_death, itf_static | itf_indestructible);
}

/* -------------------- HStrip and VStrip -------------------- */
namespace
{
    class HStrip : public Item {
        CLONEOBJ(HStrip);
        DECL_TRAITS;
    public:
        HStrip() {
        }
        bool covers_floor(ecl::V2 pos) const {
            if (GridPos(pos) != get_pos())
                return false;

            const double MAXDIST = 6.0/32;
            double ycenter = get_pos().y + 0.5;
            return (fabs(pos[1] - ycenter) > MAXDIST) ? false : true;
        }
    };
    DEF_TRAITSF(HStrip, "it-hstrip", it_hstrip, itf_static);

    class VStrip : public Item {
        CLONEOBJ(VStrip);
        DECL_TRAITS;
    public:
        VStrip() {
        }
        bool covers_floor(ecl::V2 pos) const {
            if (GridPos(pos) != get_pos())
                return false;

            const double MAXDIST = 5.0/32;
            double xcenter = get_pos().x + 0.5;
            return (fabs(pos[0] - xcenter) > MAXDIST) ? false : true;
        }
    };
    DEF_TRAITSF(VStrip, "it-vstrip", it_vstrip, itf_static);

/* -------------------- it-surprise -------------------- */

    class SurpriseItem : public Item {
        CLONEOBJ(SurpriseItem);
        DECL_TRAITS;

        void on_drop (Actor *) {
            static ItemID items[] = {
                it_umbrella,
                it_spring1,
                it_dynamite,
                it_coffee,
                it_hammer
            };
            replace (items[enigma::IntegerRand (0, 4)]);
        }
    public:
        SurpriseItem() {
        }
    };
    DEF_TRAITS(SurpriseItem, "it-surprise", it_surprise);
}

/* -------------------- ChangeFloorItem -------------------- */
namespace
{
    class ChangeFloorItem : public Item {
        CLONEOBJ(ChangeFloorItem);
        DECL_TRAITS;

        void exchange_floor (const char *a, const char *b) {
            GridPos p = get_pos();
            if (Floor *fl = GetFloor(p)) {
                if (fl->is_kind(a))
                    SetFloor (p, MakeFloor(b));
                else if (fl->is_kind(b))
                    SetFloor (p, MakeFloor(a));
            }
        }

        void actor_leave (Actor *) {
            if (server::TwoPlayerGame) {
                // two players: black / white tile
                exchange_floor ("fl-acwhite", "fl-acblack");
            } else {
                // one player: left / right accel
//                exchange_floor ("fl-
            }
        }

    public:
        ChangeFloorItem() {
        }
    };
    DEF_TRAITSF(ChangeFloorItem, "it-changefloor", it_changefloor,
                itf_static | itf_invisible);
}

/* -------------------- Drop -------------------- */

namespace
{
    Actor *replace_actor (Actor *olda, Actor *newa)
    {
        ActorInfo *info = newa->get_actorinfo();
        info->pos = olda->get_pos();
        info->vel = olda->get_vel();

        if (Value v = olda->getAttr("player")) {
            player::ReplaceActor((int)v, olda, newa);
        }

        AddActor (newa);
        if (!YieldActor (olda)) {
            enigma::Log << "Strange: could not remove old actor\n";
        }
        olda->hide();
        newa->show();
        return olda;
    }

    class DropCallback : public enigma::TimeHandler {
        Actor *rotor;
        Actor *old;
    public:
        DropCallback (Actor *rotor_, Actor *old_)
        : rotor (rotor_), old (old_)
        {}

        // TimerHandler interface
        virtual void alarm()
        {
            replace_actor (rotor, old);

            delete rotor;
            delete this;
        }
    };

    class Drop : public Item {
        CLONEOBJ (Drop);
        DECL_TRAITS;

        ItemAction activate(Actor *a, GridPos)
        {
            const double ROTOR_LIFETIME = 5.0;

            int     iplayer = a->getAttr("player");
            ActorID id      = get_id (a);

            if (id == ac_blackball || id == ac_whiteball) {
                // Kill ALL rubberbands connected with the actor:
                KillRubberBands (a);
                Actor *rotor = MakeActor (ac_rotor);
                rotor->setAttr("mouseforce", Value (1.0));
                rotor->setAttr("controllers", Value (iplayer+1));
                rotor->setAttr("player", Value (iplayer));
                rotor->setAttr("gohome", Value (0.0));
                rotor->setAttr("essential", a->getAttr("essential"));
                std::string essId;
                if (Value v = a->getAttr("essential_id")) {
                    essId = v.to_string();
                } else {
                    essId = a->get_traits().name;
                }
                rotor->setAttr("essential_id", Value(essId));

                replace_actor (a, rotor);

                GameTimer.set_alarm (new DropCallback (rotor, a),
                                            ROTOR_LIFETIME,
                                            false);
            }
            return ITEM_KILL;	       // remove from inventory
        }

    public:
        Drop() {}
    };
    DEF_TRAITS(Drop, "it-drop", it_drop);
}

/* -------------------- Rubberband -------------------- */
namespace
{
    class Rubberband : public Item {
        CLONEOBJ(Rubberband);
        DECL_TRAITS;

        ItemAction activate(Actor *a, GridPos p) {
            // Default values for the rubberband:
            double strength = getDefaultedAttr("strength", 10.0);
            double length = getDefaultedAttr("length", 1.0);
            double minlength = getDefaultedAttr("minlength", 0.0);

            RubberBandData rbd;
            rbd.strength = strength;
            rbd.length = length;
            rbd.minlength = minlength;

            // Target to connect to, default: ""
            std::string target(getAttr("target"));
            
            // TODO: Multiple Targets!
            // TODO: Target for black and target for white marble?
            // TODO: MultiplayerGame: Defaulttarget is second actor!

            // The mode attribute "scissor" defines, if when using an it-rubberband,
            // other rubberbands to the actor will be cut of or not, true means they will. false is default.
            bool isScissor = to_bool(getAttr("scissor"));

            // Get actor or stone with the name, given in "connect_to":
            Actor *target_actor = dynamic_cast<Actor*>(GetNamedObject(target));
            Stone *target_stone = dynamic_cast<Stone*>(GetNamedObject(target));

            // Target does NOT exist, Drop Item
            if((!target_actor)&&(!target_stone)) return ITEM_DROP;

            if (isScissor)
                KillRubberBands (a);

            sound_event ("rubberband");
            if (target_actor) {
                // It's not allowed to connect a rubberband to self.
                if (target_actor != a)
                    AddRubberBand (a,target_actor,rbd);
                else
                    return ITEM_DROP; }
            else
                AddRubberBand (a,target_stone,rbd);

            return ITEM_KILL;
        }

    public:
        Rubberband() {}
    };
    DEF_TRAITS(Rubberband, "it-rubberband", it_rubberband);
}

/* -------------------- Functions -------------------- */

void InitItems()
{
    RegisterItem (new Bag);
    RegisterItem (new Banana);
    RegisterItem (new BlackBomb);
    RegisterItem (new BlackBombBurning);
    Register ("it_blocker_new", new BlockerItem(true));
    RegisterItem (new BlockerItem(false));
    RegisterItem (new Booze);
    RegisterItem (new Brake);
    RegisterItem (new BrokenBooze);
    RegisterItem (new Brush);
    Burnable::setup();
    RegisterItem (new ChangeFloorItem);
    RegisterItem (new Cherry);
    RegisterItem (new Coffee);
    Coin::setup();
    Crack::setup();
    RegisterItem (new Cross);
    RegisterItem (new Death);
    RegisterItem (new Debris);
    RegisterItem (new Document);
    RegisterItem (new Drop);
    RegisterItem (new Dummyitem);
    RegisterItem (new Dynamite);
    RegisterItem (new EasyKillStone);
    RegisterItem (new EasyKeepStone);
    RegisterItem (new Explosion1);
    RegisterItem (new Explosion2);
    RegisterItem (new Explosion3);
    Extinguisher::setup();
    RegisterItem (new ExtraLife(false));
    Register ("it_extralife_new", new ExtraLife(true));
    RegisterItem (new FlagBlack);
    RegisterItem (new FlagWhite);
    RegisterItem (new Floppy);
    RegisterItem (new Glasses);
    RegisterItem (new BrokenGlasses);
    RegisterItem (new Hammer(false));
    Register ("it_hammer_new", new Hammer(true));
    RegisterItem (new Hill);
    RegisterItem (new Hollow);
    RegisterItem (new HStrip);
    RegisterItem (new InvisibleAbyss);
    RegisterItem (new Key);
    RegisterItem (new Landmine);
    RegisterItem (new MagicWand);
    Register ("it_magnet", new Magnet(false));
    RegisterItem (new Magnet(true));
    RegisterItem (new Magnet(false));
    RegisterItem (new Odometer);
    RegisterItem (new OnePKillStone);
    RegisterItem (new OxydBridge);
    RegisterItem (new OxydBridgeActive);
    RegisterItem (new Pencil);
    RegisterItem (new Pin);
    Pipe::setup();
    Puller::setup();
    RegisterItem (new Ring);
    RegisterItem (new Rubberband);
    RegisterItem (new SeedWood);
    RegisterItem (new SeedNowood);
    RegisterItem (new SeedVolcano);
//    RegisterItem (new Sensor);
    Sensor::setup();
    ShogunDot::setup();
    SignalFilterItem::setup();
    RegisterItem (new Spade);
    RegisterItem (new Spoon);
    RegisterItem (new Spring1);
    RegisterItem (new Spring2);
    RegisterItem (new Springboard);
    RegisterItem (new Squashed);
    RegisterItem (new SurpriseItem);
    RegisterItem (new Sword(false));
    Register ("it_sword_new", new Sword(true));
    RegisterItem (new TinyHill);
    RegisterItem (new TinyHollow);
    RegisterItem (new Trigger);
    RegisterItem (new TwoPKillStone);
    RegisterItem (new Umbrella(false));
    Register ("it_umbrella_new", new Umbrella(true));
    Register ("it_vortex", new Vortex(true));
    RegisterItem (new Vortex(false));
    RegisterItem (new Vortex(true));
    RegisterItem (new VStrip);
    RegisterItem (new Weight);
    RegisterItem (new WhiteBomb);
    RegisterItem (new Wrench);
    RegisterItem (new WormHole(false));
    RegisterItem (new WormHole(true));
    Register ("it_wormhole", new WormHole(true));
    RegisterItem (new YinYang);
    RegisterItem (new Rubberband);
}

} // namespace enigma

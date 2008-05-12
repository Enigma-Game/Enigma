/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
#ifndef WORLD_HH_INCLUDED
#define WORLD_HH_INCLUDED

#include "enigma.hh"
#include "Object.hh"
#include "GridObject.hh"
#include "actors.hh"
#include "floors.hh"
#include "items.hh"
#include "stones.hh"
#include "util.hh"

namespace enigma {


/* -------------------- Types -------------------- */

    using ecl::V2;

    using std::string;

    /*! 
     * Impulse is used to transfer force from one Object to another
     * Object.  Currently only Stones can be the destination of
     * Impulses.
     */
    struct Impulse {
        // Variables
        Object    *sender;
        GridPos    dest;
        Direction  dir;

        // Constructors
        Impulse(Object *sender_, const GridPos& dest_, Direction dir_)
        : sender(sender_), dest(dest_), dir(dir_)
        {}
    };

    struct Message {
        // Variables
        std::string message;
        Value value;
        Object *sender;

        // Constructors
        Message ();
        Message (const std::string &message, const Value &value, Object *sender);
    };


    /*! The different kinds of materials objects in Enigma can be made
      of. */
    enum MaterialType {
        MATERIAL_GLASS,
        MATERIAL_METAL,
        MATERIAL_STONE,
    };

    /*!
     * This structure stores all the information that is necessary to
     * handle collisions between stones and actors. 
     */
    struct StoneContact
    {
        // Variables.
        Actor           *actor;
        GridPos         stonepos;
        StoneID         stoneid;
        StoneResponse   response;

        V2      contact_point;  // Where do the shapes meet? (world coordinates)
        V2      normal;         // The surface normal at the contact point
        DirectionBits faces;    // Faces hit on contact - may be two on edge contact
        bool    outerCorner;    // Did actor hit an outer corner of the stone
        bool    is_collision;   // Actor moves towards the stone, not away
        bool    ignore;         // Ignore this contact
        bool    new_collision;  // True if actor did not touch the stone before
        bool    is_contact;	// if false, contact_point is closest feature
        std::string sound;

        // Constructor.
//        StoneContact (Actor *a,
//                      GridPos stonepos,
//                      const V2 & contact_point,
//                      const V2 & surface_normal);
        StoneContact();
    };

    // Which of the four faces of the stone does the actor touch?
    Direction     contact_face (const StoneContact &sc);
    DirectionBits contact_faces (const StoneContact &sc);

/* -------------------- Force Fields -------------------- */

    class ForceField {
    public:
        virtual ~ForceField() {}
        virtual void add_force(Actor *a, V2 &force) = 0;
        virtual void tick(double /*dtime*/) {}
    };


//----------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------

    /* The global timer for all objects that need to be notified at
       regular intervals. */
    extern enigma::Timer GameTimer;

    /* Output a message whenever a message is being sent. */
    extern bool TrackMessages;

    /* A hack to implement BlackBallsStone and WhiteBallsStone. */
    extern Actor *CurrentCollisionActor;

//----------------------------------------------------------------------
// FUNCTIONS
//----------------------------------------------------------------------

/* -------------------- World Management -------------------- */

    void InitWorld();

    void WorldPrepareLevel();

    /* Create a new, empty world with width `w' and height `h`. */
    void Resize (int w, int h);
    int Width();
    int Height();

    /* Initialize the world after loading it.  Call this after loading
       the world to force laser beams to be recalculated etc. */
    bool WorldInitLevel();

    void WorldTick(double dtime);
    void TickFinished ();

    // Destroy all objects and the complete object repository
    void ShutdownWorld();


    bool IsLevelBorder(const GridPos &p);
    bool IsInsideLevel(const GridPos &p);
    Object *GetObject(const GridLoc &l);

/* -------------------- Named Objects -------------------- */

    void    NameObject (Object *obj, const string &name);
    void    UnnameObject (Object *obj);
    Object *GetNamedObject (const string &name);
    std::list<Object *> GetNamedGroup(const std::string &templ, const Object *reference = NULL);

/* -------------------- Force Fields -------------------- */

    void AddForceField (ForceField *ff);
    void RemoveForceField (ForceField *ff);
    void SetMouseForce (V2 f);

    void SetConstantForce (V2 force);


/* -------------------- Rubbers Bands -------------------- */

    struct RubberBandData {
	double strength;
	double length;
        double minlength;
        RubberBandData ();
        RubberBandData (const RubberBandData &);
    };

    struct Rubber_Band_Info {
	Actor *act;
        RubberBandData data;
    };
    typedef std::vector<Rubber_Band_Info> RBI_vector;


    /*! Add a rubber band that connects an actor with either a stone
      or another actor.  `strength' is the force constant, and
      `length' is the natural length of the elastic: if it is shorter
      than `length' it will exert no force on the actor(s). */
    void AddRubberBand (Actor *a, Stone *st, const RubberBandData &d);
    void AddRubberBand (Actor *a, Actor *a2, const RubberBandData &d);

    /*! Remove all rubber bands connected to `a'. */
    bool KillRubberBands (Actor *a);

    /*! Remove the rubber band between `a' and `st'.  If `st' is 0,
      all rubber bands connecting `a' to a stone will be cut. */
    void KillRubberBand (Actor *a, Stone *st);

    /*! Remove the rubber band between `a' and `a2'.  If `a2' is 0,
      all rubber bands connecting `a' to other actors will be cut. */
    void KillRubberBand (Actor *a, Actor *a2);

    /*! Remove all rubber bands attached to stone ST. */
    void KillRubberBands (Stone *st);

    /*! Fills given vector with basic info about rubbers attached to
      given stone */
    void GiveRubberBands (Stone *st, RBI_vector &rubbers);

    /*! Returns true if there already is a rubber band connecting `a'
      and `st'. */
    bool HasRubberBand (Actor *a, Stone *st);

/* -------------------- Meditation Control ---------------------- */

    void ChangeMeditation(int diffMeditatists, int diffIndispensableHollows,
            int diffEngagedIndispensableHollows, int diffEngagedDispensableHollows);

/* -------------------- Puzzle Stone Scrambling -------------------- */

    void SetScrambleIntensity(int intensity);
    void AddScramble(GridPos p, Direction d);



/* -------------------- Signals & Messages -------------------- */

    // just for Oxyd and old API compatibility
    void AddSignal (const GridLoc &src, const GridLoc &dst, const string &msg);

    void BroadcastMessage (const std::string& msg, const Value& value, 
                           GridLayerBits grids);

    Value SendMessage (Object *obj, const string &msg, const Value& value = Value(), Object *sender = NULL);
    Value SendMessage (Object *obj, const Message &m);



/* -------------------- Actors -------------------- */

    void   AddActor (Actor *a);
    void   AddActor (double x, double y, Actor* a);
    void   DidMoveActor (Actor *a);

    Actor *YieldActor(Actor *a);
    void   KillActor (Actor *a);

    void   WarpActor (Actor *a, double newx, double newy, bool keep_velocity);

    /*! Move `a' to its respawnpos immediately; do not run an
      animation like `RespawnActor(a)' would. */
    void   FastRespawnActor (Actor *a, bool keep_velocity); 

    /*! Like FastRespawnActor but marble 'appears' by running an
      animation. */
    void   RespawnActor (Actor *a); 
    
    /*! Find the marble of the other color (ac-whiteball <->
      ac-blackball). */
    Actor *FindOtherMarble (Actor *thisMarble);

    /*! Searches for other marble and exchanges their positions.
      Returns false if no other marble could be found. */
    bool   ExchangeMarbles (Actor *marble1); 

    void   GrabActor (Actor *a);
    void   ReleaseActor (Actor *a);

    unsigned CountActorsOfKind (ActorID id);

    /*! Find all actors at most RANGE units away from CENTER.  Returns
      false if none were found. */
    bool GetActorsInRange (ecl::V2 center, double range, std::vector<Actor*> &actors);

    /*! Find all actors that are inside 'pos'.  Returns false if none
      were found. */
    bool GetActorsInsideField (const GridPos& pos, std::vector<Actor*>& actors);

    /*! Shatter all actors inside 'pos'. */
    void ShatterActorsInsideField (const GridPos &pos);


/* -------------------- Field -------------------- */

    struct Field {
        // Variables
        Floor *floor;
        Item *item;
        Stone *stone;

        // Constructor and Destructor
	Field();
	~Field();
    };

    const Field *GetField (GridPos p);

/* -------------------- Stones -------------------- */

    void   SetStone(GridPos p, Stone* st);
    void   ReplaceStone(GridPos p, Stone *st);
    Stone *GetStone(GridPos p);
    Stone *YieldStone(GridPos p);
    void   KillStone(GridPos p);
    void   MoveStone(GridPos oldPos, GridPos newPos);

/* -------------------- Items -------------------- */

    void  SetItem (GridPos p, Item* it);
    void  SetItem (GridPos p, ItemID id);
    Item *GetItem (GridPos p);
    Item *YieldItem (GridPos p);
    void  KillItem (GridPos p);

/* -------------------- Floors -------------------- */

    void   SetFloor (GridPos p, Floor* st);
    Floor *GetFloor (GridPos p);
    void   KillFloor (GridPos p);


/* -------------------- Explosions -------------------- */

    enum ExplosionType { 
        EXPLOSION_DYNAMITE, 
        EXPLOSION_BLACKBOMB, 
        EXPLOSION_WHITEBOMB, 
        EXPLOSION_BOMBSTONE,
        EXPLOSION_SPITTER,
    };
    void SendExplosionEffect (GridPos p, ExplosionType type);

/* -------------------- Sound Events and Damping -------------------- */

    float getVolume(const char *name, Object *obj, float def_volume = 1.0);

/* -------------------- Creation/Definition of objects -------------------- */

    Object *MakeObject (const char *kind);
    Floor  *MakeFloor (const char *kind);
    Stone  *MakeStone (const char *kind);
    Stone  *MakeStone (StoneID id);
    Actor  *MakeActor (const char *kind);
    Actor  *MakeActor (ActorID id);

    void DisposeObject(Object *o);

    void DefineSimpleStone (const string &kind, const string &sound, 
                            int hollow, int glass);

    void DefineSimpleStoneMovable (const string &kind, const string &sound, 
                                   int glass);

    void DefineSimpleFloor(const string &kind, double friction,
                           double mousefactor, bool burnable,
                           const string &firetransform);

    /* Register a new object. */
    void BootRegister(Object *obj, const char * kind = NULL, bool isRegistration = true);
    void Register (Object *obj);
    void Register (const string &kind, Object *obj);
    void Register (const string &kind, Floor *obj);
    void Register (const string &kind, Stone *obj);
    void RegisterStone (Stone *st);
    void RegisterActor (Actor *ac);



    void Register (const string &kind, Item *obj);
    void RegisterItem (Item *it);
    Item   *MakeItem (const char *kind);
    Item   *MakeItem (ItemID id);

    /* Shutdown object repository */
    void Repos_Shutdown();

    Object *GetObjectTemplate(const string &kind);

    /* Print information about all registered objects to stdout. */
    void DumpObjectInfo();
    
} // namespace enigma

#endif

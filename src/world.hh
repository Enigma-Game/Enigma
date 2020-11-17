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
#include "server.hh"
#include "stones.hh"
#include "timer.hh"
#include "others/Other.hh"
#include "others/Rubberband.hh"

namespace enigma {

/* -------------------- Types -------------------- */

/*!
 * Impulse is used to transfer force from one Object to another
 * Object.  Currently only Stones can be the destination of
 * Impulses.
 */
struct Impulse {
    // Variables
    Object *sender;
    GridPos dest;
    Direction dir;
    bool byWire;

    // Constructors
    Impulse(Object *sender_, GridPos dest_, Direction dir_, bool sendByWire = false)
    : sender(sender_), dest(std::move(dest_)), dir(dir_), byWire(sendByWire) {}
};

struct Action {
    int senderId;
    bool isCallback;
    int targetId;      // target object id for message, flag expectFunction for callback
    std::string name;  // action name for message, function name for callback
    Value val;

    Action(int senderId_, bool isCallback_, int targetId_, std::string name_, Value val_)
    : senderId(senderId_), isCallback(isCallback_), targetId(targetId_), name(std::move(name_)), val(val_) {}
};

struct Message {
    // Variables
    std::string message;
    Value value;
    Object *sender;

    // Constructors
    Message();
    Message(std::string message, const Value &value, Object *sender);
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
struct StoneContact {
    // Variables.
    Actor *actor;
    GridPos stonepos;
    StoneID stoneid;
    StoneResponse response;

    ecl::V2 contact_point;  // Where do the shapes meet? (world coordinates)
    ecl::V2 normal;         // The surface normal at the contact point
    DirectionBits faces;    // Faces hit on contact - may be two on edge contact
    bool outerCorner;       // Did actor hit an outer corner of the stone
    bool is_collision;      // Actor moves towards the stone, not away
    bool ignore;            // Ignore this contact
    bool new_collision;     // True if actor did not touch the stone before
    bool is_contact;        // if false, contact_point is closest feature
    std::string sound;

    // Constructor.
    StoneContact();
};

// Which of the four faces of the stone does the actor touch?
Direction contact_face(const StoneContact &sc);
DirectionBits contact_faces(const StoneContact &sc);

/* -------------------- Force Fields -------------------- */

class ForceField {
public:
    virtual ~ForceField() {}
    virtual ecl::V2 globalForce(Actor *a) = 0;
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

//----------------------------------------------------------------------
// FUNCTIONS
//----------------------------------------------------------------------

/* -------------------- World Management -------------------- */

void InitWorld();

void WorldPrepareLevel();

/* Create a new, empty world with width `w' and height `h`. */
void Resize(int w, int h);
int Width();
int Height();

/* Initialize the world after loading it.  Call this after loading
   the world to force laser beams to be recalculated etc. */
bool WorldInitLevel();

void WorldTick(double dtime);
void TickFinished(double dtime);

// Destroy all objects and the complete object repository
void ShutdownWorld();

bool IsLevelBorder(const GridPos &p);
bool IsInsideLevel(const GridPos &p);
bool IsInsideLevel(const ecl::V2 &p);
Object *GetObject(const GridLoc &l);

/* -------------------- Named Objects -------------------- */

void NameObject(Object *obj, const std::string &name);
void UnnameObject(Object *obj);
Object *GetNamedObject(const std::string &name);
std::list<Object *> GetNamedGroup(const std::string &templ, Object *reference = nullptr);

/* -------------------- Named Positions -------------------- */

void NamePosition(Value po, const std::string &name);
Value GetNamedPosition(const std::string &name);
PositionList GetNamedPositionList(const std::string &templ, Object *reference = nullptr);

/* -------------------- Force Fields -------------------- */

void AddForceField(ForceField *ff);
void RemoveForceField(ForceField *ff);
void SetMouseForce(ecl::V2 f);

void SetGlobalForce(ecl::V2 force);
ecl::V2 GetGlobalForce();

/* -------------------- Other Objects -------------------- */

void AddOther(Other *o);
void KillOther(Other *o);

/* -------------------- Meditation Control ---------------------- */

void ChangeMeditation(int diffMeditatists, int diffIndispensableHollows,
                      int diffEngagedIndispensableHollows, int diffEngagedDispensableHollows);

/* -------------------- Puzzle Stone Scrambling -------------------- */

void SetScrambleIntensity(int intensity);
void AddScramble(GridPos p, Direction d);

/* -------------------- Signals & Messages -------------------- */

// just for Oxyd and old API compatibility
void AddSignal(const GridLoc &src, const GridLoc &dst, const std::string &msg);

void BroadcastMessage(const std::string &msg, const Value &value, GridLayerBits grids,
                      bool actors = false, bool others = false);

Value SendMessage(Object *obj, const std::string &msg, const Value &value = Value(),
                  Object *sender = nullptr);
Value SendMessage(Object *obj, const Message &m);

/* -------------------- Secure Delayed Actions -------------------- */

void PerformSecureAction(int senderId, bool isCallback, int targetId, std::string name, Value val);

/* -------------------- Actors -------------------- */

void AddActor(Actor *a);
void AddActor(double x, double y, Actor *a);
void DidMoveActor(Actor *a);

Actor *YieldActor(Actor *a);
void KillActor(Actor *a);

void WarpActor(Actor *a, double newx, double newy, bool keep_velocity);

/*! Move `a' to its respawnpos immediately; do not run an
  animation like `RespawnActor(a)' would. */
void FastRespawnActor(Actor *a, bool keep_velocity);

/*! Like FastRespawnActor but marble 'appears' by running an
  animation. */
void RespawnActor(Actor *a);

/*! Find the marble of the other color (ac-whiteball <->
  ac-blackball). */
Actor *FindOtherMarble(Actor *thisMarble);

/*! Searches for other marble and exchanges their positions.
  Returns false if no other marble could be found. */
bool ExchangeMarbles(Actor *marble1);

void GrabActor(Actor *a);
void ReleaseActor(Actor *a);

unsigned CountActorsOfKind(ActorID id);

/*! Find all actors at most RANGE units away from CENTER.  Returns
  false if none were found. */
bool GetActorsInRange(ecl::V2 center, double range, std::vector<Actor *> &actors);

/*! Find all actors that are inside 'pos'.  Returns false if none
  were found. */
bool GetActorsInsideField(const GridPos &pos, std::vector<Actor *> &actors);

/*! Shatter all actors inside 'pos'. */
void ShatterActorsInsideField(const GridPos &pos);

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

const Field *GetField(GridPos p);

/* -------------------- Stones -------------------- */

void SetStone(GridPos p, Stone *st);
void ReplaceStone(GridPos p, Stone *st);
Stone *GetStone(GridPos p);
Stone *YieldStone(GridPos p);
void KillStone(GridPos p);
void MoveStone(GridPos oldPos, GridPos newPos);
void TouchStone(GridPos pos);

/* -------------------- Items -------------------- */

void SetItem(GridPos p, Item *it);
Item *GetItem(GridPos p);
Item *YieldItem(GridPos p);
void KillItem(GridPos p);

/* -------------------- Floors -------------------- */

void SetFloor(GridPos p, Floor *st);
Floor *GetFloor(GridPos p);
void KillFloor(GridPos p);

/* -------------------- Explosions -------------------- */

enum ExplosionType {
    EXPLOSION_DYNAMITE,
    EXPLOSION_BLACKBOMB,
    EXPLOSION_WHITEBOMB,
    EXPLOSION_DISPENSER,
    EXPLOSION_SPITTER,
};
void SendExplosionEffect(GridPos p, ExplosionType type);

/* -------------------- Stone Contacts -------------------- */

Direction GetPushDirection (const StoneContact &sc);

/* -------------------- Sound Events and Damping -------------------- */

float GetVolume(const char *name, Object *obj, float def_volume = 1.0);

/* -------------------- Creation/Definition of objects -------------------- */

Object *MakeObject(const char *kind);
Floor *MakeFloor(const char *kind);
Item *MakeItem(const char *kind);
Stone *MakeStone(const char *kind);
Actor *MakeActor(const char *kind);

void DisposeObject(Object *o);

/* Register a new object. */
void BootRegister(Object *obj, const char *kind = nullptr, bool isRegistration = true);
void Register(const std::string &kind, Object *obj);

/* Shutdown object repository */
void Repos_Shutdown();

Object *GetObjectTemplate(const std::string &kind);

/* Print information about all registered objects to stdout. */
void DumpObjectInfo();

}  // namespace enigma

#endif

/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#include "game.hh"
#include "lua.hh"
#include "sound.hh"
#include "world.hh"

#include "ecl_util.hh"
#include "ecl_dict.hh"

#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <iomanip>


using namespace std;

namespace enigma {

// remove comment from define below to switch on verbose messaging
// note: VERBOSE_MESSAGES is defined in multiple source files!
// #define VERBOSE_MESSAGES

/* -------------------- Object implementation -------------------- */

int Object::next_id = 1;
std::map<int, Object *> Object::objects;

int Object::getNextId(Object *obj, bool bootFinished) {
    static bool isBooting = true;
    static int nextIdBoot = 1;
    static std::map<int, Object *> objectsBoot;
    
    if (isBooting) {
        if (bootFinished) {
            isBooting = false;
            next_id = nextIdBoot;
            objects = objectsBoot;
            return 0;
        } else {
            objectsBoot[nextIdBoot] = obj;
            return nextIdBoot++;
        }
    } else {
        objects[next_id] = obj;
        return next_id++;
    }
}

void Object::bootFinished() {
    getNextId(NULL, true);
}

void Object::freeId(int id) {
    objects.erase(id);
}

Object * Object::getObject(int id) {
    std::map<int, Object *>::iterator it = objects.find(id);
    if (it == objects.end())
        return NULL;
    else
        return it->second;
}

Object::Object() {
    id = getNextId(this, false);
}

Object::Object(const char *kind) {
    set_attrib("kind", Value(kind));
    id = getNextId(this, false);
}

Object::Object(const Object &src_obj) {
    id = getNextId(this, false);
    attribs = src_obj.attribs;
}

Object::~Object() {
    freeId(id);
//cerr << "obj del " << id << " - " << this->get_kind() <<"\n";
}

int Object::getId() const {
    return id;
}

Value Object::on_message (const Message &m)
{
    return message (m.message, m.value);
}

Value Object::message(const string& /*msg*/, const Value &/*val*/)
{
    return Value();
}

void Object::on_levelinit() {
}


const char * Object::get_kind() const {      // To be made pure virtual
    const Value *v = get_attrib("kind");
    ASSERT(v && v->getType() == Value::STRING, XLevelRuntime,
        "Object: attribute kind is not of type string (found in get_kind)");
    return v->get_string();
}

// check kind of object
// kind_templ may contain wildcards ( ? and * )
bool Object::is_kind(const char *kind_templ) const {
    return ecl::string_match(get_kind(), kind_templ);
}

bool Object::is_kind(const string& kind_templ) const {
    return ecl::string_match(get_kind(), kind_templ.c_str());
}

void Object::set_attrib(const string& key, const Value& val) {
    if (val)         // only set non-default values
        attribs[key] = val;  //.insert (key, val);
}

const Value* Object::get_attrib(const string& key) const { // To be delete as soon as
                                                           // get_kind() has no need of it
    AttribMap::const_iterator i = attribs.find(key);
    if (i == attribs.end())
        return 0;
    else
        return &i->second;
}

Value Object::getAttr(const string& key) const {
    AttribMap::const_iterator i = attribs.find(key);
    if (i == attribs.end())
        return Value(Value::DEFAULT);
    else
        return i->second;
}

Value Object::getDefaultedAttr(const string& key, Value defaultValue) const {
    if (Value v = getAttr(key))
        return v;
    else
        return defaultValue;
}

Value Object::getValue(const string& key) const {
    return getAttr(key);       // TODO write template method
}

Value Object::getDefaultValue(const string &key) const {
    return Value(Value::DEFAULT);
}

void Object::performAction (const Value& val) {
    TokenList targets = getAttr("target");
    TokenList actions = getAttr("action");
    if (Value state = getAttr("state")) {
        int s = state;
        if (Value stateTargets = getAttr(ecl::strf("target_%d", s)))
            targets = stateTargets;
        if (Value actionTargets = getAttr(ecl::strf("action_%d", s)))
            actions = actionTargets;
    }
    
    TokenList::iterator ait = actions.begin();
    std::string action;  // empty string as default
    for (TokenList::iterator tit = targets.begin(); tit != targets.end(); ++tit) {
        action = (ait != actions.end()) ? ait->to_string() : "";
        
        ObjectList ol = *tit;  // get all objects described by target token
        if (ol.size() == 0 || (ol.size() == 1 && ol.front() == NULL)) {  // no target object
            if ((action == "callback" || action == "") && (tit->getType() == Value::STRING) 
                    && lua::IsFunc(lua::LevelState(), tit->get_string())) {
                // it is an existing callback function
                if (lua::CallFunc(lua::LevelState(), tit->get_string(), val, this) != lua::NO_LUAERROR) {
                    throw XLevelRuntime(string("callback '") + tit->get_string() + "' failed:\n"+lua::LastError(lua::LevelState()));
                }
            }
            // else ignore this no longer valid target
//            Log << "PerformAction target not valid\n";
        } else {
            // send message to all objects
            if (action == "") 
                action = "toggle";
            for (ObjectList::iterator oit = ol.begin(); oit != ol.end(); ++oit) {
                if (*oit != NULL) {
                    if (GridObject *go = dynamic_cast<GridObject*>(this)) {
                        SendMessage(*oit, Message(action, val, go->get_pos()));
                    } else
                        SendMessage(*oit, Message(action, val));
                }
            }
        }
        
        if (ait != actions.end()) ++ait;
    }
}

/* Send an impulse to position 'dest' into direction dir.  If 'dest'
   contains a stone, on_impulse() is called for that stone */
void Object::send_impulse(const GridPos& dest, Direction dir) 
{
    if (Stone *st = GetStone(dest)) {
        Impulse impulse(this, dest, dir);
        st->on_impulse(impulse);
    }
}

/* Like variant above, but the _result_ of the impulse is delayed.
 */

void
Object::send_impulse(const GridPos& dest, Direction dir, double delay)
{
    if (Stone *st = GetStone(dest)) {
        addDelayedImpulse(Impulse(this, dest, dir), delay, st);
    }
}

void Object::warning(const char *format, ...) const {
    va_list arg_ptr;

    va_start(arg_ptr, format);

    fprintf(stderr, "%p non-grid-\"%s\": ", this, get_kind());
    vfprintf(stderr, format, arg_ptr);
    fputc('\n', stderr);

    va_end(arg_ptr);
}

Object::ObjectType Object::getObjectType() const {
    return OTHER;
}
/* -------------------- GridObject implementation -------------------- */

void GridObject::setOwner(int player) {
    ASSERT(pos.x < 0, XLevelRuntime,
        "GridObject: attempt to add object to owner inventory that is still on grid");
    pos.x = -1;
    pos.y = player;
}

Value GridObject::getOwner() {
    if (pos.x == -1 && pos.y != -1)
        return Value(pos.y);
    else
        return Value();
}

void GridObject::setOwnerPos(GridPos po) {
    ASSERT(pos.x < 0, XLevelRuntime,
        "GridObject: attempt to add object to owner inventory that is still on grid");
    if (po.x >= 0) {
        pos.x = -2 - po.x;
        pos.y = -2 - po.y;
    } else {
        pos.x = po.x;
        pos.y = po.y;
    }
}

GridPos GridObject::getOwnerPos() {
    if (pos.x <= -2)
        return GridPos(-2 - pos.x, -2 - pos.y);
    else 
        return pos;
}

display::Model *GridObject::set_anim (const std::string &mname) 
{
    set_model (mname);
    display::Model *m = get_model();
    m->set_callback(this);
    return m;
}

bool GridObject::sound_event (const char *name, double vol)
{
    return sound::EmitSoundEvent (name, get_pos().center(), getVolume(name, this, vol));
}

void GridObject::warning(const char *format, ...) const 
{
    va_list        arg_ptr;
    const GridPos& position = get_pos();

    va_start(arg_ptr, format);

    fprintf(stderr, "%p \"%s\" at %i/%i: ", this, get_kind(), position.x, position.y);
    vfprintf(stderr, format, arg_ptr);
    fputc('\n', stderr);

    va_end(arg_ptr);
}

} // namespace enigma

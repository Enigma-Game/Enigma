/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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

#include "Object.hh"

#include "errors.hh"
#include "game.hh"
#include "main.hh"
#include "lua.hh"
#include "ObjectValidator.hh"
#include "server.hh"
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
    
    Object::Object() : objFlags (0) {
        id = getNextId(this, false);
    }
    
    Object::Object(const char *kind) : objFlags (0) {
        setAttr("kind", Value(kind));
        id = getNextId(this, false);
    }
    
    Object::Object(const Object &src_obj) {
        id = getNextId(this, false);
        attribs = src_obj.attribs;
        objFlags = src_obj.objFlags;
    }
    
    Object::~Object() {
        freeId(id);
    //cerr << "obj del " << id << " - " << this->getKind() <<"\n";
    }
    
    int Object::getId() const {
        return id;
    }    
    
    std::string Object::getKind() const {
        return ObjectValidator::instance()->getKind(this);
    }
    
    bool Object::isKind(const std::string &kind) const {
        return ObjectValidator::instance()->isKind(this, kind);
    }

    bool Object::validateMessage(std::string msg, Value arg) {
        return ObjectValidator::instance()->validateMessage(this, msg, arg);
    }
    
    Value Object::message(const Message &m) {
        if (m.message == "_init") {
            // finalize nearest target and destination
            if (objFlags & OBJBIT_INIT)
                finalizeNearestObjectReferences();
        } else if (m.message == "kill") {
            switch (getObjectType()) {
                case OTHER:
                    KillOther(dynamic_cast<Other *>(this)); break;
                case FLOOR:
                    KillFloor(dynamic_cast<GridObject *>(this)->get_pos()); break;
                case ITEM:
                    KillItem(dynamic_cast<GridObject *>(this)->get_pos()); break;
                case STONE:
                    KillStone(dynamic_cast<GridObject *>(this)->get_pos()); break;
            }
        } else if (m.message == "disconnect") {
            bool wasConnected = false;
            ObjectList olist = getAttr("rubbers");   // a private deletion resistant copy
            for (ObjectList::iterator it = olist.begin(); it != olist.end(); ++it)
                KillOther(dynamic_cast<Other *>(*it));
            if (olist.size() > 0) {
                wasConnected = true;
                setAttr("rubber", Value());   // delete attribute
            }
            olist = getAttr("wires");   // a private deletion resistant copy
            for (ObjectList::iterator it = olist.begin(); it != olist.end(); ++it)
                KillOther(dynamic_cast<Other *>(*it));
            if (olist.size() > 0) {
                wasConnected = true;
                setAttr("wires", Value());   // delete attribute
            }
            return wasConnected;
        }
        return Value();
    }
    
    
    
    void Object::setAttr(const std::string& key, const Value& val) {
        if (key == "inverse") {
            if (val.to_bool())
                objFlags |= OBJBIT_INVERSE;
            else
                objFlags &= ~OBJBIT_INVERSE;
                
        } else if (key == "nopaction") {
            if (val.to_bool())
                objFlags |= OBJBIT_NOP;
            else
                objFlags &= ~OBJBIT_NOP;
            
        } else if (val) {        // only set non-default values
            if (val.getType() == Value::NIL /*&& server::EnigmaCompatibility >= 1.10*/)
                // delete attribute
                attribs.remove_key(key);
            else
                attribs[key] = val;  //.insert (key, val);
        }
    }
    
    
    void Object::setAttrChecked(const std::string& key, const Value &val) {
        // allow all user attributes and those system attributes with write allowance
        if (key == "name") {
            std::string oldName = getAttr("name").to_string();
            std::string newName = val.to_string();
            bool isRename =  oldName.size() > 0 && newName != oldName;
            
            // on name clash unname other object with same name and repair all fellow references
            Object *victim = GetNamedObject(newName);
            if (victim != NULL) {
                UnnameObject(victim);
                ObjectList olist = victim->getAttr("fellows");
                for (ObjectList::iterator itr = olist.begin(); itr != olist.end(); ++itr) {
                    ObjectList olist2 = (*itr)->getAttr("fellows");
                    olist2.remove(NULL);
                    olist2.push_back(victim);
                    (*itr)->setAttr("fellows", olist2);
                }
            }
            
            NameObject(this, val.to_string());
            
            // in case of a renaming repair all fellow reference that will now be NULL instead of this
            if (isRename) {
                ObjectList olist = getAttr("fellows");
                for (ObjectList::iterator itr = olist.begin(); itr != olist.end(); ++itr) {
                    ObjectList olist2 = (*itr)->getAttr("fellows");
                    olist2.remove(NULL);
                    olist2.push_back(this);
                    (*itr)->setAttr("fellows", olist2);
                }
            }
        } else {
            ValidationResult result = VALID_OK;
            if (key.find('_') != 0) 
                result = ObjectValidator::instance()->validateAttributeWrite(this, key, val);
            if (result == VALID_OK) {
                if (key == "destination" || key.find("target") == 0 || key.find("anchor") == 0)
                     if (val.maybeNearestObjectReference())
                         objFlags |= OBJBIT_INIT; 
                setAttr(key, val);
            } else {
                std::string reason;
                if (result == VALID_UNKNOWN_KEY) reason = "attribute name unknown";
                else if (result == VALID_ACCESS_DENIED) reason = "no write access allowed";
                else if (result == VALID_TYPE_MISMATCH) reason = "value typ mismatch";
                ASSERT(false, XLevelRuntime, ecl::strf("Object: attribute '%s' write failed for kind '%s', %s",
                        key.c_str(), getKind().c_str(), reason.c_str()).c_str());
            }
        }
    }
    
    Value Object::getAttrChecked(const std::string &key) const {
        // allow all user attributes and those system attributes with read allowance
        if (key.find('_') == 0 || ObjectValidator::instance()->validateAttributeRead(this, key))
            return getAttr(key);
        
        ASSERT(false, XLevelRuntime, ecl::strf("Object: attribute '%s' read not allowed for kind '%s'",
                key.c_str(), getKind().c_str()).c_str());
        return Value();
    }
    
    Value Object::getAttr(const string& key) const {
        if (key == "inverse") {
            return (objFlags & OBJBIT_INVERSE) != 0;            
        } else if (key == "nopaction") {
            return (objFlags & OBJBIT_NOP) != 0;
        } else if (key == "basename") {
            std::string name = getAttr("name").to_string();
            std::string::size_type p = name.rfind('%');
            if (p != std::string::npos)
                return name.substr(0, p);
            p = name.rfind('#');
            if (p != std::string::npos)
                return name.substr(0, p);
            return name;
        } else {
            AttribMap::const_iterator i = attribs.find(key);
            if (i == attribs.end()) 
    //            return Value(Value::DEFAULT);
                return ObjectValidator::instance()->getDefaultValue(this, key);
            else
                return i->second;
        }
    }
    
    Value Object::getDefaultedAttr(const string& key, Value defaultValue) const {
        if (Value v = getAttr(key))
            return v;
        else
            return defaultValue;
    }
    
    
    void Object::transferName(Object *target) {
        if (target == NULL)
            return;
        if (Value v = getAttr("name")) {
            std::string name(v);
            UnnameObject(this);
            if (Value old = target->getAttr("name")) {
                std::string oldname(old);
                if ( oldname.size() > 0 && oldname[0] != '$' && name.size() > 0 && name[0] != '$' )
                    target->warning("name '%s' overwritten by '%s'", oldname.c_str(), name.c_str());
                UnnameObject(target);
            }
            NameObject(target, name);
        }
    }
    
    void Object::transferIdentity(Object *target) {
        if (target == NULL)
            return;
        transferName(target);
        for (AttribMap::iterator it = attribs.begin(); it != attribs.end(); ++it) {
            // copy user attributes starting with "_" and
            // target and actions including their stated versions
            if ( it->first.find("_") == 0 || it->first.find("target") == 0
                    || it->first.find("action") == 0) {
                target->setAttr(it->first, it->second);
//                Log << "Identity transfer key " << it->first << " - value " << it->second.to_string() << "\n";
            }
        }
    }
    
    void Object::performAction(const Value& val) {
        Value messageValue = val;
        if (objFlags & OBJBIT_INVERSE)
            messageValue = invertActionValue(val);
        
        if (server::EnigmaCompatibility < 1.10) {
            messageValue = messageValue.to_bool() ? 1 : 0; 
        }

        TokenList targets = getAttr("target");
        TokenList actions = getAttr("action");
        if (Value state = getAttr("state")) {
            int s = state;
            if (Value stateTargets = getAttr(ecl::strf("target_%d", s))) {
                targets = stateTargets;
            }
            if (Value actionTargets = getAttr(ecl::strf("action_%d", s)))
                actions = actionTargets;
        }
        
        TokenList::iterator ait = actions.begin();
        std::string action;  // empty string as default
        bool secure = getAttr("safeaction").to_bool();
        for (TokenList::iterator tit = targets.begin(); tit != targets.end(); ++tit) {
            action = (ait != actions.end()) ? ait->to_string() : "";
            
            ObjectList ol = (*tit).getObjectList(this);  // get all or nearest objects described by target token
            if (ol.size() == 0 || (ol.size() == 1 && ol.front() == NULL)) {  // no target object
                if ((action == "callback" || action.empty()) && (tit->getType() == Value::STRING)) { 
//                        && lua::IsFunc(lua::LevelState(), tit->get_string())) {
                    // it is an existing callback function
                    if (secure) {
                        PerformSecureAction(this->getId(), true, !action.empty(), tit->get_string(), messageValue);
                    } else {
                        if (lua::CallFunc(lua::LevelState(), tit->get_string(), messageValue, this, !action.empty()) != lua::NO_LUAERROR) {
                            throw XLevelRuntime(std::string("callback '") + tit->get_string() + "' failed:\n"+lua::LastError(lua::LevelState()));
                        }
                    }
                }
                // else ignore this no longer valid target
//                Log << "PerformAction target not valid\n";
            } else {
                // send message to all objects
                if (action == "" )
                    action = (objFlags & OBJBIT_NOP) ? "nop" : "toggle";
                for (ObjectList::iterator oit = ol.begin(); oit != ol.end(); ++oit) {
                    if (*oit != NULL) {
                        std::string obj_action = action;
                        if (server::EnigmaCompatibility < 1.10) {
                            // we may need to translate the new API message to old API
                            obj_action = lua::NewMessageName(lua::LevelState(), *oit, action);
//                            if (obj_action != action)
//                                Log << "PerformAction renamed '" << action << "' to '" << obj_action << "' for receiver '" << (*oit)->getKind() << "'\n";
                        }
                        // check if message is valid, otherwise ignore message
                        if (obj_action != "nop" && (*oit)->validateMessage(obj_action, messageValue))
                            if (secure)
                                PerformSecureAction(this->getId(), false, (*oit)->getId(), obj_action, messageValue);
                            else
                                SendMessage(*oit, Message(obj_action, messageValue, this));                    
                    }
                }
            }
            
            if (ait != actions.end()) ++ait;
        }
    }
    
    Value Object::invertActionValue(const Value &val) const {
        return !val.to_bool();  // invert and convert value to bool
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
    
    
    bool Object::getDestinationByIndex(int idx, ecl::V2 &dstpos) {
        int i = 0;  // counter for destination candidates
        TokenList tl = getAttr("destination");  // expand any tokens to a list of values
        for (TokenList::iterator tit = tl.begin(); tit != tl.end(); ++tit) {
            PositionList pl = (*tit).getPositionList(this);  // convert next token to a list of positions
            for (PositionList::iterator pit = pl.begin(); pit != pl.end(); ++pit) {
                ecl::V2 pos = (*pit).centeredPos();
                if (IsInsideLevel(pos))  {  // no positions in inventory,..
                    if (i == idx) {
                        dstpos = pos;
                        return true;
                    } else
                        i++;
                }
            }
        }        
        return false;
    }
    
    void Object::finalizeNearestObjectReferences(std::string attr) {
        bool modified = false;
        TokenList targets = getAttr(attr);
        
        for (TokenList::iterator tit = targets.begin(); tit != targets.end(); ++tit) {
            modified |= (*tit).finalizeNearestObjectReference(this);
        }
        if (modified) {
            if (attr.find("anchor") == 0) {
                setAttr(attr, targets.front());  // do not store as token to allow direct object access
            } else
                setAttr(attr, targets);
        }
    }
    
    void Object::finalizeNearestObjectReferences() {
        finalizeNearestObjectReferences("target");
        finalizeNearestObjectReferences("destination");
        finalizeNearestObjectReferences("anchor1");
        finalizeNearestObjectReferences("anchor2");
        int min = getAttr("$minState");
        int max = getAttr("$maxState");
        for (int i = min; i <= max; i++) {
            finalizeNearestObjectReferences(ecl::strf("target_%d", i));
        }
    }
    
    void Object::warning(const char *format, ...) const {
        va_list arg_ptr;
    
        va_start(arg_ptr, format);
    
        fprintf(stderr, "%p non-grid-\"%s\": ", this, getKind().c_str());
        vfprintf(stderr, format, arg_ptr);
        fputc('\n', stderr);
    
        va_end(arg_ptr);
    }
    
    Object::ObjectType Object::getObjectType() const {
        return OBJECT;
    }
    
    double Object::squareDistance(const Object *other) const {
        return 1e15;   // inifinity in enigma world
    }
    
    bool Object::isSouthOrEastOf(const Object *other) const {
        return true;
    }
    
} // namespace enigma

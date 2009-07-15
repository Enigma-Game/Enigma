/*
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

#include "others/Rubberband.hh"
#include "actors.hh"
#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    Rubberband::Rubberband() {
    }
    
    std::string Rubberband::getClass() const {
        return "ot_rubberband";
    }

    void Rubberband::setAttr(const std::string &key, const Value &val) {
        if (key == "anchor1") {
            Actor *old = anchor1;
            anchor1 = dynamic_cast<Actor *>((Object *)val);
            ASSERT(anchor1 != NULL, XLevelRuntime, "Rubberband: 'anchor1' is no actor");
            ASSERT(anchor1 != anchor2.ac, XLevelRuntime, "Rubberband: 'anchor1' is identical to 'anchor2'");
            switchAnchor(old, anchor1, anchor2Object());
        } else if (key == "anchor2") {
            Object * old = anchor2Object();
            Object * obj = val;
            if (obj != NULL && obj->getObjectType() == Object::ACTOR) {
                anchor2.ac = dynamic_cast<Actor *>((Object *)val);
                ASSERT(anchor1 != anchor2.ac, XLevelRuntime, "Rubberband: 'anchor1' is identical to 'anchor2'");
                objFlags &= ~OBJBIT_STONE;
                switchAnchor(old, anchor2.ac, anchor1);
            } else if (obj != NULL && obj->getObjectType() == Object::STONE) {
                anchor2.st = dynamic_cast<Stone *>((Object *)val);
                objFlags |= OBJBIT_STONE;
                switchAnchor(old, anchor2.st, anchor1);
            } else
                ASSERT(false, XLevelRuntime, "Rubberband: 'anchor2' is neither actor nor stone");
        } else if (key == "strength") {
            strength = val;
        } else if (key == "length") {
            outerThreshold = val;
            ASSERT(outerThreshold >= 0, XLevelRuntime, "Rubberband: length is negative");
        } else if (key == "threshold") {
            innerThreshold = val;
            ASSERT(innerThreshold >= 0, XLevelRuntime, "Rubberband: inner threshold is negative");
        } else if (key == "max") {
            maxLength = val;
            ASSERT((maxLength >= 0) && (maxLength == 0 || maxLength >= minLength), XLevelRuntime, "Rubberband: max length is negative or less min");
        } else if (key == "min") {
            minLength = val;
            ASSERT((minLength >= 0) && (maxLength == 0 || maxLength >= minLength), XLevelRuntime, "Rubberband: min length is negative or greater max");
        }
        Other::setAttr(key, val);
    }
    
    Value Rubberband::getAttr(const std::string &key) const {
        if (key == "anchor1") {
            return anchor1;
        } else if (key == "anchor2") {
            return anchor2Object();
        } else if (key == "strength") {
            return strength;
        } else if (key == "length") {
            return outerThreshold;
        } else if (key == "threshold") {
            return innerThreshold;
        } else if (key == "max") {
            return maxLength;
        } else if (key == "min") {
            return minLength;
        }
        return Other::getAttr(key);
    }
    
    Value Rubberband::message(const Message &m) {
        if (m.message == "_recheck") {
            ecl::V2 v = posAnchor2() - anchor1->get_pos();
            double len = ecl::length(v);
            bool violating = false;
            if (maxLength > 0 && len > maxLength) {
                objFlags |= OBJBIT_MAXVIOLATION;
                violating = true;
            } else if (len < minLength) {
                objFlags |= OBJBIT_MINVIOLATION;
                violating = true;
            }
            if (violating) {
                performAction(false);
            }
            return Value(); 
        } else if (m.message == "_performaction") {
            performAction(true);
            return Value();             
        }
        return Other::message(m);
    }

    void Rubberband::postAddition() {
        ASSERT(anchor1 != NULL, XLevelRuntime, "Rubberband: 'anchor1' is no actor");
        ASSERT(anchor2.ac != NULL, XLevelRuntime, "Rubberband: 'anchor2' is neither actor nor stone");
        model = display::AddRubber(anchor1->get_pos(), posAnchor2(), 240, 140, 20, true);  // orange
        SendMessage(this, "_recheck");
    }
    
    void Rubberband::preRemoval() {
        model.kill();
        switchAnchor(anchor1, NULL, anchor2Object());
        switchAnchor(anchor2Object(), NULL, anchor1);        
    }
    
    void Rubberband::tick(double dt) {
        model.update_first(anchor1->get_pos());
        model.update_second(posAnchor2());
    }
    
    void Rubberband::applyForces(double dt) {
        const double eps = 0.02;  // epsilon distant limit for contacts
        ecl::V2 v = posAnchor2() - anchor1->get_pos();
        double len = ecl::length(v);
        ecl::V2 force;
        
        // revalidate pending max/min violations
        if ((objFlags & OBJBIT_MAXVIOLATION) && len <= maxLength)
            objFlags &= ~OBJBIT_MAXVIOLATION;
        if ((objFlags & OBJBIT_MINVIOLATION) && len >= minLength)
            objFlags &= ~OBJBIT_MINVIOLATION;
        
        if (minLength + eps <= len && (len <= maxLength -eps || maxLength == 0)) {
            // length within the purly force controlled min/max limited region
            if (len == 0) {
                force = V2(0, 0);
            } else if (len > outerThreshold) {
                force = v * strength * (len - outerThreshold)/len;
            } else if (len < innerThreshold) {
                force = v * strength * (len - innerThreshold)/len;
            }
        
            ActorInfo *ai = anchor1->get_actorinfo();
            ai->force += force;
            if (!(objFlags & OBJBIT_STONE)) {
                ai = anchor2.ac->get_actorinfo();
                ai->force -= force;
            }
                
        } else if (objFlags & OBJBIT_STONE) {
            // min/max handling for stone contected rubberbands
            ActorInfo *ai = anchor1->get_actorinfo();
            ecl::V2 vn = normalize(v);
            bool isMax = (len > maxLength - eps);
            bool isMin = (len < minLength + eps);
            ObjectList rl = anchor1->getAttr("rubbers");
            int numRubbers =rl.size();

            // neutralize other force componentes in rubber direction
            double force1 = vn * ai->force;
            if ((!isMin && (force1 > 0)) || (!isMax && (force1 < 0)))
                force1 = 0;
            ai->force -= force1 * vn;
            
            double relspeed = ai->vel * vn;   // positive for shrinking dist
            if (!isMin && (relspeed > 0) || !isMax && (relspeed < 0))
                relspeed = 0;
            force = - (1 + 0.8 / numRubbers) * relspeed * vn / dt * ai->mass;  // damping for inverse friction and multiconnections
//            Log << "Rubber stone force "<< force1 << "  " <<relspeed<< "\n";
            
            // in case one actor is blocked the length can exceed the limits due to later force corrections
            // in the last timestep - we need to correct possible small errors before they sum up
            if (isMax && (len > maxLength) && (relspeed <= 0) && !(objFlags & OBJBIT_MAXVIOLATION)) {
                double dlen = ecl::Min(len - maxLength, len - (minLength + eps));
                dlen = ecl::Max(0.0, dlen);
                force = (ai->mass * dlen / dt / dt) * vn;
            }
            if (isMin && (len < minLength) && (relspeed >= 0) && !(objFlags & OBJBIT_MINVIOLATION)) {
                double dlen = len - minLength;
                if (maxLength > 0) {
                    dlen = ecl::Max(len - minLength, len - (maxLength - eps));
                    dlen = ecl::Min(0.0, dlen);
                }
                force = (ai->mass * dlen / dt / dt) * vn;
            }
            
            // eliminate limit violations by moderate forces
            if (isMax && (objFlags & OBJBIT_MAXVIOLATION)) {
                force += server::RubberViolationStrength * vn;
            } else if (isMin && (objFlags & OBJBIT_MINVIOLATION)) {
                force -= server::RubberViolationStrength * vn;
            }
            
            ai->collforce += force;
        } else {
            // two actors bouncing on min/max limits
            ActorInfo *ai1 = anchor1->get_actorinfo();
            ActorInfo *ai2 = anchor2.ac->get_actorinfo();
            ecl::V2 vn = normalize(v);
            double mass = ai1->mass + ai2->mass;
            bool isMax = (len > maxLength - eps);
            bool isMin = (len < minLength + eps);
            bool isBoth = isMax && isMin;
            if (isBoth) {
                isMax = (len > (maxLength - minLength)/2);
                isMin = !isMax; 
            }
            ObjectList rl1 = anchor1->getAttr("rubbers");
            ObjectList rl2 = anchor2.ac->getAttr("rubbers");
            int numRubbers = rl1.size() + rl2.size() - 1;
            
            // redistribute other force components in rubber direction according 
            // to the mass of actors to move the complex but to avoid length change
            
            // component of other forces in rubber direction
            double force1 = vn * ai1->force;
            double force2 = vn * ai2->force;
            
            // limit to min/max affected forces
            if ((!isMin && (force1 > 0)) || (!isMax && (force1 < 0)))
                force1 = 0;
            if ((!isMin && (force2 < 0)) || (!isMax && (force2 > 0)))
                force2 = 0;
            ai1->force += (-force1 + (force1 + force2) * (ai1->mass)/mass) * vn;
            ai2->force += (-force2 + (force1 + force2) * (ai2->mass)/mass) * vn;
            
            // bounce if min/max rules are violated
            double relspeed = vn * (ai2->vel - ai1->vel);  // speed of band extension
            double dmu = 2 * ai1->mass * ai2->mass / (ai1->mass + ai2->mass);

            if (isMax && (relspeed < 0) || isMin && (relspeed >0))
                relspeed = 0;
                
            force = (dmu * relspeed / dt) * vn;
            force = force * (0.5 + 0.4 / numRubbers);   // damping for inverse friction and multicollision

            // in case one actor is blocked the length can exceed the limits due to later force corrections
            // in the last timestep - we need to correct possible small errors before they sum up
            if (isMax && (len > maxLength) && (relspeed >= 0) && !(objFlags & OBJBIT_MAXVIOLATION)) {
                double dlen = ecl::Min(len - maxLength, len - (minLength + eps));
                dlen = ecl::Max(0.0, dlen);
                force = (dmu * dlen / dt / dt) * vn;
            }
            if (isMin && (len < minLength) && (relspeed <= 0) && !(objFlags & OBJBIT_MINVIOLATION)) {
                double dlen = len - minLength;
                if (maxLength > 0) {
                    dlen = ecl::Max(len - minLength, len - (maxLength - eps));
                    dlen = ecl::Min(0.0, dlen);
                }
                force = (dmu * dlen / dt / dt) * vn;
            }
            
            // eliminate limit violations by moderate forces
            if (isMax && (objFlags & OBJBIT_MAXVIOLATION)) {
                force += server::RubberViolationStrength * vn;
            } else if (isMax && (objFlags & OBJBIT_MINVIOLATION)) {
                force -= server::RubberViolationStrength * vn;
            }

//            Log << "Rubber force " << force1 <<  "  " << force2 << "  relspeed  " << relspeed  << " both " << isBoth << "\n";
            ai1->collforce += force;
            ai2->collforce -= force;
        }

    }
    
    Object * Rubberband::anchor2Object() const {
        return (objFlags & OBJBIT_STONE) ? (Object *)anchor2.st : (Object *)anchor2.ac;
    }
    
    ecl::V2 Rubberband::posAnchor2() const {
        return (objFlags & OBJBIT_STONE) ? anchor2.st->getOwnerPos().center() : anchor2.ac->get_pos();
    }
    
    void Rubberband::switchAnchor(Object *oldAnchor, Object *newAnchor, Object *otherAnchor) {
        if (oldAnchor != NULL) {
            ObjectList olist = oldAnchor->getAttr("rubbers");
            olist.remove(this);
            oldAnchor->setAttr("rubbers", olist);
            if (otherAnchor != NULL) {
                // remove both anchors from each others fellows list
                olist = oldAnchor->getAttr("fellows");
                ObjectList::iterator it = find(olist.begin(), olist.end(), otherAnchor);
                if (it != olist.end()) {
                    olist.erase(it);
                }
                oldAnchor->setAttr("fellows", olist);
                olist = otherAnchor->getAttr("fellows");
                it = find(olist.begin(), olist.end(), oldAnchor);
                if (it != olist.end()) {
                    olist.erase(it);
                }
                otherAnchor->setAttr("fellows", olist);
            }            
        }
        if (newAnchor != NULL) {
            ObjectList olist = newAnchor->getAttr("rubbers");
            olist.push_back(this);
            newAnchor->setAttr("rubbers", olist);
            if (otherAnchor != NULL) {
                // check on existing rubberbands between anchors
                olist = newAnchor->getAttr("fellows");
                ObjectList::iterator it = find(olist.begin(), olist.end(), otherAnchor);
                if (it != olist.end()) {
                    // we do not allow two rubberbands between identical anchors!
                    // - the user can't see it
                    // - danger of automatic addition of infinte rubberbands, that cause the engine to stop
                    // - danger of contradicting min, max values
                    olist = newAnchor->getAttr("rubbers");
                    for (it = olist.begin(); it != olist.end(); ++it) {
                        Rubberband *oldRubber = dynamic_cast<Rubberband *>(*it);
                        if (otherAnchor == oldRubber->anchor1 || otherAnchor == oldRubber->anchor2Object()) {
                            KillOther(oldRubber);
                            break;
                        }
                    }
                }
                
                // add both anchors to each others fellows list
                olist = newAnchor->getAttr("fellows");
                olist.push_back(otherAnchor);
                newAnchor->setAttr("fellows", olist);
                olist = otherAnchor->getAttr("fellows");
                olist.push_back(newAnchor);
                otherAnchor->setAttr("fellows", olist);
            }
        }
    }

    BOOT_REGISTER_START
        BootRegister(new Rubberband(), "ot_rubberband");
    BOOT_REGISTER_END

} // namespace enigma

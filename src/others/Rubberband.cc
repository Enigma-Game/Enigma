/*
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

#include "others/Rubberband.hh"
#include "actors.hh"
#include "errors.hh"
#include "main.hh"
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
            switchAnchor(old, anchor1, (objFlags & OBJBIT_STONE) ? (Object *)anchor2.st : (Object *)anchor2.ac);
        } else if (key == "anchor2") {
            Object * old = (objFlags & OBJBIT_STONE) ? (Object *)anchor2.st : (Object *)anchor2.ac;
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
            ASSERT(maxLength >= 0, XLevelRuntime, "Rubberband: max length is negative");
        } else if (key == "min") {
            minLength = val;
            ASSERT(minLength >= 0, XLevelRuntime, "Rubberband: min length is negative");
        }
        Other::setAttr(key, val);
    }
    
    Value Rubberband::getAttr(const std::string &key) const {
        if (key == "anchor1") {
            return anchor1;
        } else if (key == "anchor2") {
            return (objFlags & OBJBIT_STONE) ? (Object *)anchor2.st : (Object *)anchor2.ac;
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
    
    void Rubberband::postAddition() {
        model = display::AddRubber(anchor1->get_pos(), posAnchor2(), 240, 140, 20);  // orange
    }
    
    void Rubberband::preRemoval() {
        model.kill();
        switchAnchor(anchor1, NULL, (objFlags & OBJBIT_STONE) ? (Object *)anchor2.st : (Object *)anchor2.ac);
        switchAnchor((objFlags & OBJBIT_STONE) ? (Object *)anchor2.st : (Object *)anchor2.ac, NULL, anchor1);        
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
        
        if (minLength + eps <= len && (len <= maxLength -eps || maxLength == 0)) {
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
            ActorInfo *ai = anchor1->get_actorinfo();
            ecl::V2 vn = normalize(v);
            bool isMax = (len > maxLength - eps);
            bool isMin = (len < minLength + eps);

            // neutralize other force componentes in rubber direction
            double force1 = vn * ai->force;
            if ((!isMin && (force1 > 0)) || (!isMax && (force1 < 0)))
                force1 = 0;
            ai->force -= force1 * vn;
            
            double relspeed = ai->vel * vn;   // positive for shrinking dist
            if (!isMin && (relspeed > 0) || !isMax && (relspeed < 0))
                relspeed = 0;
            force = - 1.8 * relspeed * vn / dt * ai->mass;  // 0.9 factor as damping
            ai->collforce += force;
//            Log << "Rubber stone force "<< force1 << "  " <<relspeed<< "\n";
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
            force = force * 0.9;   // damping for wrong friction calculation
//            Log << "Rubber force " << force1 <<  "  " << force2 << "  relspeed  " << relspeed  << " both " << isBoth << "\n";
            ai1->collforce += force;
            ai2->collforce -= force;
        }

    }
    
    ecl::V2 Rubberband::posAnchor2() {
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

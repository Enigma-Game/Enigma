/*
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
#ifndef STATEOBJECT_HH
#define STATEOBJECT_HH

#include "Object.hh"

namespace enigma {

    /** 
     * The base class for all gaming objects that have different states that
     * can be toggled by means of messages, actions or attribute settings.
     * This class is the successor of the former OnOffStone and OnOffItem.
     * Being a superclass of all gaming objects it provides a common interface
     * for the handling of states. The state is not limited to 2 states like
     * the common on/off and open/close. Arbitrary number of external states 
     * are supported. E.g. FourSwitches have 4 states that describe the current
     * direction.<p>
     * But the external state - the logical gaming state used in level lua code -
     * is often just a small subset of states that an object has to distinguish.
     * Additional display related states, timer related states and internal
     * gaming logic states require sometimes a much more complex state machine.
     * <p>
     * The maximum set of states that we call the internal state of a final
     * gaming object class should be stored in StateObjects state ivar. It is
     * the responsibility of StateObject to guarantee the persistence and
     * cloning of the internal state.<p>
     * StateObject provides the common interface for state management with hooks
     * for all needs like conversion of external to and from internal states,
     * common messages like toggle, signal, on, off, open, close and checked
     * setter and getter for the state as a pseudo attribute.<p>
     * Furtheron its default implementation suits the needs a simple 2 state
     * on/off or open/close object. More complex stated objects will need to
     * override the hook methods to their needs.
     */
    class StateObject : public Object {
    public:
        StateObject();
        StateObject(const char * kind);
        
        // Object Interface
        
        /**
         * Handle the messages "toggle", "signal", "on", "off", "open", "close".
         * Any subclass should forward these messages to its superclass without
         * interfering these messages. Per default these messages will be blocked
         * by SendMessage() if they are not explicitly declared for the final
         * subclass in objects.xml.
         */
        virtual Value message(const Message &m);
        
        /**
         * Handle the attribute "state" by a pseudo implementation. Subclasses
         * shall not interfere with this attribute but override the method
         * "externalState()" on demand.
         */
        virtual Value getAttr(const string &key) const;
        
        /**
         * Handle the attribute "state" by a pseudo implementation. Subclasses
         * shall not interfere with this attribute but override the method
         * "setState()". New state values are checked against min and max. 
         */
        virtual void setAttr(const string& key, const Value &val);
        
    protected:
        /**
         * The storage location of the internal state. This ivar is logically
         * owned by the final discrete object class which will define in most
         * cases an enum called iState that lists all states. All intermediate
         * classes between StateObject and the state owning subclass will just
         * be aware of the external state that they access via the setter and
         * getter methods. Just these two methods in their StateObject default
         * implementation will directly access this ivar (besides the gaming
         * logic indepent persistence and cloning methods). 
         */
        int state;
        
        /**
         * The maximum number of the external state. It defaults to 1, and 
         * should be overridden by the final subclass just on demand. 
         */
        virtual int maxState() const;
        
        /**
         * The minimum number of the external state. It defaults to 0, and
         * should be overridden by the final subclass just on demand.
         */
        virtual int minState() const;
        
        /**
         * The method that is called on toggle messages to switch the object
         * to the next external state. The default implementation is a round
         * robin of the external state from minState() to maxState() and returning
         * to minState() again. Override this method if the toggle operation
         * changes the states in another sequence..
         */
        virtual void toggleState();
        
        /**
         * Access method that translates the internal state stored in "state" to
         * the external gaming logic state. The default implementation is a
         * direct return of the state which suits a one to one mapping between
         * internal and external states. If additional internal states or another
         * mapping is used this method needs to be overwritten. All access to
         * the external state by any other intermediate class has to use this
         * method.
         */
        virtual int externalState() const;
        
        /**
         * Setter method for external state that switches the object to the
         * appropriate internal state. The default implementation is a direct
         * storage of the external state as the internal state. Most final
         * subclasses will have to override this method to cause some actions,
         * to send messages and to switch the display model.
         */
        virtual void setState(int extState);
    };

} // namespace enigma

#endif

//======================================================================
// Copyright (C) 2002 Daniel Heck
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
//======================================================================
#ifndef PX_CALLBACK_HH
#define PX_CALLBACK_HH

namespace px
{
    class Callback {
    public:
        virtual ~Callback() {}
        virtual void operator() () = 0;
    };

    template <class T>
    class MethodCallback : public Callback {
    public:
        typedef void (T::* Func)();
        MethodCallback(T* o, Func f) : obj(o), func(f) {}
        void operator() () { (obj->*func)(); }
    private:
        T *obj;
        Func func;
    };

    class FunctionCallback : public Callback {
    public:
        typedef void (*Func)();
        FunctionCallback(Func f) : func(f) {}
        void operator() () { func(); }
    private:
        Func func;
    };
}

namespace px
{
    inline FunctionCallback *
    make_cb(void (*func)()) 
    {
        return new FunctionCallback(func);
    }

    template <class T> Callback *
    make_cb(T *o, void (T::* f)())
    {
        return new MethodCallback<T>(o, f);
    }
}

#endif

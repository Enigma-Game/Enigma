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

#include "stones/YieldingStone.hh"
#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {
    YieldingStone::YieldingStone() : Stone(), yieldedStone (NULL), yieldedModel (NULL) {
    }
    
    void YieldingStone::dispose() {
         if (yieldedStone != NULL) {
            SendMessage(yieldedStone, "disconnect");
            DisposeObject(yieldedStone);
         }
         if (yieldedModel != NULL)
            delete yieldedModel;
         yieldedStone = NULL;
         yieldedModel = NULL;
         delete this;
    }

    void YieldingStone::yieldStone(Stone *st) {
        if (st != NULL) {
            ASSERT(yieldedStone == NULL, XLevelRuntime, "YieldingStone: internal error of double yielding"); 
            yieldedStone = st;
            GridPos pos = st->get_pos();
            yieldedModel = display::YieldModel(GridLoc(GRID_STONES, pos));
            YieldStone(pos);
            yieldedStone->setOwnerPos(get_pos());   // the stone id owned at the new position
        }
    }
    
    void YieldingStone::setStone() {
        GridPos pos = get_pos();
        SendMessage(this, "disconnect");
        YieldStone(pos);
        if (yieldedStone != NULL) {
            int id = yieldedStone->getId();
            SetStone(pos, yieldedStone);
            if (Object::getObject(id) != NULL) { // not killed?
                display::SetModel(GridLoc(GRID_STONES, pos), yieldedModel);
                yieldedStone->on_move();    // continue animations -- this is buggy if the stone has another
                                            // model on the new position like st-chameleon
                if (Object::getObject(id) != NULL)   // not killed?
                    SendMessage(yieldedStone, "_model_reanimated");  // temp fix: reset bad models
            }
            yieldedStone = NULL;
            yieldedModel = NULL;
        }
        DisposeObject(this);
    }

} // namespace enigma

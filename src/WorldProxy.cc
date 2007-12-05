/*
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

#include "WorldProxy.hh"
#include "server.hh"
#include "display.hh"

namespace enigma {
 
    WorldProxy *WorldProxy::theSingleton = 0;
    
    WorldProxy* WorldProxy::instance() {
        if (theSingleton == 0) {
            theSingleton = new WorldProxy();
        }
        return theSingleton;
    }
    
    void WorldProxy::shutdown() {
        delete theSingleton;
    }
    
    WorldProxy::WorldProxy() {
    }

    Value WorldProxy::getAttr(const string& key) const {
        if (key == "AllowSingleOxyds") {
            return server::AllowSingleOxyds;
        } else if (key == "AllowTogglePlayer") {
            return server::AllowTogglePlayer;
        } else if (key == "ConserveLevel") {
            return server::ConserveLevel;
        } else if (key == "CreatingPreview") {
            return server::CreatingPreview;  // read only
        } else if (key == "IsDifficult") {
            return (server::GetDifficulty() == DIFFICULTY_HARD);  // read only
        } else if (key == "LevelTime") {   
            return server::LevelTime;    // read only
        } else if (key == "ShowMoves") {
            return server::ShowMoves;
        } else if (key == "ShowMoves") {
            return server::ShowMoves;
        } else if (key == "Brittleness") {
            return server::Brittleness;
        } else if (key == "BumperForce") {
            return server::BumperForce;
        } else if (key == "ElectricForce") {
            return server::ElectricForce;
        } else if (key == "FlatForce") {
            return server::FlatForce;
        } else if (key == "FrictionFactor") {
            return server::FrictionFactor;
        } else if (key == "HoleForce") {
            return server::HoleForce;
        } else if (key == "IceFriction") {
            return server::IceFriction;
        } else if (key == "MagnetForce") {
            return server::MagnetForce;
        } else if (key == "MagnetRange") {
            return server::MagnetRange;
        } else if (key == "SlopeForce") {
            return server::SlopeForce;
        } else if (key == "SwampSinkSpeed") {
            return server::SwampSinkSpeed;
        } else if (key == "WaterSinkSpeed") {
            return server::WaterSinkSpeed;
        } else if (key == "WormholeForce") {
            return server::WormholeForce;
        } else if (key == "WormholeRange") {
            return server::WormholeRange;
        }
        return Value(Value::DEFAULT);
    }  

    void WorldProxy::set_attrib(const string& key, const Value &val) {
        if (key == "AllowSingleOxyds") {
            server::AllowSingleOxyds = val;
        } else if (key == "AllowTogglePlayer") {
            server::AllowTogglePlayer = val;
        } else if (key == "ConserveLevel") {
            server::ConserveLevel = val.to_bool();
        } else if (key == "FollowMode") {
            display::SetFollowMode((display::FollowMode)((int)val));
        } else if (key == "ShowMoves") {
            server::ShowMoves = val.to_bool();
            STATUSBAR->show_move_counter (server::ShowMoves);
        } else if (key == "Brittleness") {
            server::Brittleness = val;
        } else if (key == "BumperForce") {
            server::BumperForce = val;
        } else if (key == "ElectricForce") {
            server::ElectricForce = val;
        } else if (key == "FlatForce") {
            server::FlatForce = val;
        } else if (key == "FrictionFactor") {
            server::FrictionFactor = val;
        } else if (key == "HoleForce") {
            server::HoleForce = val;
        } else if (key == "IceFriction") {
            server::IceFriction = val;
        } else if (key == "MagnetForce") {
            server::MagnetForce = val;
        } else if (key == "MagnetRange") {
            server::MagnetRange = val;
        } else if (key == "SlopeForce") {
            server::SlopeForce = val;
        } else if (key == "SwampSinkSpeed") {
            server::SwampSinkSpeed = val;
        } else if (key == "WaterSinkSpeed") {
            server::WaterSinkSpeed = val;
        } else if (key == "WormholeForce") {
            server::WormholeForce = val;
        } else if (key == "WormholeRange") {
            server::WormholeRange = val;
        }
    }

} // namespace enigma

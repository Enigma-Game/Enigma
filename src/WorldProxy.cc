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
#include "display.hh"
#include "errors.hh"
#include "server.hh"
#include "world.hh"
#include "stones/OxydStone.hh"

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
        } else if (key == "FollowAction") {
            return server::FollowAction;
        } else if (key == "FollowGrid") {
            return server::FollowGrid;
        } else if (key == "FollowMethod") {
            return server::FollowMethod;
//        } else if (key == "FollowRoomSize") {    // Enigma 1.2 
        } else if (key == "FollowThreshold") {
            return server::FollowThreshold;            
        } else if (key == "Height") {
            return Height();
        } else if (key == "IsDifficult") {
            return (server::GetDifficulty() == DIFFICULTY_HARD);  // read only
        } else if (key == "LevelTime") {   
            return server::LevelTime;    // read only
        } else if (key == "ProvideExtralifes") {
            return server::ProvideExtralifes;
        } else if (key == "ShowMoves") {
            return server::ShowMoves;
        } else if (key == "SurviveFinish") {
            return server::SurviveFinish;
        } else if (key == "Width") {
            return Width();
        } else if (key == "Brittleness") {
            return server::Brittleness;
        } else if (key == "ActorimpulseStrength") {
            return server::BumperForce;
        } else if (key == "ElectricForce") {
            return server::ElectricForce;
        } else if (key == "ExtralifeGlasses") {
            return server::ExtralifeGlasses;
        } else if (key == "FrictionFactor") {
            return server::FrictionFactor;
        } else if (key == "MeditationStrength") {
            return server::HoleForce;
        } else if (key == "MagnetStrength") {
            return server::MagnetForce;
        } else if (key == "MagnetRange") {
            return server::MagnetRange;
        } else if (key == "MaxOxydColor") {
            return server::MaxOxydColor;
        } else if (key == "RubberViolationStrength") {
            return server::RubberViolationStrength;
        } else if (key == "SlopeForce") {
            return server::SlopeForce;
        } else if (key == "SwampSinkSpeed") {
            return server::SwampSinkSpeed;
        } else if (key == "WaterSinkSpeed") {
            return server::WaterSinkSpeed;
        } else if (key == "WormholeStrength") {
            return server::WormholeForce;
        } else if (key == "WormholeRange") {
            return server::WormholeRange;
        }
        return Value(Value::DEFAULT);
    }  

    void WorldProxy::setAttr(const string& key, const Value &val) {
        if (key == "AllowSingleOxyds") {
            server::AllowSingleOxyds = val;
        } else if (key == "AllowTogglePlayer") {
            server::AllowTogglePlayer = val;
        } else if (key == "ConserveLevel") {
            server::ConserveLevel = val.to_bool();
        } else if (key == "FollowAction") {
            server::FollowAction = val;
            display::UpdateFollowMode();
        } else if (key == "FollowGrid") {
            bool wasGrid = server::FollowGrid;
            server::FollowGrid = val.to_bool();
            if (wasGrid && !server::FollowGrid) {
                server::FollowMethod = display::FOLLOW_SCROLL;
                server::FollowThreshold = 0;
                server::FollowAction = 0;
            } else if (!wasGrid && server::FollowGrid) {
                server::FollowThreshold = 0.5;
                server::FollowAction = ecl::V2(9.5, 6);
            }
            display::UpdateFollowMode();
        } else if (key == "FollowMethod") {
            int oldMethod = server::FollowMethod;
            server::FollowMethod = val;
            if ( oldMethod != server::FollowMethod) {
                if (server::FollowMethod == display::FOLLOW_FLIP) {
                    server::FollowThreshold = 0.5;
                    server::FollowAction = ecl::V2(19, 12);
                } else if (server::FollowMethod == display::FOLLOW_SCROLL &&  server::FollowGrid) {
                    server::FollowThreshold = 0.5;
                    server::FollowAction = ecl::V2(9.5, 6);
                }
                display::UpdateFollowMode();
            }
//        } else if (key == "FollowRoomSize") {    // Enigma 1.2 
        } else if (key == "FollowThreshold") {
            server::FollowThreshold = val;
            display::UpdateFollowMode();
        } else if (key == "ProvideExtralifes") {
            bool newFlag = val.to_bool();
            ASSERT(server::ProvideExtralifes || !newFlag, XLevelRuntime, 
                "Attribute 'ProvideExtralifes': toggling from false to true not allowed");
            server::ProvideExtralifes = newFlag;
            
        } else if (key == "ShowMoves") {
            server::ShowMoves = val.to_bool();
            STATUSBAR->show_move_counter (server::ShowMoves);
        } else if (key == "SurviveFinish") {
            server::SurviveFinish = val.to_bool();
        } else if (key == "Brittleness") {
            server::Brittleness = val;
        } else if (key == "ActorimpulseStrength") {
            server::BumperForce = val;
        } else if (key == "ElectricForce") {
            server::ElectricForce = val;
        } else if (key == "ExtralifeGlasses") {
            server::ExtralifeGlasses = val;
        } else if (key == "FrictionFactor") {
            server::FrictionFactor = val;
        } else if (key == "MeditationStrength") {
            server::HoleForce = val;
        } else if (key == "MagnetStrength") {
            server::MagnetForce = val;
            BroadcastMessage("_updateglobals", "it_magnet", GRID_ITEMS_BIT);
        } else if (key == "MagnetRange") {
            server::MagnetRange = val;
            BroadcastMessage("_updateglobals", "it_magnet", GRID_ITEMS_BIT);
        } else if (key == "MaxOxydColor") {
            server::MaxOxydColor = val;
            ASSERT(server::MaxOxydColor >= 0 && server::MaxOxydColor < OxydStone::COLORCOUNT, XLevelRuntime,
                    "Attribute 'MaxOxydColor' value is out of range.");
        } else if (key == "RubberViolationStrength") {
            server::RubberViolationStrength = val;
        } else if (key == "SlopeForce") {
            server::SlopeForce = val;
        } else if (key == "SwampSinkSpeed") {
            server::SwampSinkSpeed = val;
        } else if (key == "WaterSinkSpeed") {
            server::WaterSinkSpeed = val;
        } else if (key == "WormholeStrength") {
            server::WormholeForce = val;
            BroadcastMessage("_updateglobals", "it_wormhole", GRID_ITEMS_BIT);
        } else if (key == "WormholeRange") {
            server::WormholeRange = val;
            BroadcastMessage("_updateglobals", "it_wormhole", GRID_ITEMS_BIT);
        }
    }

} // namespace enigma

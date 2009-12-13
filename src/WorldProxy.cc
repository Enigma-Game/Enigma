/*
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

    std::string WorldProxy::getClass() const {
        return "WorldProxy";
    }
    
    Value WorldProxy::getAttr(const std::string &key) const {
        if (key == "AllowSingleOxyds") {
            return server::AllowSingleOxyds;
        } else if (key == "AllowSuicide") {
            return server::AllowSuicide;
        } else if (key == "AutoRespawn") {
            return server::AutoRespawn;
        } else if (key == "TogglePlayerOnDeath") {
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
            return server::IsDifficult;  // read only
        } else if (key == "LevelTime") {   
            return server::LevelTime;    // read only
        } else if (key == "ProvideExtralifes") {
            return server::ProvideExtralifes;
        } else if (key == "InfiniteReincarnation") {
            return server::InfiniteReincarnation;
        } else if (key == "ShowMoves") {
            return server::ShowMoves;
        } else if (key == "SublevelNumber") {
            return (int)server::SublevelNumber;
        } else if (key == "SublevelTitle") {
            return server::SublevelTitle;
        } else if (key == "SurviveFinish") {
            return server::SurviveFinish;
        } else if (key == "Width") {
            return Width();
        } else if (key == "ActorimpulseStrength") {
            return server::BumperForce;
        } else if (key == "CrackSpreading") {
            return server::CrackSpreading;
        } else if (key == "ElectricStrength") {
            return server::ElectricForce;
        } else if (key == "ExtralifeGlasses") {
            return server::ExtralifeGlasses;
        } else if (key == "FallenPuzzle") {
            return server::FallenPuzzle;
        } else if (key == "Fragility") {
            return server::Fragility;
        } else if (key == "FrictionStrength") {
            return server::FrictionFactor;
        } else if (key == "GlobalForce") {
            return server::ConstantForce;
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
        } else if (key == "SlopeStrength") {
            return server::SlopeForce;
        } else if (key == "SubSoil") {
            return server::SubSoil;
        } else if (key == "SwampSinkTime") {
            if (server::SwampSinkSpeed > 0) {
                if (server::SwampSinkSpeed >= 10000)
                    return 0;
                else
                    return 7.0 / server::SwampSinkSpeed;
            } else
                return Value();  // infinite time
        } else if (key == "WaterSinkTime") {
            if (server::WaterSinkSpeed > 0) {
                if (server::WaterSinkSpeed >= 10000)
                    return 0;
                else
                    return 7.0 / server::WaterSinkSpeed;
            } else
                return Value();  // infinite time
        } else if (key == "WormholeStrength") {
            return server::WormholeForce;
        } else if (key == "WormholeRange") {
            return server::WormholeRange;
        }
        return Value(Value::DEFAULT);
    }  

    void WorldProxy::setAttr(const std::string &key, const Value &val) {
        if (key == "AllowSingleOxyds") {
            server::AllowSingleOxyds = val.to_bool();
        } else if (key == "AllowSuicide") {
            server::AllowSuicide = val.to_bool();
        } else if (key == "AutoRespawn") {
            server::AutoRespawn = val.to_bool();
        } else if (key == "TogglePlayerOnDeath") {
            server::AllowTogglePlayer = val.to_bool();
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
        } else if (key == "InfiniteReincarnation") {
            server::InfiniteReincarnation = val.to_bool();            
        } else if (key == "ShowMoves") {
            server::ShowMoves = val.to_bool();
            STATUSBAR->show_move_counter (server::ShowMoves);
        } else if (key == "SublevelTitle") {
            server::SublevelTitle = val.to_string();
        } else if (key == "SurviveFinish") {
            server::SurviveFinish = val.to_bool();
        } else if (key == "CrackSpreading") {
            server::CrackSpreading = val;
        } else if (key == "ActorimpulseStrength") {
            server::BumperForce = val;
        } else if (key == "ElectricStrength") {
            server::ElectricForce = val;
        } else if (key == "ExtralifeGlasses") {
            server::ExtralifeGlasses = val;
        } else if (key == "FallenPuzzle") {
            server::FallenPuzzle = val.to_string();
        } else if (key == "Fragility") {
            server::Fragility = val;
        } else if (key == "FrictionStrength") {
            server::FrictionFactor = val;
        } else if (key == "GlobalForce") {
            server::ConstantForce = val;
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
        } else if (key == "SlopeStrength") {
            server::SlopeForce = val;
        } else if (key == "SubSoil") {
            server::SubSoil = val;
            ASSERT(server::SubSoil >= 0 && server::SubSoil <= 2, XLevelRuntime,
                    "Attribute 'SubSoil' value is out of range.");
        } else if (key == "SwampSinkTime") {
            if (val.getType() == Value::NIL)
                server::SwampSinkSpeed = 0;
            else if ((double)val == 0)
                server::SwampSinkSpeed = 10000;
            else {
                ASSERT((double)val > 0, XLevelRuntime, "SwampSinkTime less zero");
                server::SwampSinkSpeed = 7.0 / (double)val;
            }
        } else if (key == "WaterSinkTime") {
            if (val.getType() == Value::NIL)
                server::WaterSinkSpeed = 0;
            else if ((double)val == 0)
                server::WaterSinkSpeed = 10000;
            else {
                ASSERT((double)val > 0, XLevelRuntime, "SwampSinkTime less zero");
                server::WaterSinkSpeed = 7.0 / (double)val;
            }
        } else if (key == "WormholeStrength") {
            server::WormholeForce = val;
            BroadcastMessage("_updateglobals", "it_wormhole", GRID_ITEMS_BIT);
        } else if (key == "WormholeRange") {
            server::WormholeRange = val;
            BroadcastMessage("_updateglobals", "it_wormhole", GRID_ITEMS_BIT);
        } else
            ASSERT(false, XLevelRuntime, ecl::strf("Attempt to set not existing global attribute '%s'.", key.c_str()).c_str());
        
    }

} // namespace enigma

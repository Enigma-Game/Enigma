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

#include "stones/ClusterStone.hh"
//#include "main.hh"

namespace enigma {
    ClusterStone::ClusterStone(std::string classname, std::string connections) : Stone() {
        setAttr("connections", connections);
        setAttr("$class", classname);
    }
    
    std::string ClusterStone::getClass() const {
        return getAttr("$class").to_string();
    }
    
    void ClusterStone::setAttr(const string& key, const Value &val) {
        if (key == "connections" || key == "faces" || key == "cluster") {
            Stone::setAttr(key, val);
            if (isDisplayable()) {
                autoLeaveCluster();
                autoJoinCluster();
            }
            return;
        }
        Stone::setAttr(key, val);
    }
    
    Value ClusterStone::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            init_model();
        }
        return Stone::message(m);
    }

    void ClusterStone::on_creation(GridPos p) {
        autoJoinCluster();
        Stone::on_creation(p);
    }
    
    void ClusterStone::on_removal(GridPos p) {
        autoLeaveCluster();
        Stone::on_removal(p);
    }
    
    std::string ClusterStone::getModelName() const {
        return getClass();
    }

    bool ClusterStone::is_removable() const {
        return getFaces() == ALL_DIRECTIONS;
    }
    
    void ClusterStone::autoJoinCluster() {
        GridPos p = get_pos();
        Value myCluster = getAttr("cluster");
        for (int i = WEST; i <= NORTH; i++) {
            Direction d = (Direction) i;
            Stone *neighbour = GetStone(move(p, d));
            if (neighbour != NULL && std::string(neighbour->getClass()) == getClass()) {
                Value neighbourCluster = neighbour->getAttr("cluster");
                if (myCluster) {
                    if (myCluster == neighbourCluster) {
                        setAttr("$connections", getConnections() | to_bits(d));
                        neighbour->setAttr("$connections", neighbour->getConnections() | to_bits(reverse(d)));
                    } else if (!neighbourCluster && neighbour->getConnections() & to_bits(reverse(d))) {
                        setAttr("$connections", getConnections() | to_bits(d));
                    } else {
                        setAttr("$connections", getConnections() & (ALL_DIRECTIONS ^ to_bits(d))); // clear connection
                    }
                } else if (neighbourCluster)  {// I have fixed connections -> adapt neighbour
                    if (getConnections() & to_bits(d))
                        neighbour->setAttr("$connections", neighbour->getConnections() | to_bits(reverse(d)));
                    else
                        neighbour->setAttr("$connections", neighbour->getConnections() & (ALL_DIRECTIONS ^ to_bits(reverse(d))));
                }
            } else if (myCluster) { // no neighbour -> no connection
                setAttr("$connections", getConnections() & (ALL_DIRECTIONS ^ to_bits(d))); // clear connection
            }
        }
    }
    
    void ClusterStone::autoLeaveCluster() {
        GridPos p = get_pos();
        for (int i = WEST; i <= NORTH; i++) {
            Direction d = (Direction) i;
            Stone *neighbour = GetStone(move(p, d));
            if (neighbour != NULL && std::string(neighbour->getClass()) == getClass() 
                    && neighbour->getAttr("cluster")) {
                neighbour->setAttr("$connections", neighbour->getConnections() & (ALL_DIRECTIONS ^ to_bits(reverse(d))));
            }
        }
    }
    
    BOOT_REGISTER_START
        BootRegister(new ClusterStone("st_panel", ""), "st_panel");
        BootRegister(new ClusterStone("st_panel", "w"), "st_panel_w");
        BootRegister(new ClusterStone("st_panel", "s"), "st_panel_s");
        BootRegister(new ClusterStone("st_panel", "sw"), "st_panel_sw");
        BootRegister(new ClusterStone("st_panel", "e"), "st_panel_e");
        BootRegister(new ClusterStone("st_panel", "ew"), "st_panel_ew");
        BootRegister(new ClusterStone("st_panel", "es"), "st_panel_es");
        BootRegister(new ClusterStone("st_panel", "esw"), "st_panel_esw");
        BootRegister(new ClusterStone("st_panel", "n"), "st_panel_n");
        BootRegister(new ClusterStone("st_panel", "nw"), "st_panel_nw");
        BootRegister(new ClusterStone("st_panel", "ns"), "st_panel_ns");
        BootRegister(new ClusterStone("st_panel", "nsw"), "st_panel_nsw");
        BootRegister(new ClusterStone("st_panel", "ne"), "st_panel_ne");
        BootRegister(new ClusterStone("st_panel", "new"), "st_panel_new");
        BootRegister(new ClusterStone("st_panel", "nes"), "st_panel_nes");
        BootRegister(new ClusterStone("st_panel", "nesw"), "st_panel_nesw");

        BootRegister(new ClusterStone("st_brick", ""), "st_brick");
        BootRegister(new ClusterStone("st_brick", "w"), "st_brick_w");
        BootRegister(new ClusterStone("st_brick", "s"), "st_brick_s");
        BootRegister(new ClusterStone("st_brick", "sw"), "st_brick_sw");
        BootRegister(new ClusterStone("st_brick", "e"), "st_brick_e");
        BootRegister(new ClusterStone("st_brick", "ew"), "st_brick_ew");
        BootRegister(new ClusterStone("st_brick", "es"), "st_brick_es");
        BootRegister(new ClusterStone("st_brick", "esw"), "st_brick_esw");
        BootRegister(new ClusterStone("st_brick", "n"), "st_brick_n");
        BootRegister(new ClusterStone("st_brick", "nw"), "st_brick_nw");
        BootRegister(new ClusterStone("st_brick", "ns"), "st_brick_ns");
        BootRegister(new ClusterStone("st_brick", "nsw"), "st_brick_nsw");
        BootRegister(new ClusterStone("st_brick", "ne"), "st_brick_ne");
        BootRegister(new ClusterStone("st_brick", "new"), "st_brick_new");
        BootRegister(new ClusterStone("st_brick", "nes"), "st_brick_nes");
        BootRegister(new ClusterStone("st_brick", "nesw"), "st_brick_nesw");
        
        BootRegister(new ClusterStone("st_bluesand", ""), "st_bluesand");
        BootRegister(new ClusterStone("st_bluesand", "w"), "st_bluesand_w");
        BootRegister(new ClusterStone("st_bluesand", "s"), "st_bluesand_s");
        BootRegister(new ClusterStone("st_bluesand", "sw"), "st_bluesand_sw");
        BootRegister(new ClusterStone("st_bluesand", "e"), "st_bluesand_e");
        BootRegister(new ClusterStone("st_bluesand", "ew"), "st_bluesand_ew");
        BootRegister(new ClusterStone("st_bluesand", "es"), "st_bluesand_es");
        BootRegister(new ClusterStone("st_bluesand", "esw"), "st_bluesand_esw");
        BootRegister(new ClusterStone("st_bluesand", "n"), "st_bluesand_n");
        BootRegister(new ClusterStone("st_bluesand", "nw"), "st_bluesand_nw");
        BootRegister(new ClusterStone("st_bluesand", "ns"), "st_bluesand_ns");
        BootRegister(new ClusterStone("st_bluesand", "nsw"), "st_bluesand_nsw");
        BootRegister(new ClusterStone("st_bluesand", "ne"), "st_bluesand_ne");
        BootRegister(new ClusterStone("st_bluesand", "new"), "st_bluesand_new");
        BootRegister(new ClusterStone("st_bluesand", "nes"), "st_bluesand_nes");
        BootRegister(new ClusterStone("st_bluesand", "nesw"), "st_bluesand_nesw");

    BOOT_REGISTER_END

} // namespace enigma

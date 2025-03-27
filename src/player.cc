/*
 * Copyright (C) 2002,2003,2004,2006 Daniel Heck
 * Copyright (C) 2007,2008,2009,2010 Ronald Lamprecht
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
#include "player.hh"
#include "Inventory.hh"
#include "display.hh"
#include "errors.hh"
#include "SoundEffectManager.hh"
#include "client.hh"
#include "server.hh"
#include "world.hh"
#include "main.hh"
#include "items/GlassesItem.hh"

#include "ecl_util.hh"

using namespace std;
using namespace enigma;
using enigma::Actor;
using enigma::Inventory;

namespace {

class PlayerInfo {
public:
    PlayerInfo();

    string name;
    Inventory inventory;
    vector<Actor *> actors;
    bool out_of_lives;
    double dead_dtime;  // number of seconds the player is already dead
    bool inhibit_pickup;
};

struct RespawnInfo {
    RespawnInfo(Actor *a, double t) : actor(a), time_left(t) {}

    Actor *actor;      // The actor to respawn
    double time_left;  // Time left before respawning
};

struct LevelLocalData {
    // Functions
    LevelLocalData() {}

    void respawn_dead_actors(double dtime);
    void resurrect_actor(Actor *a);
    bool remove_extralife(Actor *a);
    void reset();

    // Variables
    vector<RespawnInfo> respawn_list;
};

}  // namespace

/* -------------------- PlayerInfo -------------------- */

PlayerInfo::PlayerInfo()
: name(), inventory(), actors(), out_of_lives(false), dead_dtime(0), inhibit_pickup(false) {
}

/* -------------------- LevelLocalData -------------------- */

void LevelLocalData::respawn_dead_actors(double dtime) {
    for (unsigned i = 0; i < respawn_list.size();) {
        RespawnInfo &info = respawn_list[i];

        info.time_left -= dtime;
        if (info.time_left < 0) {
            info.actor->respawn();
            respawn_list.erase(respawn_list.begin() + i);
            continue;  // don't increment i
        }
        ++i;
    }
}

void LevelLocalData::resurrect_actor(Actor *a) {
    const double RESPAWN_TIME = 1.5;

    SendMessage(a, "_resurrect");
    if (!server::InfiniteReincarnation)
        remove_extralife(a);
    respawn_list.push_back(RespawnInfo(a, RESPAWN_TIME));
}

bool LevelLocalData::remove_extralife(Actor *a) {
    Inventory *inv = player::GetInventory(a);
    int idx = inv->find("it_extralife");

    if (idx == -1)  // no extralife found
        return false;
    else {
        delete inv->yield_item(idx);
        player::RedrawInventory(inv);
        return true;
    }
}

void LevelLocalData::reset() {
    respawn_list.clear();
}

/* -------------------- Local variables -------------------- */

namespace {

LevelLocalData leveldat;
std::vector<PlayerInfo> players(2);  // this currently has always size 2
unsigned icurrent_player = 0;
std::vector<Actor *> unassignedActors;

}  // namespace

void player::PlayerShutdown() {
    players.clear();  // release objects of inventories
}

/* -------------------- Functions -------------------- */

void player::NewGame() {
    int nplayers = 2;  // Always prepare for two players
    std::vector<int> extralives(2);

    // calculate number of extralives
    for (int i = 0; i < nplayers; ++i) {
        if (server::IsLevelRestart) {
            if (server::ProvideExtralifes) {
                // count existing number of extralives
                int idxLife = -1;
                extralives[i] = -1;
                do {
                    ++extralives[i];
                    idxLife = players[i].inventory.find("it_extralife", ++idxLife);
                } while (idxLife != -1);
            } else
                extralives[i] = 0;
        } else {
            // new game provides 2 extralives
            extralives[i] = (server::ProvideExtralifes) ? 2 : 0;
        }
    }

    players.clear();
    players.resize(nplayers);

    for (int i = 0; i < nplayers; ++i) {
        Inventory *inv = GetInventory(i);
        inv->assignOwner(i);
        for (int j = 0; j < extralives[i]; j++)
            inv->add_item(MakeItem("it_extralife"));
    }

    unassignedActors.clear();
    leveldat.reset();
}

void player::AddYinYang() {
    for (unsigned i = 0; i < players.size(); ++i) {
        Inventory *inv = GetInventory(i);
        if (inv->find("it_yinyang") == -1)
            inv->add_item(MakeItem("it_yinyang"));
    }
}

void player::LevelLoaded(bool isRestart) {
    if (server::TwoPlayerGame && server::SingleComputerGame) {
        AddYinYang();
    }
    if (server::ProvideYinYang) {
        AddYinYang();
    }
    RedrawInventory();
}

void player::PrepareLevel() {
    // Clear up the inventories of all players: keep only extra lifes.
    for (unsigned iplayer = 0; iplayer < players.size(); ++iplayer) {
        Inventory *inv = GetInventory(iplayer);
        int nextralifes = 0;
        for (size_t i = 0; i < inv->size(); ++i)
            if (get_id(inv->get_item(i)) == it_extralife)
                nextralifes += 1;
        inv->clear();
        for (int i = 0; i < nextralifes; ++i)
            inv->add_item(MakeItem("it_extralife"));

        players[iplayer].actors.clear();
    }

    SetCurrentPlayer(0);
    leveldat = LevelLocalData();
}

void player::LevelFinished(int stage) {
    for (auto &player : players) {
        for (Actor *a : player.actors) {
            if (stage == 0) {
                SendMessage(a, "_levelfinish");
            } else {
                SendMessage(a, "_disappear");
                SendMessage(a, "disconnect");
            }
        }
    }
}

Inventory *player::GetInventory(int iplayer) {
    return &players[iplayer].inventory;
}

Inventory *player::GetInventory(Actor *a) {
    if (Value v = a->getAttr("owner"))
        return GetInventory((int)v);
    return nullptr;
}

bool player::WieldedItemIs(Actor *a, const string &kind) {
    if (Inventory *inv = GetInventory(a))
        if (Item *it = inv->get_item(0))
            return it->isKind(kind);
    return false;
}

int player::CurrentPlayer() {
    return icurrent_player;
}

void player::SetCurrentPlayer(unsigned iplayer) {
    if (iplayer >= players.size())
        Log << ecl::strf("SetCurrentPlayer: no such player %d\n", iplayer);
    else {
        icurrent_player = iplayer;
        RedrawInventory(GetInventory(iplayer));
        Glasses::updateGlasses();
    }
}

unsigned player::NumberOfRealPlayers() {
    unsigned real_players = 0;

    for (auto &player : players) {
        if (!player.actors.empty()) {
            ++real_players;
        }
    }

    return real_players;
}

/*! Sets respawn positions for black or white actors. */
void player::SetRespawnPositions(GridPos pos, Value color) {
    ecl::V2 center = pos.center();

    for (auto &player : players) {
        for (auto &actor : player.actors) {
            if (Value ac = actor->getAttr("color")) {
                if (ac == color)
                    actor->set_respawnpos(center);
            }
        }
    }
}

/*! Remove respawn positions for black or white actors */
void player::RemoveRespawnPositions(Value color) {
    for (auto &player : players) {
        for (auto &actor : player.actors) {
            if (Value ac = actor->getAttr("color")) {
                if (ac == color)
                    actor->remove_respawnpos();
            }
        }
    }
}

void player::Suicide() {
    for (auto &player : players) {
        for (auto &actor : player.actors) {
            SendMessage(actor, "_suicide");
        }
    }
}

Actor *player::ReplaceActor(unsigned iplayer, Actor *old, Actor *a) {
    if (iplayer >= players.size())
        server::RaiseError("Invalid actor number");

    for (auto &actor : players[iplayer].actors) {
        if (actor == old) {
            actor = a;
            return old;
        }
    }
    return nullptr;
}

void player::AddActor(unsigned iplayer, Actor *a) {
    if (iplayer >= players.size())
        server::RaiseError("Invalid actor number");

    ReleaseActor(a);
    players[iplayer].actors.push_back(a);

    if (players[iplayer].actors.size() == 1) {
        // the ``main actor'' was set
        client::Msg_PlayerPosition(iplayer, a->get_pos());
    }
}

bool player::HasActor(unsigned iplayer, Actor *a) {
    if (iplayer < players.size()) {
        for (auto &actor : players[iplayer].actors) {
            if (actor == a)
                return true;
        }
    }
    return false;
}

void player::SwapPlayers() {
    if (NumberOfRealPlayers() >= 2) {
        SetCurrentPlayer(1 - icurrent_player);
    }
}

static bool has_extralive(unsigned pl, unsigned num) {
    size_t idx = 0;
    for (int i = 0; i < num; i++) {
        int idxLife = players[pl].inventory.find("it_extralife", idx);
        if (idxLife == -1)
            return false;
        else
            idx = idxLife + 1;
    }
    return true;
}

static bool resurrect_actor(unsigned pl, Actor *a) {
    assert(server::ConserveLevel);  // no resurrection otherwise!

    bool has_life = has_extralive(pl, 1);
    if (has_life)
        leveldat.resurrect_actor(a);  // = resurrect with delay
    else
        players[pl].out_of_lives = true;
    return has_life;
}

void player::AddUnassignedActor(Actor *a) {
    unassignedActors.push_back(a);
}

void player::CheckDeadActors() {
    bool toggle_player = false;
    const unsigned NO_PLAYER = UINT_MAX;
    unsigned toggle_to_player = NO_PLAYER;
    bool new_game = false;  // complete restart (new lives)

    // to live means to be not dead and to be able to move
    for (int pl = -1; pl < (int)players.size(); ++pl) {  // -1 are unassigned actors
        vector<Actor *> &actors = (pl == -1) ? unassignedActors : players[pl].actors;
        bool has_living_actor = false;  // controllable and living
        std::map<std::string, int> essMap;
        std::map<std::string, int>::iterator itEss;

        for (auto a : actors) {
            std::string essId;
            if (Value v = a->getAttr("essential_id"))
                essId = v.to_string();
            else
                essId = a->get_traits().name;
            int essential = a->getAttr("essential");
            // count number of necessary actors per kind
            if (essential == 2)
                --essMap[essId];

            if (!a->is_dead() || (pl >= 0 && server::ConserveLevel && resurrect_actor(pl, a))) {
                // actor is still alive
                if (pl >= 0 && a->controlled_by(pl) && a->isSteerable()) {
                    has_living_actor = true;
                }
                // count number of alive actors per kind
                if (essential == 0 || essential == 2)
                    ++essMap[essId];
            } else {
                // player is dead and could not resurrect
                if (essential == 1) {
                    // actor is personnally essential but dead
                    new_game = true;
                }
            }
        }
        // check if for any kind we have less living actors as required
        for (itEss = essMap.begin(); itEss != essMap.end(); itEss++) {
            if ((*itEss).second < 0)
                new_game = true;
        }

        if (has_living_actor) {
            if (toggle_to_player == NO_PLAYER)
                toggle_to_player = pl;
        } else {
            if (pl == icurrent_player) {
                // check if player has yinyang for single gamer mode
                if (player::GetInventory(pl)->find("it_yinyang", 0) >= 0)
                    toggle_player = true;
                else
                    new_game = true;
            }
        }
    }

    // if no_living_player -> toggle_player is true and toggle_to_player is NO_PLAYER
    // => new_game is set to true below

    if ((server::ConserveLevel == false) && !server::IsRestartingLevel() &&
        (new_game || (toggle_player && toggle_to_player == NO_PLAYER))) {
        // check if we have enough extralives for a restart instead of new game
        std::vector<int> numDead;
        bool reset_level = true;
        for (unsigned pl = 0; pl < players.size(); ++pl) {
            numDead.push_back(0);
            for (auto a : players[pl].actors) {
                if (a->is_dead()) {
                    numDead[pl]++;
                }
            }
            if (!has_extralive(pl, numDead[pl])) {
                reset_level = false;
                break;
            }
        }
        if (reset_level) {
            for (unsigned pl = 0; pl < players.size(); ++pl) {
                Inventory *inv = player::GetInventory(pl);
                for (int i = 0; i < numDead[pl]; i++) {
                    int idx = inv->find("it_extralife");
                    ASSERT(idx != -1, XLevelRuntime, "Missing extralife for restart of level");
                    delete inv->yield_item(idx);
                }
            }
            server::RestartLevel();  // should restart w/o scrolling
            return;
        }
    }

    if (new_game) {
        server::Msg_RestartGame();
    } else if (toggle_player) {
        if (toggle_to_player == NO_PLAYER)
            server::Msg_RestartGame();
        else
            player::SetCurrentPlayer(toggle_to_player);
    }
}

Actor *player::GetMainActor(unsigned iplayer) {
    vector<Actor *> &actors = players[iplayer].actors;
    return actors.empty() ? nullptr : actors[0];
}

void player::Tick(double dtime) {
    // Respawn actors that have been dead for a certain amount of time
    leveldat.respawn_dead_actors(dtime);

    // Update the respawn list or restart the game when all actors are
    // dead and no extra lifes are left.
    CheckDeadActors();
}

/*! Tell clients about position of main actor for stereo sound and
   screen position. */
void player::MessagePlayerPositionsToClient() {
    for (unsigned iplayer = 0; iplayer < players.size(); ++iplayer)
        if (Actor *ac = player::GetMainActor(iplayer))
            client::Msg_PlayerPosition(iplayer, ac->get_pos());
}

void player::InhibitPickup(bool flag) {
    players[icurrent_player].inhibit_pickup = flag;
}

/*! Return pointer to inventory if actor may pick up items, 0
   otherwise. */
Inventory *player::MayPickup(Actor *a, Item *it, bool allowFlying) {
    int iplayer = -1;
    if (Value v = a->getAttr("owner"))
        iplayer = v;
    if (iplayer < 0 || (unsigned)iplayer >= players.size()) {
        return nullptr;
    }

    Inventory *inv = GetInventory(iplayer);
    bool dont_pickup = players[iplayer].inhibit_pickup || (!allowFlying && a->is_flying()) ||
                       !inv->willAddItem(it) || a->is_dead() ||
                       (server::GameCompatibility != GAMET_ENIGMA && a->getClass() != "ac_marble");

    return dont_pickup ? nullptr : inv;
}

void player::PickupItem(Actor *a, GridPos p) {
    if (Inventory *inv = MayPickup(a, GetField(p)->item)) {
        if (Item *item = YieldItem(p)) {
            inv->add_item(item);
            item->on_pickup(a);
            RedrawInventory(inv);
            sound::EmitSoundEvent("pickup", p.center());
        }
    }
}

bool player::PickupAsItem(Actor *a, GridObject *obj, std::string kind) {
    if (Item *item = MakeItem(kind.c_str())) {
        if (Inventory *inv = MayPickup(a, item, true)) {
            inv->add_item(item);
            obj->transferIdentity(item);
            player::RedrawInventory(inv);
            sound::EmitSoundEvent("pickup", a->get_pos());
            return true;
        } else {
            DisposeObject(item);
        }
    }
    return false;
}

void player::ActivateFirstItem() {
    Inventory &inv = players[icurrent_player].inventory;

    if (inv.size() > 0) {
        Item *it = inv.get_item(0);
        Actor *ac = nullptr;
        GridPos p;
        bool can_drop_item = false;
        std::vector<Actor *>::iterator itr;
        for (itr = players[icurrent_player].actors.begin();
             itr != players[icurrent_player].actors.end() && ac == nullptr; itr++) {
            if (!(*itr)->is_dead()) {
                ac = *itr;
                p = GridPos(ac->get_pos());
                can_drop_item = ac->can_drop_items();
            }
        }

        if (ac != nullptr) {
            switch (it->activate(ac, p)) {
            case ITEM_DROP:
                // only drop if no item underneath and actor allows it
                if (it->can_drop_at(p) && can_drop_item) {
                    it = inv.yield_first();
                    RedrawInventory(&inv);
                    it->drop(ac, p);
                }
                break;
            case ITEM_KILL:
                DisposeObject(inv.yield_first());
                RedrawInventory(&inv);
                break;
            case ITEM_KEEP: break;
            }
        }
    }
}

void player::RotateInventory(int dir) {
    sound::EmitSoundEvent("invrotate", ecl::V2());
    Inventory &inv = players[icurrent_player].inventory;
    if (dir == 1)
        inv.rotate_left();
    else
        inv.rotate_right();
    RedrawInventory(&inv);
}

/** Update the specified inventory on the screen, provided it is the
    inventory of the current player.  For all other inventories, this
    function does nothing. */
void player::RedrawInventory(Inventory *inv) {
    if (inv == GetInventory(CurrentPlayer()))
        RedrawInventory();
}

void player::RedrawInventory() {
    Inventory *inv = GetInventory(CurrentPlayer());
    std::vector<std::string> modelnames;
    for (size_t i = 0; i < inv->size(); ++i) {
        Item *it = inv->get_item(i);
        modelnames.push_back(it->get_inventory_model());
    }
    display::GetStatusBar()->set_inventory(CurrentPlayer() == 0 ? YIN : YANG, modelnames);
}

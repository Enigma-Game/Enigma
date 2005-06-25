/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
 * $Id: player.hh,v 1.18 2004/03/15 20:15:34 dheck Exp $
 */
#ifndef PLAYER_HH
#define PLAYER_HH

/*
 * Player management.  Deals mostly with inventory management,
 * switching between players, respawning, etc.  Players are numbered
 * from 0..N-1, where N is the number of players in the current game
 * (currently always 1 or 2).
 */

#include "objects.hh"

namespace enigma_player
{
    using namespace enigma;

    class ItemHolder {
    public:
        virtual ~ItemHolder() {}

        //! Return true if not further object can be picked up
        virtual bool is_full() const = 0;

        //! Add another item
        virtual void add_item (Item *it) = 0;
    };

    class Inventory : public ItemHolder {
    public:
        Inventory();
        ~Inventory();

        // ItemHolder interface
        bool is_full() const;
        void add_item(Item *i);


        int size() const { return m_items.size(); } // number of items
        void clear();

        void  rotate(int dir=1);
        void  activate_first();
        Item *get_item(int idx) const;
        Item *yield_item(int idx);
        Item *yield_first();

        int find(const std::string& kind, int start_idx = 0) const;

        void redraw();

    private:
        // Private methods.
        Item *remove_item(Item *it);

        // Private variables.
        static const unsigned max_items;
        std::vector<Item*> m_items;
    };

    /*! Start a new game for two virtual players. */
    void NewGame();

    /*! This is called whenever a new level is reached in a running
      game.  The inventories of all players are cleaned up, i.e., all
      items except for extra lifes are removed. */
    void NewWorld();

    /*! Add a yinyang item to all players' inventories. */
    void AddYinYang ();

    /*! Called after the loading a level but before starting the
      game.  It initializes the inventory. */
    void LevelLoaded();

    /*! Called as soon as the current level is finished; it removes
      the actors of all players from the level. */
    void LevelFinished();

    /* This function is only used by the YinYang items to exchange the
       two players. */
    void SwapPlayers();

    // set/remove respawn positions for all black or all white actors
    // (used when it-flagwhite/black is dropped)
    void SetRespawnPositions(enigma::GridPos pos, bool black);
    void RemoveRespawnPositions(bool black);

    int      CurrentPlayer();
    void     SetCurrentPlayer(unsigned iplayer);
    unsigned NumberOfRealPlayers();
    bool     IsCurrentPlayer(Actor *a);

    Inventory *MayPickup(Actor *a);
    Inventory *GetInventory(int iplayer);
    Inventory *GetInventory(Actor *a);
    Item      *wielded_item (Actor *a);
    bool       wielded_item_is(Actor *a, const std::string &kind);

    void Suicide();

    void AddActor (unsigned iplayer, Actor *a);
    Actor *ReplaceActor (unsigned iplayer, Actor *old, Actor *a);
    Actor *GetMainActor (unsigned iplayer);

    bool     AllActorsDead();

    void InhibitPickup (bool yesno);
    void PickupItem (Actor *a, enigma::GridPos p);
    void PickupStoneAsItem (Actor *a, enigma::GridPos p);
    void RotateInventory (int dir=1);
    void ActivateItem();

    void Tick (double dtime);
}

namespace enigma
{
    namespace player = enigma_player;
}
#endif

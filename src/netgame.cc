/*
 * Copyright (C) 2005 Daniel Heck
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
#include "client.hh"
#include "main.hh"
#include "options.hh"
#include "netgame.hh"
#include "server.hh"

#include "enet/enet.h"

#include "SDL.h"
#include <string>

using namespace enigma;

#include "client_internal.hh"
#include "server_internal.hh"

namespace
{
    class Peer {
    public:
        virtual ~Peer() {}
        virtual void send_message (const Buffer &b, int channel) = 0;
        virtual void send_reliable (const Buffer &b, int channel) = 0;
        virtual bool poll_message (Buffer &b, int &player_no) = 0;
        virtual void disconnect(int timeout=1000) = 0;
        virtual bool is_connected() const = 0;
    };

    class Peer_Enet : public Peer {
    public:
        Peer_Enet (ENetHost *host, ENetPeer *peer, int playerno) 
        : m_host (host),
          m_peer (peer), 
          m_playerno (playerno),
          m_connected (true)
        {
        }


        ~Peer_Enet() 
        {
            disconnect(0);
        }

        // Peer interface

        virtual void send_message (const Buffer &b, int channel)
        {
            ENetPacket *packet = enet_packet_create (b.data(), b.size(), 0);
            enet_peer_send (m_peer, channel, packet);
        }

        virtual void send_reliable (const Buffer &b, int channel)
        {
            ENetPacket *packet = enet_packet_create (b.data(), b.size(),
                                                     ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send (m_peer, channel, packet);
        }

        virtual bool poll_message (Buffer &b, int &player_no) 
        {
            if (!m_connected)
                return false;

            ENetEvent event;
            if (enet_host_service (m_host, & event, 0) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    b.assign (reinterpret_cast<char*> (event.packet->data), 
                              event.packet->dataLength);
                    player_no = m_playerno;
                    enet_packet_destroy (event.packet);
                    return true;
                }
           
                case ENET_EVENT_TYPE_DISCONNECT:
                    m_connected = false;
                    event.peer -> data = NULL;
                    break;

                default:
                    break;
                }
            }
            return false;
        }

        virtual void disconnect(int timeout = 1000) 
        {
            ENetEvent event;
    
            enet_peer_disconnect (m_peer);

            while (enet_host_service (m_host, & event, timeout) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    enet_packet_destroy (event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf ("Disconnection succeeded.\n");
                    return;
                case ENET_EVENT_TYPE_NONE:
                case ENET_EVENT_TYPE_CONNECT:
                    break;
                }
            }
    
            enet_peer_reset (m_peer);
            m_connected = false;
        }

        virtual bool is_connected() const 
        {
            return m_connected;
        }


    private:
        // Variables
        ENetHost *m_host;
        ENetPeer *m_peer;
        int m_playerno;
        bool m_connected;
    };

    class Peer_Local : public Peer {
    public:

        virtual void send_message (const Buffer &b, int channel) 
        {
        }

        virtual void send_reliable (const Buffer &b, int channel)
        {
            send_message (b, channel);
        }

        virtual bool poll_message (Buffer &b, int &player_no)
        {
            if (!m_queue.empty()) {
                
            }
            return false;
        }
    private:
        // Variables
        std::list<Buffer> m_queue;
    };
}


void handle_client_packet (Buffer &b, int player_no)
{
    using namespace enigma_server;
    Uint8 code; 
    while (b >> code) {
        switch (code) {
        case SVMSG_NOOP:
            break;              // no nothing

        case SVMSG_LOADLEVEL: {
            Uint16 levelno;
            string levelpack;
            if (b >> levelno >> levelpack) {
                printf ("SV: Loading level %d from levelpack %s\n", int(levelno), 
                        levelpack.c_str());
                server::Msg_SetLevelPack (levelpack);
                server::Msg_LoadLevel (levelno);
            }

            break;
        }

        case SVMSG_MOUSEFORCE: {
            printf ("mouse force\n");
            float dx, dy;
            if (b >> dx >> dy) {
                printf ("-- yei!\n");
                server::Msg_MouseForce (ecl::V2(dx, dy));
            }
            break;
        }

        case SVMSG_ACTIVATEITEM: {
            server::Msg_ActivateItem ();
            break;
        }

        default:
            enigma::Log << "SV: received undefined packet: " << int(code) << "\n";
        }
    }
}

namespace
{
    Uint32      last_tick_time;
}


void server_loop (Peer *m_peer)
{
    printf ("SV: Entered server loop\n");
    server::InitNewGame();
    Buffer buf;
    buf << Cl_LevelLoaded ();
    m_peer->send_reliable (buf, 1);

    double dtime = 0;
    while (!client::AbortGameP() && m_peer->is_connected()) {
        last_tick_time=SDL_GetTicks();


        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type && e.key.keysym.sym == SDLK_ESCAPE)
                goto done;
        }

        try {
            client::Tick (dtime);
            server::Tick (dtime);
        }
        catch (enigma_levels::XLevelRuntime& err) {        
            client::Msg_Error (string("Server Error: level runtime error:\n")
                               + err.what());
            server::Msg_Panic(true);
        }

        Buffer buf;
        int player_no;
        while (m_peer->poll_message (buf, player_no)) {
            printf ("SV: Received message from client %d\n", player_no);
            handle_client_packet (buf, player_no);
        }


        int sleeptime = 10 - (SDL_GetTicks()-last_tick_time);
        if (sleeptime >= 3)      // only sleep if relatively idle
            SDL_Delay(sleeptime);
        dtime = (SDL_GetTicks()-last_tick_time)/1000.0;
        if (fabs(1-dtime/0.01) < 0.2) {
            // less than 20% deviation from desired frame time?
            dtime = 0.01;
        }

	if (dtime > 500.0) /* Time has done something strange, perhaps
			      run backwards */
            dtime = 0.0;

    }

  done:
    return;
}

void netgame::Start ()
{

    // ---------- Create network host ----------
    ENetHost *network_host;
    ENetAddress network_address;

    network_address.host = ENET_HOST_ANY;
    network_address.port = 12345;

    network_host = enet_host_create (&network_address, 1, 0, 0);
    if (network_host == NULL) {
        fprintf (stderr, 
                 "SV: An error occurred while trying to create an ENet server host.\n");
        return;
    }

    // ---------- Wait for client(s) ----------
    ENetEvent event;
    Peer *m_peer = 0;
    printf ("SV: Waiting for client...\n");

    while (!m_peer) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type && e.key.keysym.sym == SDLK_ESCAPE)
                return;
        }
        
        while (enet_host_service (network_host, & event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                printf ("SV: Connected to client\n");
                m_peer = new Peer_Enet (network_host, event.peer, 2);
            }
        }
        SDL_Delay (10);
    }

    server_loop (m_peer);

    m_peer->disconnect();
    delete m_peer;
}


/* ==================== CLIENT ==================== */


void netgame::Join (std::string hostname, int port)
{
    printf ("CL: trying to join remote game\n");



    // ---------- Create network host ----------
    ENetHost *m_network_host;
    m_network_host = enet_host_create (NULL,
                                       1 /* only allow 1 outgoing connection */,
                                       57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                                       14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (m_network_host == NULL) {
        fprintf (stderr, "CL: An error occurred while trying to create an ENet client host.\n");
        return;
    }


    // ---------- Connect to server ----------

    ENetAddress sv_address;
    ENetPeer *m_server;

    /* Connect to some.server.net:1234. */
    enet_address_set_host (&sv_address, hostname.c_str());
    sv_address.port = port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    int numchannels = 2;
    m_server = enet_host_connect (m_network_host, &sv_address, numchannels);
    
    if (m_server == NULL) {
        fprintf (stderr, 
                 "CL: No available peers for initiating an ENet connection.\n");
        return;
    }
    

    Peer *m_peer = 0;
    ENetEvent event;
    if (enet_host_service (m_network_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        fprintf (stderr, "CL: Connection to some.server.net:12345 succeeded.\n");
        if (m_server != event.peer)
            printf ("CL: peers differ!?!\n");
        m_peer = new Peer_Enet (m_network_host, m_server, 0);
    }
    else
        return;


    while (m_peer->is_connected()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type && e.key.keysym.sym == SDLK_ESCAPE)
                goto done;
            else if (e.type ==SDL_MOUSEMOTION) {
                Buffer buf;
                float mouseforce = options::GetDouble("MouseSpeed");
                buf << Uint8 (enigma_server::SVMSG_MOUSEFORCE)
                    << float (e.motion.xrel * mouseforce)
                    << float (e.motion.yrel * mouseforce);
                m_peer->send_reliable (buf,1);
            }
        }

        Buffer buf;
        int peerno;
        while (m_peer->poll_message (buf, peerno)) {
            printf ("CL: A packet of length %u was received from %d on channel %u.\n",
                    buf.size(),
                    peerno,
                    0);          // channel

            if (buf.size() > 0) {
                Uint8 code;
                buf >> code;
                switch (code) {
                case CLMSG_LEVEL_LOADED:
                    printf ("cl_level_loaded\n");
                    break;
                }
            }
            
            Buffer buf;
            buf << Uint8(enigma_server::SVMSG_LOADLEVEL);
            buf << Uint16(84);
            buf << string("Enigma");
            m_peer->send_reliable (buf, 1);
            printf ("CL: sending message %u\n", buf.size());


            break;
        }
        SDL_Delay (10);
    }

//         enet_peer_reset (m_server);
//         m_server = 0;

//         fprintf (stderr, "Connection to localhost:12345 failed.\n");
//         return;
//     }
  done:
    m_peer->disconnect();
    delete m_peer;
    return;

    // ---------- ENet-independent code ----------


}

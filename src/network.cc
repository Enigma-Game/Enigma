/*
 * Copyright (C) 2002,2004 Daniel Heck
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: network.cc,v 1.4 2004/05/22 13:04:30 dheck Exp $
 */

#include "network.hh"
#include "px/buffer.hh"

#ifdef ENABLE_EXPERIMENTAL
#include "enet/enet.h"
#endif

#include <cassert>
#include <cstring>
#include <string>
#include <cstdio>

#include "SDL_types.h"

using namespace std;
using namespace network;

 // MemorySocket

MemorySocket::MemorySocket()
    : _target(0)		// not connected
    , _recvbuf(new Buffer(4096))
    , _pendingbuf(new Buffer(4096))
{}

MemorySocket::~MemorySocket()
{
    delete _recvbuf;
    delete _pendingbuf;
}

void
MemorySocket::set_target(MemorySocket* target)
{
    _target = target;
}

int
MemorySocket::send(const void* data, int len)
{
    if (_target == 0)
	return -1;
    _target->_pendingbuf->write (data, len);
    return 0;
}

Buffer*
MemorySocket::recv()
{
    if (_pendingbuf->size() == 0)
    {
        // no data pending
	return 0;
    }
    else
    {
	// swap pending and receive buffer
	Buffer* tmp = _recvbuf;
	_recvbuf = _pendingbuf;
	_pendingbuf = tmp;
	_pendingbuf->clear();
	return _recvbuf;
    }
}

int
MemorySocket::close()
{
    _target = 0;
    _pendingbuf->clear();
    _recvbuf->clear();
    return 0;
}


/* -------------------- Functions -------------------- */

void network::Init ()
{
#ifdef ENABLE_EXPERIMENTAL
    if (enet_initialize () != 0) {
        fprintf (stderr, "Couldn't init ENet.\n");
        exit(1);
    }
    atexit(enet_deinitialize);
#endif
}



 // Testing routine
#ifdef TESTING
#include <iostream>
#include <fstream>

int
main (int, char**)
{
    Buffer buf;
    MemorySocket sock1;
    MemorySocket sock2;
    sock1.set_target (&sock2);
    sock2.set_target (&sock1);

    buf << Uint8(12) << Uint16(-23) << Uint32(766565) << -3.1415926;
    buf << string ("Hello World!");

    sock1.send (buf.data(), buf.size());

    Buffer* b2 = sock2.recv();
    if (b2) {
	cout << "received some data:\n";

	Uint8 a = 0;
	Uint16 b = 0;
	Uint32 c = 0;
	double d;
	string str;
	(*b2) >> a >> b >> c >> d >> str;

	cout << int(a) << ", " << Sint16(b) << ", " << c << ", " << d
	     << ", '" << str << "'" << endl;
    } else {
	cout << "no message pending\n";
    }

    // new write buf to a file
    ofstream ofile ("network_test");
    ofile << buf;
    ofile.close();

    buf.clear();
    ifstream ifile ("network_test");
    ifile >> buf;
    ifile.close();

    {
	cout << "\nread the file:\n";

	Uint8 a = 0;
	Uint16 b = 0;
	Uint32 c = 0;
	double d;
	string str;
	buf >> a >> b >> c >> d >> str;

	cout << int(a) << ", " << Sint16(b) << ", " << c << ", " << d
	     << ", '" << str << "'" << endl;
    }

}
#endif

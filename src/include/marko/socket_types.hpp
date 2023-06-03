/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#pragma once

#include "socket.hpp"

/*
 * Simple UDP socket base class for query/response architecture. Not really
 * useful by itself. Look at classes that build upon it.
 */
class UDPSocket : public Socket {
public:
  UDPSocket() : Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) {}
};

// https://github.com/troydhanson/network/blob/master/unixdomain/01.basic/srv.c
// https://gist.github.com/Phaiax/ae7d1229e6f078457864dae712c51ae0
class UDSSocket : public Socket {
public:
  UDSSocket() : Socket(AF_UNIX, SOCK_STREAM, 0) {}
};

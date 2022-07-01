
#pragma once

#include "socket.hpp"

// https://github.com/troydhanson/network/blob/master/unixdomain/01.basic/srv.c
// https://gist.github.com/Phaiax/ae7d1229e6f078457864dae712c51ae0
class UDSSocket: public Socket {
public:
    UDSSocket(int timeout=0): Socket(AF_UNIX, SOCK_STREAM, 0){}
};

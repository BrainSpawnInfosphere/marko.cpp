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
  // UDP bind
  // INADDR_ANY - bind to all available interfaces
  // port=0 let's the OS pick a port number
  void bind(const std::string &saddr, int port) {
    // allow multiple sockets to re-use the same address and port
    setsockopt(SOL_SOCKET, SO_REUSEPORT, 1);
    setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);

    sockaddr_t aaddr = make_sockaddr(saddr, port);
    int err = ::bind(socket_fd, (struct sockaddr *)&aaddr, sizeof(aaddr));
    guard(err, "bind() failed for (" + saddr + ":" + std::to_string(port) + ")");
  }
  void connect(const std::string &HOST, int PORT) {
    // Create a sockaddr_in structure for UDP
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(HOST.c_str());

    // Connect to the remote host
    int err = ::connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    guard(err, "connect() failed for (" + HOST + ":" + std::to_string(PORT) + ")");
  }
};

// https://github.com/troydhanson/network/blob/master/unixdomain/01.basic/srv.c
// https://gist.github.com/Phaiax/ae7d1229e6f078457864dae712c51ae0
class UDSSocket : public Socket {
public:
  UDSSocket() : Socket(AF_UNIX, SOCK_STREAM, 0) {}

  // UDS bind
  void bind(const std::string &path) {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path.c_str());

    int err = ::bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    guard(err, "bind() failed for UDS " + path);
  }

  void connect(const std::string &path) {
    // struct sockaddr_un addr;
    // memset(&addr, 0, sizeof(addr));
    // addr.sun_family = AF_UNIX;
    // strcpy(addr.sun_path, path.c_str());
    geckoUDS_t addr = geckoUDS(path);

    int err = ::connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    guard(err, "connect() failed for UDS " + path);
  }
};


#pragma once

#include "socket.hpp"
#include <sys/un.h>     // UDS


class SocketUDS: public Socket {
  public:
  SocketUDS() {}
  ~SocketUDS() {}

  // void bind(const std::string &path, bool multipleBind=false) {}
  message_t recvfrom(size_t msg_size, udsaddr_t *from_addr, const int flags=0) {
    message_t m;
    return std::move(m);
  }

  message_t recv(size_t msg_size, const int flags=0) {
    message_t m;
    return std::move(m);
  }

  int sendto(const message_t& msg, const udsaddr_t &addr, int flags=0) {
    return 0;
  }

  // static udsaddr_t getsockaddr(const std::string& path) {
  //   udsaddr_t addr = {0};
  //   // addr.sin_family      = AF_INET;
  //   // addr.sin_addr.s_addr = inet_addr(ip.c_str());
  //   // addr.sin_port        = htons(port);
  //   return std::move(addr);
  // }
};
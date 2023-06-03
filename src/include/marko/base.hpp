/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <iostream>
#include <string>
#include "socket.hpp"

template<typename SOCKET>
class Base {
public:
  Base() {}

  void info() {}

  inline void bind(const std::string &addr, int port) {
    sock.bind(addr, port);
  }

protected:
  SOCKET sock;
};
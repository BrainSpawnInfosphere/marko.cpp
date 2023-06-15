/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include "socket.hpp"
#include <iostream>
#include <string>

template <typename SOCKET> class Base {
public:
  Base() {}
  ~Base() { sock.close(); }

  void info() {}

  inline void bind(const std::string &addr, int port) { sock.bind(addr, port); }
  inline void bind(const std::string &path) { sock.bind(path); }

  inline void connect(const std::string &addr, int port) { sock.connect(addr, port); }
  // inline void connect(const std::string &path) { sock.connect(path); }

protected:
  SOCKET sock;
};
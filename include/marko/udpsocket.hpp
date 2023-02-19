/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#pragma once

#include <cstdio>
#include <functional> // std::function
#include "socket.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

constexpr int TIMEOUT = 5000;

/*
 * Simple UDP socket base class for query/response architecture. Not really
 * useful by itself. Look at classes that build upon it.
 */
class UDPSocket : public Socket {
public:
  UDPSocket(int timeout = 0) : Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) {}
};

class Base {
public:
  Base() { ; }

  void info() { ; }

  inline void bind(const std::string &addr, int port) {
    socket.bind(addr, port);
  }

protected:
  UDPSocket socket;
};

////////////////////////////////////////////////////////
//       TEMPLATES
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//       Publisher / Subscriber
////////////////////////////////////////////////////////

template <typename T> class TSubscriber : public Base {
public:
  TSubscriber() { ; }
  ~TSubscriber() { ; }

  typedef std::function<void(const T &)> TSubCallback_t;

  void register_cb(TSubCallback_t f) { cb.push_back(f); }

  void loop(Event &event, int timeout = TIMEOUT) {
    socket.settimeout(timeout);
    T s;
    sockaddr_t from_addr;
    while (event.is_set()) {
      this->socket.recvfrom(&s, sizeof(s), from_addr);
      for (const auto &callback : this->cb) {
        callback(s);
      }
    }
  }

  void once(int timeout = TIMEOUT) {
    socket.settimeout(timeout);
    T s;
    sockaddr_t from_addr;
    this->socket.recvfrom(&s, sizeof(s), from_addr);
    for (const auto &callback : this->cb) {
      callback(s);
    }
  }

protected:
  std::vector<TSubCallback_t> cb;
};

template <typename T> class TPublisher : public Base {
public:
  TPublisher() {}
  ~TPublisher() {}

  void publish(const T &data) {
    for (const auto &addr : clientaddr) {
      socket.sendto(&data, sizeof(T), addr);
    }
  }

  // value?
  inline void setClientAddr(const sockaddr_t &c) { clientaddr.push_back(c); }

  std::vector<sockaddr_t> clientaddr;
};

////////////////////////////////////////////////////////
//       Request / Reply
////////////////////////////////////////////////////////

template <typename REQ, typename REPLY> class TReply : public Base {
public:
  // TReply(const std::string& addr, int port){
  //     socket.bind(addr, port);
  // }
  TReply() {}
  ~TReply() {}

  typedef std::function<REPLY(const REQ &)> TReplyCallback_t;

  // inline void bind(const std::string& addr, int port){
  //     socket.bind(addr, port);
  // }
  inline void register_cb(TReplyCallback_t f) { cb.push_back(f); }

  void loop(Event &event, int timeout = TIMEOUT) {
    socket.settimeout(timeout);
    REQ s;
    sockaddr_t addr;
    while (event.is_set()) {
      bool good = socket.recvfrom(&s, sizeof(REQ), addr);
      // std::cout << good << std::endl;
      if (!good) {
        // std::cout << "miss" << std::endl;
        continue;
      }
      for (const auto &callback : this->cb) {
        REPLY reply = callback(s);
        socket.sendto(&reply, sizeof(REPLY), addr);
      }
    }
  }

protected:
  std::vector<TReplyCallback_t> cb;
};

template <typename REQ, typename REP> class TRequest : public Base {
public:
  TRequest() {}
  ~TRequest() {}

  REP request(const REQ &data, const sockaddr_t &addr) {
    socket.sendto(&data, sizeof(REQ), addr);
    REP reply;
    sockaddr_t tmp;
    socket.recvfrom(&reply, sizeof(REP), tmp);
    return reply;
  }
};

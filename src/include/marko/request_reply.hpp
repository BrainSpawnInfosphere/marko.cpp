// /**************************************************\
// * The MIT License (MIT)
// * Copyright (c) 2014 Kevin Walchko
// * see LICENSE for full details
// \**************************************************/
// ////////////////////////////////////////////////////////
// //       Request / Reply
// ////////////////////////////////////////////////////////
#pragma once

// // #include "base.hpp"
#include "event.hpp"  // Event
#include "socket_types.hpp"
#include "sockaddr.hpp"
#include <functional> // std::function
#include <string>
#include <vector>

template <typename REQ, typename REPLY>
class Reply : public SocketUDP {
public:
  Reply(): msg_size(sizeof(REQ)) {}
  ~Reply() {}

  typedef std::function<REPLY(const REQ &)> ReplyCallback_t;
  // typedef std::function<message_t(const message_t &)> ReplyCallback_t;

  inline void register_cb(ReplyCallback_t func) {
    callback = func;
  }

  void loop(Event &event) {
    while (event.is_set()) {
      once();
    }
  }

  void loop() {
    // std::cout << "start" << std::endl;
    while (true) {
      once();
    }
  }

  void once() {
    udpaddr_t from_addr = {0};
    message_t m = recvfrom(msg_size, &from_addr);

    if (m.size() == 0) return;
    REQ q = unpack<REQ>(m);
    REPLY r = callback(q);
    message_t reply = pack<REPLY>(r);
    sendto(reply, from_addr);
  }

protected:
  ReplyCallback_t callback;
  const size_t msg_size;
};

// typedef Reply ReplyUDP;
template <typename REQ, typename REPLY>
using ReplyUDP = Reply<REQ, REPLY>;
// template <typename REQ, typename REPLY>
// using ReplyUDS = Reply<REQ, REPLY, UDSSocket>;

// /////////////////////////////////////////////////////////////////

// template <typename REQ, typename REP, typename SOCKET>
template <typename REQ, typename REP>
class Request : public SocketUDP {
public:
  // Request(size_t size): msg_size(size) {}
  Request(): msg_size(sizeof(REP)) {}
  ~Request() {}

  const REP& request(const REQ &data, const udpaddr_t &addr) {
    message_t m = pack<REQ>(data);
    sendto(m, addr);
    message_t msg = recv(msg_size);
    REP ans = unpack<REP>(msg);
    return std::move(ans);
  }

protected:
  const size_t msg_size;
};

template <typename REQ, typename REPLY>
using RequestUDP = Request<REQ, REPLY>;
// template <typename REQ, typename REPLY>
// using RequestUDS = Request<REQ, REPLY, UDSSocket>;
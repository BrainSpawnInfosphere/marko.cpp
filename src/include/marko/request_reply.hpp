/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
////////////////////////////////////////////////////////
//       Request / Reply
////////////////////////////////////////////////////////
#pragma once

#include "base.hpp"
#include "event.hpp"  // Event
#include "socket_types.hpp"
#include <functional> // std::function
#include <string>
#include <vector>

template <typename REQ, typename REPLY, typename SOCKET>
class Reply : public Base<SOCKET> {
public:
  Reply(int timeout = 5000) : timeout(timeout), run(false) {}
  ~Reply() {}

  typedef std::function<REPLY(const REQ &)> ReplyCallback_t;

  inline void register_cb(ReplyCallback_t func) {
    callback = func;
  } // { cb.push_back(f); }

  void loop(Event &event, int timeout = 5000) {
    this->sock.settimeout(timeout);
    this->run = true;
    while (event.is_set()) {
      once();
    }
  }

  void once() {
    REQ s;
    sockaddr_t from_addr;
    int size = this->sock.recvfrom(&s, sizeof(s), from_addr);
    if (size <= 0) return;
    // for (const ReplyCallback_t &callback : this->cb) {
    //     REPLY reply = callback(s);
    //     this->sock.sendto(&reply, sizeof(REPLY), from_addr);
    // }
    REPLY reply = callback(s);
    this->sock.sendto(&reply, sizeof(REPLY), from_addr);
  }

  void stop() { run = false; }

protected:
  // std::vector<ReplyCallback_t> cb;
  int timeout;
  bool run; // remove Event from loop()
  ReplyCallback_t callback;
};

template <typename REQ, typename REPLY>
using ReplyUDP = Reply<REQ, REPLY, UDPSocket>;
template <typename REQ, typename REPLY>
using ReplyUDS = Reply<REQ, REPLY, UDSSocket>;

/////////////////////////////////////////////////////////////////

template <typename REQ, typename REP, typename SOCKET>
class Request : public Base<SOCKET> {
public:
  Request() {}
  ~Request() {}

  const REP &request(const REQ &data, const sockaddr_t &addr) {
    this->sock.sendto(&data, sizeof(REQ), addr);
    sockaddr_t tmp;
    this->sock.recvfrom(&(this->replyFrom), sizeof(REP), tmp);
    return replyFrom;
  }

protected:
  REP replyFrom;
};

template <typename REQ, typename REPLY>
using RequestUDP = Request<REQ, REPLY, UDPSocket>;
template <typename REQ, typename REPLY>
using RequestUDS = Request<REQ, REPLY, UDSSocket>;
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


template <typename SOCKET, typename SOCKADDR>
class Reply : public SOCKET {
public:
  Reply(size_t size): msg_size(size) {}
  ~Reply() {}

  typedef std::function<message_t(const message_t &)> ReplyCallback_t;

  inline void register_cb(ReplyCallback_t func) { callback = func; }

  void loop(Event &event) { while (event.is_set()) once(); }
  void loop() { while (true) once(); }

  void once() {
    SOCKADDR from_addr = {0};
    message_t m = SOCKET::recvfrom(msg_size, &from_addr);

    if (m.size() == 0) return;
    if (m.size() != msg_size) std::cout << "CRAP" << std::endl;
    message_t r = callback(m);

    std::cout << "Reply::once: " << get_ip_port(from_addr) << std::endl;
    SOCKET::sendto(r, from_addr);
  }

protected:
  ReplyCallback_t callback;
  const size_t msg_size;
};

using ReplyUDP = Reply<SocketUDP, udpaddr_t>;

// /////////////////////////////////////////////////////////////////

template <typename SOCKET, typename SOCKADDR>
class Request : public SOCKET {
public:
  Request(size_t size): msg_size(size) {}
  ~Request() {}

  const message_t request(const message_t &msg, const SOCKADDR &addr) {
    // std::cout << "request" << std::endl;
    SOCKET::sendto(msg, addr);
    std::cout << "Request::sendto:  " << get_ip_port(addr)  << std::endl;

    SOCKADDR from_addr = {0};
    message_t rep = SOCKET::recvfrom(msg_size, &from_addr);
    std::cout << "Request::recvfrom:  " << get_ip_port(from_addr)  << std::endl;
    return std::move(rep);
  }

protected:
  const size_t msg_size;
};

// template <typename REQ, typename REPLY>
// using RequestUDP = Request<REQ, REPLY, SocketUDP, udpaddr_t>;
// template <typename REQ, typename REPLY>
// using RequestUDS = Request<UDSSocket>;
using RequestUDP = Request<SocketUDP, udpaddr_t>;
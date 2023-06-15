/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
////////////////////////////////////////////////////////
//       Publisher / Subscriber
////////////////////////////////////////////////////////
#pragma once

// #include "base.hpp"
#include "event.hpp"
#include "socket.hpp"
#include "socket_types.hpp"
#include "signals.hpp"
#include "sockaddr.hpp"
#include <functional> // std::function
#include <string>
#include <vector>

class Subscriber : public SocketUDP {
public:
  Subscriber(size_t data_size): data_size(data_size) {}
  ~Subscriber() {}

  typedef std::function<void(const message_t &)> SubCallback_t;

  void register_cb(SubCallback_t func) {
    callbacks.push_back(func);
  }

  void loop() {
    while (true) once();
  }

  void loop(Event &event) {
    while (event.is_set()) once();
  }

  void once() {
    message_t msg = recv(data_size);
    if (msg.size() == 0) return;
    for (const SubCallback_t &callback: this->callbacks) callback(msg);
  }

protected:
  std::vector<SubCallback_t> callbacks; // FIXME: rename callbacks
  size_t data_size;
};

typedef Subscriber SubscriberUDP;
// template <typename T> using SubscriberUDP = Subscriber<T>;
// template <typename T> using SubscriberUDP = Subscriber<T>;
// template <typename T> using SubscriberUDS = Subscriber<T, UDSSocket>;

///////////////////////////////////////////////////////////////////////////

// template <typename SOCKADDR> class Publisher : public Socket<SOCKADDR>
class Publisher : public SocketUDP {
public:
  Publisher() {}
  ~Publisher() {}

  void publish(const message_t &data) {
    for (const udpaddr_t &addr : clientaddrs) sendto(data, addr);
  }

  inline void register_addr(const udpaddr_t &c) { clientaddrs.push_back(c); }

protected:
  std::vector<udpaddr_t> clientaddrs;
};

typedef Publisher PublisherUDP;
// template <typename T> using PublisherUDP = Publisher<T>;
// template <typename T> using PublisherUDP = Publisher<T, udpaddr_t>;
// template <typename T> using PublisherUDS = Publisher<T, UDSSocket>;
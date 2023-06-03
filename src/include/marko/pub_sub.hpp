/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
////////////////////////////////////////////////////////
//       Publisher / Subscriber
////////////////////////////////////////////////////////
#pragma once


#include "base.hpp"
#include "event.hpp" // Event
#include <functional> // std::function
#include <string>
#include <vector>

template <typename T, typename SOCKET>
class Subscriber : public Base<SOCKET> {
public:
  Subscriber() {}
  ~Subscriber() {}

  typedef std::function<void(const T &)> SubCallback_t;

  void register_cb(SubCallback_t func) { callbacks.push_back(func); }

  void loop(Event &event, int timeout = 5000) {
    run = true;
    this->sock.settimeout(timeout);
    while (event.is_set()) {
      once();
    }
  }

  void once() {
    T s;
    sockaddr_t from_addr;
    int size = this->sock.recvfrom(&s, sizeof(s), from_addr);
    if (size <= 0) return;
    for (const SubCallback_t &callback : this->callbacks) {
      callback(s);
    }
  }

  void stop() { run = false; }

protected:
  std::vector<SubCallback_t> callbacks; // FIXME: rename callbacks
  bool run; // remove Event from loop()
};

template<typename T>
using SubscriberUDP = Subscriber<T,UDPSocket>;
template<typename T>
using SubscriberUDS = Subscriber<T,UDSSocket>;

///////////////////////////////////////////////////////////////////////////

template <typename T, typename SOCK>
class Publisher : public Base<SOCK> {
public:
  Publisher() {}
  ~Publisher() {}

  void publish(const T &data) {
    for (const sockaddr_t &addr : clientaddrs) {
      this->sock.sendto(&data, sizeof(T), addr);
    }
  }

  inline void register_addr(const sockaddr_t &c) { clientaddrs.push_back(c); }

protected:
  std::vector<sockaddr_t> clientaddrs;
};


template<typename T>
using PublisherUDP = Publisher<T,UDPSocket>;
template<typename T>
using PublisherUDS = Publisher<T,UDSSocket>;
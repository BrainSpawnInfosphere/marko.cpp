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
// #include "event.hpp"  // Event
// #include "socket_types.hpp"
// #include "sockaddr.hpp"
// #include <functional> // std::function
// #include <string>
// #include <vector>

// template <typename REQ, typename REPLY, typename SOCKET>
// class Reply : public SOCKET {
// public:
//   Reply() {}
//   ~Reply() {}

//   typedef std::function<REPLY(const REQ &)> ReplyCallback_t;

//   inline void register_cb(ReplyCallback_t func) {
//     callback = func;
//   }

//   void loop(Event &event) {
//     while (event.is_set()) {
//       once();
//     }
//   }

//   void loop() {
//     std::cout << "start" << std::endl;
//     while (true) {
//       once();
//     }
//   }

//   void once() {
//     std::cout << "loop" << std::endl;
//     REQ s;
//     geckoUDP_t from_addr = {0};
//     int size = SOCKET::recvfrom(reinterpret_cast<void*>(&s), sizeof(s), from_addr);
//     std::cout<<size<<std::endl;
//     if (size <= 0) return;
//     REPLY reply = callback(s);
//     SOCKET::sendto(reinterpret_cast<void*>(&reply), sizeof(reply), from_addr);
//   }

// protected:
//   ReplyCallback_t callback;
// };

// template <typename REQ, typename REPLY>
// using ReplyUDP = Reply<REQ, REPLY, UDPSocket>;
// // template <typename REQ, typename REPLY>
// // using ReplyUDS = Reply<REQ, REPLY, UDSSocket>;

// // /////////////////////////////////////////////////////////////////

// template <typename REQ, typename REP, typename SOCKET>
// class Request : public SOCKET {
// public:
//   Request() {}
//   ~Request() {}

//   const REP& request(const REQ &data, const geckoUDP_t &addr) {
//     SOCKET::sendto(reinterpret_cast<const void*>(&data), sizeof(REQ), addr);
//     SOCKET::recv(reinterpret_cast<void*>(&this->replyFrom), sizeof(REP));
//     return replyFrom;
//   }

// protected:
//   REP replyFrom;
// };

// template <typename REQ, typename REPLY>
// using RequestUDP = Request<REQ, REPLY, UDPSocket>;
// // template <typename REQ, typename REPLY>
// // using RequestUDS = Request<REQ, REPLY, UDSSocket>;
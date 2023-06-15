/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include "sockaddr.hpp"
#include <errno.h> // errno
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h> // socket(), bind(), recvfrom(), ...
#include <sys/un.h>     // UDS
#include <unistd.h>     // for close()

typedef int socket_t;

extern int errno; // don't like this global value

/**
 * Simple C++ wrapper around the C networking stuff I use. Modelled after
 * python socket module.
 */
class Socket {
public:
  Socket(int family, int type, int proto) {
    // create a socket
    socket_fd = socket(family, type, proto);
    guard(socket_fd, "Socket() constructor failed");

    this->family   = family;
    this->type     = type;
    this->proto    = proto;
    this->blocking = true;
    this->timeout  = 0;
  }

  // // UDP bind
  // // INADDR_ANY - bind to all available interfaces
  // void bind(const std::string &saddr, int port) {
  //   // allow multiple sockets to re-use the same address and port
  //   setsockopt(SOL_SOCKET, SO_REUSEPORT, 1);
  //   setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);

  //   sockaddr_t aaddr = make_sockaddr(saddr, port);
  //   int err = ::bind(socket_fd, (struct sockaddr *)&aaddr, sizeof(aaddr));
  //   guard(err, "bind() failed for (" + saddr + ")");
  // }

  // // UDS bind
  // void bind(const std::string &path) {
  //   struct sockaddr_un addr;
  //   memset(&addr, 0, sizeof(addr));
  //   addr.sun_family = AF_UNIX;
  //   strcpy(addr.sun_path, path.c_str());

  //   int err = ::bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
  //   guard(err, "bind() failed for UDS");
  // }

  void close() { ::close(socket_fd); }

  // Gets sockaddr_t for connected peer system.
  sockaddr_t getpeername() {
    sockaddr_t addr      = {0};
    unsigned int addrlen = sizeof(addr);
    guard(::getpeername(socket_fd, (struct sockaddr *)&addr, &addrlen),
          "getsockname(): ");
    return addr;
  }

  // Gets the sockaddr_t for this system
  sockaddr_t getsockname() {
    sockaddr_t addr      = {0};
    unsigned int addrlen = sizeof(addr);
    guard(::getsockname(socket_fd, (struct sockaddr *)&addr, &addrlen),
          "getsockname(): ");
    return addr;
  }

  /**
   * This will cause a socket recvfrom to return after the timeout period
   * with -1 if no data is received
   */
  void settimeout(int timeout) {
    this->blocking = false;
    this->timeout  = timeout;

    struct timeval tv;
    long sec  = 0;
    long msec = 0;

    if (timeout >= 1000) {
      sec = (long)timeout / 1000;
      timeout %= 1000;
    }

    tv.tv_sec  = sec;
    tv.tv_usec = msec * 1000;

    guard(::setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)),
          "settimeout(): ");
  }

  /**
   * This will throw a signal that must be caught
   */
  void setnonblocking() {
    // int flags = guard(fcntl(socket_fd, F_GETFL), "setblocking(): ");
    // guard(fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK), "setblocking(): ");

    int on = 1;
    guard(ioctl(socket_fd, FIONBIO, (char *)&on), "setnonblocking(): ");
  }

  /*
  dst: buffer
  size: size of data to get
  from_addr: remote system sockaddr_t
  flags: default 0
    MSG_DONTWAIT on unix systems for nonblocking, sends signal you must catch

  returns: size of data or -1 for timeout
  */
  int recvfrom(void *dst, size_t size, sockaddr_t &from_addr,
               const int flags = 0) {

    // blocking recvfrom
    memset(dst, 0, size); // clear buffer

    // from_addr             = {0};
    // unsigned int from_len = sizeof(from_addr);
    // int flags = MSG_DONTWAIT; // nonblocking - sends signal you must catch
    // const int flags = 0;

    // block waiting to receive a packet
    // int recv_len = ::recvfrom(socket_fd, dst, (size_t)size, flags,
    //                           (struct sockaddr *)&from_addr, &from_len);

    int recv_len = ::recvfrom(socket_fd, dst, size, flags, (struct sockaddr *)NULL, NULL);

    // guard(recv_len, "recvfrom() failed"); // timeout gives -1 ... avoid
    // needless throws

    return recv_len;
  }

  // this is for sending Ascii/ascii_t messages
  inline int sendto(const std::string &sdata, const sockaddr_t &addr) {
    return sendto((void*) sdata.data(), sdata.size(), addr);
  }

  int sendto(const void *data, int size, const sockaddr_t &addr) {
    int num = ::sendto(socket_fd, data, (size_t)size, 0,
                       (struct sockaddr *)&addr, sizeof(addr));

    guard(size != num, "sendto() sent incorrect number of bytes");
    return num;
  }

  void setsockopt(int level, int name, int val) {
    int err = ::setsockopt(socket_fd, level, name, (char *)&val, sizeof(val));
    guard(err, "setsockopt(): ");
  }

protected:
  int timeout;        // msec
  bool blocking;      // are we blocking?
  socket_t socket_fd; // socket file descriptor
  int family, proto, type;

  /**
   * Handles old C socket functions and throws exceptions with error description
   * from errno.
   */
  inline void guard(int err, const std::string &msg) {
    if (err < 0) {
      std::cout << msg + std::string(strerror(int(errno))) << std::endl;
      throw std::runtime_error(msg + std::string(strerror(int(errno))));
    }
  }
};

// [DEPRECATED]
// msgaddr_t recvfrom() {
//   // clear the receive buffers & structs
//   memset(recv_str, 0, sizeof(recv_str)); // clear buffer

//   sockaddr_t from_addr  = {0};
//   unsigned int from_len = sizeof(from_addr);
//   int recv_len          = 0;

//   // block waiting to receive a packet
//   recv_len = ::recvfrom(socket_fd, recv_str, MAX_LEN, 0,
//                         (struct sockaddr *)&from_addr, &from_len);
//   // if (recv_len < 0) {
//   //   // throw SocketError("recvfrom() failed");
//   //   throw SocketError();
//   // }
//   guard(recv_len < 0, "recvfrom() failed");

//   return msgaddr_t({recv_str}, from_addr);
// }

// if timeout set, use select to determine if data ready
// if (/*timeout >*/ 0) {
//     struct timeval tv;
//     fd_set readfds;
//     long sec = 0;
//     long msec = (long)timeout;
//
//     if (msec >= 1000) {
//         sec = msec/1000;
//         msec %= 1000;
//     }
//
//     tv.tv_sec = sec;
//     tv.tv_usec = msec * 1000;
//
//     FD_ZERO(&readfds);
//     FD_SET(socket_fd, &readfds);
//
//     // don't care about writefds and exceptfds:
//     int status = select(sock+1, &readfds, NULL, NULL, &tv);
//     if (status == 0) return false;
//     else if (status < 0) guard(status, "recvfrom(): ");
//
//     // if (FD_ISSET(socket_fd, &readfds) == 0) {
//     //     // cout << "no data" << endl;
//     //     return false;
//     // }
//     // cout << "data!!" << endl;
// }

// class Base {
// public:
//   Base() { ; }

//   void info() { ; }

//   inline void bind(const std::string &addr, int port) {
//     socket.bind(addr, port);
//   }

// protected:
//   UDPSocket socket;
// };

// value?
// bool ready() {
//   struct timeval tv;
//   tv.tv_sec  = 0;
//   tv.tv_usec = 1000;

//   fd_set readfds;
//   FD_ZERO(&readfds);
//   FD_SET(socket_fd, &readfds); // get sock info

//   // don't care about writefds and exceptfds:
//   int status = select(sock + 1, &readfds, NULL, NULL, &tv);
//   if (status == 0) return false;
//   else if (status < 0) guard(status, "recvfrom(): ");
//   return true;
// }

/**
 * Handles old C socket functions and throws SocketError with error description
 * from errno.
 */
// int guard(int err, std::string msg) {
//   if (err < 0) {
//     throw SocketError(msg + std::string(strerror(int(errno))));
//   }
//   return err;
// }

// sockaddr_t make(const string &saddr, int port) {
//   sockaddr_t addr;
//   memset(&addr, 0, sizeof(addr));
//   addr.sin_family      = AF_INET;
//   addr.sin_addr.s_addr = inet_addr(saddr.c_str());
//   addr.sin_port        = htons(port);
//   // cout << ">> Addr: " << inet_ntoa(addr.sin_addr) << ":"
//   // << ntohs(addr.sin_port) << endl;
//   return std::move(addr);
// }

// struct sockaddr_in make(int port, int iaddr){
//     struct sockaddr_in addr = {0};
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(port);
//     addr.sin_addr.s_addr = htonl(iaddr);
//     return std::move(addr);
// }
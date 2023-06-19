/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <arpa/inet.h> // for sockaddr_in
#include <sys/types.h>  // for type definitions like size_t
#include <iostream>
#include <string>
// #include <sys/socket.h> // socket, connect, etc ...
#include <sys/un.h>     // UDS
#include <unistd.h>     // gethostname()
#include <netdb.h>      // gethostbyname

using sockaddr_in_t = struct sockaddr_in;
using udpaddr_t = struct sockaddr_in;
using tcpaddr_t = struct sockaddr_in;
using udsaddr_t = struct sockaddr_un;

static
udsaddr_t make_sockaddr(const std::string &path) {
  udsaddr_t addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, path.c_str());
  return std::move(addr);
}

static
udpaddr_t make_sockaddr(const std::string &ip, uint16_t port) {
  udpaddr_t addr = {0};
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  addr.sin_port        = htons(port);
  return std::move(addr);
}

// inaddr: INADDR_ANY or INADDR_BROADCAST
static
udpaddr_t make_sockaddr(uint32_t inaddr, uint16_t port) {
  udpaddr_t addr = {0};
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = inaddr; // htonl(inaddr); // htonl???
  addr.sin_port        = htons(port);
  return std::move(addr);
}

// https://beej.us/guide/bgnet/html/multi/gethostbynameman.html
// PLEASE NOTE: these two functions are superseded by getaddrinfo() and
// getnameinfo()! In particular, gethostbyname() doesn't work well with IPv6.
static
std::string gethostname() {
  char name[256] = {0};
  // guard(::gethostname(name, 256), "gethostname() failed");
  ::gethostname(name, 256);
  return std::string(name);
}

static
std::string getHostbyName(const std::string& name) {
  struct hostent* host = ::gethostbyname(name.c_str());
  if (host == nullptr) return std::string("");
  std::string hostname(host->h_addr, host->h_length);
  return hostname;
}

static
std::string get_host_ip() {
  int sockfd;
  socklen_t addrlen;

  // Create a socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    throw std::runtime_error("get_host_ip error");
  }

  // you need to connect to get your IP address, even
  // a failed connect works
  udpaddr_t addr  = make_sockaddr("9.9.9.9", 9999);
  ::connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

  // Get the local address
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addrlen              = sizeof(addr);
  if (::getsockname(sockfd, (struct sockaddr *)&addr, &addrlen) < 0) {
    throw std::runtime_error("get_host_ip error");
  }

  // Convert the address to a string
  // strcpy(ip, inet_ntoa(addr.sin_addr));
  std::string address(inet_ntoa(addr.sin_addr));

  // Close the socket
  close(sockfd);

  return address;
}

static
std::string get_ip_port(const udpaddr_t &addr) {
  char ip[32] = {0};
  ::inet_ntop(AF_INET, &(addr.sin_addr),ip,sizeof(ip));
  std::string host(ip,strlen(ip));
  host += ":" + std::to_string(ntohs(addr.sin_port));
  // std::cout << "INADDR_ANY: " << host << ":" << ntohs(addr.sin_port) << std::endl;
  return host;
}


// // Gets geckoUDP_t for connected peer system.
// geckoUDP_t getpeername(int sockfd) {
//   geckoUDP_t addr      = {0};
//   unsigned int addrlen = sizeof(addr);
//   ::getpeername(sockfd, (struct sockaddr *)&addr, &addrlen);
//   return addr;
// }

// // Gets the geckoUDP_t for this system
// geckoUDP_t getsockname(int sockfd) {
//   geckoUDP_t addr      = {0};
//   unsigned int addrlen = sizeof(addr);
//   ::getsockname(sockfd, (struct sockaddr *)&addr, &addrlen);
//   return addr;
// }
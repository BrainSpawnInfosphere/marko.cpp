
#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sys/socket.h>
#include "sockaddr.hpp"

// std::map<int,std::string> socketTypes {
//   {SOCK_DGRAM, "UDP"},
//   {SOCK_RAW, "RAW"},
//   {SOCK_STREAM, "STREAM"}
// };

// enum dataTypes {
//   Int,
//   Bool,
//   Type,
//   Timeval
// };

// struct SoOpts {
//   int val;
//   std::string description;
//   dataTypes type;
// };

// std::vector<SoOpts> socketOptsSOL {
//   {SO_TYPE, "socket type", dataTypes::Type},
//   {SO_ACCEPTCONN, "Accepting connections", dataTypes::Bool}, // int
//   {SO_BROADCAST, "Broadcast messages supported", dataTypes::Bool}, // int
//   // SO_DEBUG, Debugging information is being recorded,
//   {SO_DONTROUTE, "Bypass normal routing", dataTypes::Bool},
//   {SO_ERROR, "Socket error status", dataTypes::Bool}, // int
//   {SO_KEEPALIVE, "Connections are kept alive with periodic messages", dataTypes::Bool}, // int
//   // {SO_LINGER, "Socket lingers on close"}, // linger struct
//   {SO_OOBINLINE, "Out-of-band data is transmitted in line", dataTypes::Bool},
//   // {SO_RCVTIMEO, "receive timeout"}, // timeval
//   {SO_REUSEADDR, "SO_REUSEADDR", dataTypes::Bool},
//   {SO_REUSEPORT, "SO_REUSEPORT", dataTypes::Bool},
//   {SO_RCVBUF, "Receive buffer size", dataTypes::Int},
//   {SO_RCVLOWAT, "receive low water mark", dataTypes::Int},
//   {SO_SNDBUF, "Send buffer size", dataTypes::Int},
//   {SO_SNDLOWAT, "send low water mark", dataTypes::Int},
//   // {SO_SNDTIMEO, "send timeout"}, // timeval
// };

class SocketInfo {
  public:
  SocketInfo(int fd): socket_fd(fd) {}

  enum SocketTypes {
    UDP, UDS, TCP
  };

  void info(const std::string& s, SocketTypes st) {
    printf("[%s Socket] =====================================\n", s.c_str());
    printf("  bind/connect to: %s\n", getsockname().c_str());
    printf("  file descriptor: %d\n", socket_fd);

    if (st == UDP || st == TCP) info_so();
    if (st == UDP) info_ip();
    // if (st == UDS); // nothing yet
  }

  private:
  int socket_fd;

  void info_so(){
    u_char val;
    socklen_t size = sizeof(val);

    // printf("[%s Socket] =====================================\n", s.c_str());
    // printf("  bind/connect to: %s\n", getsockname().c_str());
    // printf("  file descriptor: %d\n", socket_fd);
    printf(" [Socket Layer]-----------------\n");

    for (const auto& m : socketOptsSOL) {
      getsockopt(socket_fd, SOL_SOCKET, m.val, &val, &size);
      if (m.type == dataTypes::Int)
        printf("  %s: %d\n", m.description.c_str(), int(val));
      else if (m.type == dataTypes::Bool)
        printf("  %s: %s\n", m.description.c_str(), val ? "true" : "false");
      else if (m.type == dataTypes::Type)
        printf("  %s: %s\n", m.description.c_str(), socketTypes[val].c_str());
    }
  }

  void info_ip(){
    // https://man7.org/linux/man-pages/man7/ip.7.html
    // https://man7.org/linux/man-pages/man7/tcp.7.html
    // https://man7.org/linux/man-pages/man7/udp.7.html
    u_char val;
    struct in_addr addr = {0};
    socklen_t ss = sizeof(addr);
    socklen_t size = sizeof(val);
    char ip[32] = {0};

    // Socket::info(s);

    printf(" [IP Protocol]-----------------\n");
    getsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &addr, &ss);
    ::inet_ntop(AF_INET, &(addr.s_addr), ip, sizeof(ip));
    printf("  multicast IF: %s\n", ip);

    getsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, &size);
    printf("  multicast loopback: %s\n", val ? "enabled" : "disabled");

    getsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, &val, &size);
    printf("  multicast TTL: %d\n", val);

    getsockopt(socket_fd, IPPROTO_IP, IP_TTL, &val, &size);
    printf("  IP TTL: %d\n", val);
  }

  std::string getsockname() {
    udpaddr_t addr = {0};
    socklen_t addr_len = sizeof(addr);
    int err = ::getsockname(socket_fd, (struct sockaddr*)&addr, &addr_len);
    // guard(err, "getsockname(): ");
    if (err < 0) return std::string();
    return get_ip_port(addr);
  }

  std::map<int,std::string> socketTypes {
  {SOCK_DGRAM, "UDP"},
  {SOCK_RAW, "RAW"},
  {SOCK_STREAM, "STREAM"}
};

enum dataTypes {
  Int,
  Bool,
  Type,
  Timeval
};

struct SoOpts {
  int val;
  std::string description;
  dataTypes type;
};

std::vector<SoOpts> socketOptsSOL {
  {SO_TYPE, "socket type", dataTypes::Type},
  {SO_ACCEPTCONN, "Accepting connections", dataTypes::Bool}, // int
  {SO_BROADCAST, "Broadcast messages supported", dataTypes::Bool}, // int
  // SO_DEBUG, Debugging information is being recorded,
  {SO_DONTROUTE, "Bypass normal routing", dataTypes::Bool},
  {SO_ERROR, "Socket error status", dataTypes::Bool}, // int
  {SO_KEEPALIVE, "Connections are kept alive with periodic messages", dataTypes::Bool}, // int
  // {SO_LINGER, "Socket lingers on close"}, // linger struct
  {SO_OOBINLINE, "Out-of-band data is transmitted in line", dataTypes::Bool},
  // {SO_RCVTIMEO, "receive timeout"}, // timeval
  {SO_REUSEADDR, "SO_REUSEADDR", dataTypes::Bool},
  {SO_REUSEPORT, "SO_REUSEPORT", dataTypes::Bool},
  {SO_RCVBUF, "Receive buffer size", dataTypes::Int},
  {SO_RCVLOWAT, "receive low water mark", dataTypes::Int},
  {SO_SNDBUF, "Send buffer size", dataTypes::Int},
  {SO_SNDLOWAT, "send low water mark", dataTypes::Int},
  // {SO_SNDTIMEO, "send timeout"}, // timeval
};
};
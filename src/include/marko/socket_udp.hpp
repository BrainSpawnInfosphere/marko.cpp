
#pragma once


#include "socket.hpp"


class SocketUDP: public Socket {
  public:
  SocketUDP() {}
  ~SocketUDP() {}

  void bind(const std::string &ip, int port, bool multipleBind=false) {
    makeSocket(AF_INET, SOCK_DGRAM, 0);

    // allow multiple sockets to re-use the same address and port
    if (multipleBind) {
      setsockopt(SOL_SOCKET, SO_REUSEPORT, 1);
      setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    }
    else {
      setsockopt(SOL_SOCKET, SO_REUSEPORT, 0);
      setsockopt(SOL_SOCKET, SO_REUSEADDR, 0);
    }

    // setsockopt(IPPROTO_IP, IP_MULTICAST_LOOP, 0); // disable loopback?

    udpaddr_t addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // all interfaces
    addr.sin_port        = htons(port);

    int err = ::bind(socket_fd, (const struct sockaddr *)&addr, sizeof(addr));
    guard(err, "SocketUDP::bind() failed: ");

    // info("Bind");
  }

  void connect() {
    makeSocket(AF_INET, SOCK_DGRAM, 0);
    // info("Connect");
  }

  void listen() {
    int err = ::listen(socket_fd, 5);
    guard(err, "SocketUDP::listen error: ");
  }

  void accept() {

  }

  message_t recvfrom(size_t msg_size, udpaddr_t *from_addr, const int flags=0) {
    message_t dst(msg_size);
    int num = 0;

    if (from_addr != NULL) {
      socklen_t from_len = sizeof(*from_addr);
      memset(from_addr, 0, from_len);
      num = ::recvfrom(socket_fd,
        dst.data(), msg_size,
        flags,
        (struct sockaddr *)from_addr, &from_len);
    }
    else
      num = ::recvfrom(socket_fd, dst.data(), msg_size, flags, NULL, NULL);

    // std::cout << "recvfrom done msg: " << dst.capacity() << " " << dst.size() << std::endl;

    // FIXME: add msg_size != dst.size() ???
    // timeout gives -1, so set size=0
    if (num == SOCKET_TIMEOUT || num == 0) dst.clear();

    return std::move(dst);
  }

  message_t recv(size_t msg_size, const int flags=0) {
    message_t m = recvfrom(msg_size,NULL,flags);
    return std::move(m);
  }

  int sendto(const message_t& msg, const udpaddr_t &addr, int flags=0) {
    // std::cout << "sendto" << std::endl;
    int num = ::sendto(
      socket_fd,
      msg.data(), msg.size(),
      flags,
      (struct sockaddr *)&addr, sizeof(addr));

    guard(msg.size() != num, "sendto() sent incorrect number of bytes");
    // std::cout << "sendto done" << std::endl;
    return num;
  }

  // static udpaddr_t getsockaddr(const int port) {
  //   udpaddr_t addr = {0};
  //   addr.sin_family      = AF_INET;
  //   addr.sin_addr.s_addr = INADDR_ANY;
  //   addr.sin_port        = htons(port);
  //   return std::move(addr);
  // }

  // static udpaddr_t getsockaddr(const std::string& ip, int port) {
  //   udpaddr_t addr = {0};
  //   addr.sin_family      = AF_INET;
  //   addr.sin_addr.s_addr = inet_addr(ip.c_str());
  //   addr.sin_port        = htons(port);
  //   return std::move(addr);
  // }

  void info(const std::string& s){
    // https://man7.org/linux/man-pages/man7/ip.7.html
    // https://man7.org/linux/man-pages/man7/tcp.7.html
    // https://man7.org/linux/man-pages/man7/udp.7.html
    u_char val;
    struct in_addr addr = {0};
    socklen_t ss = sizeof(addr);
    socklen_t size = sizeof(val);
    char ip[32] = {0};

    Socket::info(s);

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
};
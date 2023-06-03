/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once


#include <unistd.h> // gethostname()
#include <arpa/inet.h> // for sockaddr_in
#include <sys/socket.h> // socket, connect, etc ...
#include <iostream>
#include <string>

typedef struct sockaddr_in sockaddr_t;


sockaddr_t make_sockaddr(const std::string &saddr, int port) {
  sockaddr_t addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = inet_addr(saddr.c_str());
  addr.sin_port        = htons(port);
  return std::move(addr);
}

std::string gethostname() {
  char name[256] = {0};
  // guard(::gethostname(name, 256), "gethostname() failed");
  ::gethostname(name, 256);
  return std::string(name);
}

std::string get_host_ip() {
  int sockfd;
  sockaddr_t addr;
  socklen_t addrlen;

  // Create a socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    throw std::runtime_error("get_host_ip error");
  }

  // you need to connect to get your IP address, even
  // a failed connect works
  sockaddr_t aaddr = make_sockaddr("9.9.9.9", 9999);
  ::connect(sockfd, (struct sockaddr*)&aaddr, sizeof(aaddr));

  // Get the local address
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addrlen = sizeof(addr);
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
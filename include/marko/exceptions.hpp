#pragma once

#include <exception>
#include <string>

// Based on python's exception
struct NotImplemented : public std::exception {
  NotImplemented(const std::string &s) : msg(s) {}
  NotImplemented() : msg("Not implemented") {}
  const char *what() const throw() { return msg.c_str(); }

protected:
  std::string msg;
};

/**
 * https://en.cppreference.com/w/cpp/error/exception
 */
class SocketError : public std::exception {
public:
  SocketError(const std::string &s) : msg(s) {}
  SocketError() : msg("Socket Error") {}
  const char *what() const throw() { return msg.c_str(); }

protected:
  std::string msg;
};

/*
https://beej.us/guide/bgnet/html/multi/gethostbynameman.html
PLEASE NOTE: these two functions are superseded by getaddrinfo() and
getnameinfo()! In particular, gethostbyname() doesn't work well with IPv6.
*/
struct HostnameError : public std::exception {
  HostnameError(const std::string &s)
      : msg("Couldn't determine hostname: " + s) {}
  HostnameError() : msg("Couldn't determine hostname") {}
  const char *what() const throw() { return msg.c_str(); }

protected:
  std::string msg;
};

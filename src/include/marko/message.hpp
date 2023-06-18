#pragma once

#include <vector>
#include <iostream>
#include <ostream>

using message_t = std::vector<uint8_t>;

template<typename T>
message_t pack(const T& d) {
  message_t msg(sizeof(d));
  memcpy(static_cast<uint8_t*>(msg.data()), &d, sizeof(d));
  return std::move(msg);
}

template<typename T>
T unpack(const message_t& m) {
  T d;
  memcpy(&d, m.data(), m.size());
  return std::move(d);
}

void print_msg(const message_t& msg) {
  for (const uint8_t& m: msg) std::cout << int(m) << " ";
  std::cout << std::endl;
}

std::ostream &operator<<(std::ostream &os, message_t const &msg) {
  os << "[";
  for (const uint8_t& m: msg) os << int(m) << ",";
  return os << "]";
}
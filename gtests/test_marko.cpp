// https://google.github.io/googletest/primer.html
#include <gtest/gtest.h>
#include <marko/marko.hpp>
#include <string>
#include <iostream>

using namespace std;

struct data_t { int a; };

// data_t test_data[LOOP]{{1},{2},{3},{4},{5}};

TEST(marko, addresses) {
  int port = 9999;
  string host = get_host_ip();
  udpaddr_t addr = make_sockaddr(host, port);
}

TEST(marko, ascii) {
  Ascii a;
  string orig{"bob|tom|alice|george"};
  ascii_t msg = a.unpack(orig);
  EXPECT_TRUE("bob" == msg[0]);
  EXPECT_TRUE("tom" == msg[1]);
  EXPECT_TRUE("alice" == msg[2]);
  EXPECT_TRUE("george" == msg[3]);
  string redo = a.pack(msg);
  EXPECT_TRUE(redo == orig);
  // cerr << "ascii" << endl;
}

TEST(marko, message_t) {
  data_t d{5};
  message_t m = pack<data_t>(d);
  data_t e = unpack<data_t>(m);
  EXPECT_EQ(d.a, e.a);
  EXPECT_EQ(sizeof(d), sizeof(e));
}
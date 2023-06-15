// https://google.github.io/googletest/primer.html
#include <gtest/gtest.h>
#include <marko/marko.hpp>
#include <string>
#include <vector>

using namespace std;

// Demonstrate some basic assertions.
// TEST(marko, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
// }

TEST(marko, addresses) {
  string HOST = get_host_ip();
  int PORT = 9999;
  sockaddr_t addr = make_sockaddr(HOST, PORT);
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
}

struct data_t {
  double a;
};

// TEST(marko, pub_sub_uds) {
//   PublisherUDS<data_t> p;
//   SubscriberUDS<data_t> s;

//   string path = "./socket.uds";
//   unlink(path.c_str()); // remove from filesystem

//   PublisherUDS<data_t> pp;
//   pp.bind(path);

//   SubscriberUDS<data_t> ss;
//   // ss.bind(path);
//   ss.connect(path);
// }

TEST(marko, pub_sub_udp) {
  PublisherUDP<data_t> p;
  SubscriberUDP<data_t> s;

  string host = "127.0.0.1";
  int port = 9999;

  PublisherUDP<data_t> pp;
  pp.bind(host, port);

  SubscriberUDP<data_t> ss;
  // ss.bind(path);
  ss.connect(host, port);
}

TEST(marko, request_reply) {
  ReplyUDP<data_t,data_t> p;
  RequestUDP<data_t,data_t> s;
  ReplyUDS<data_t,data_t> pp;
  RequestUDS<data_t,data_t> ss;
}
// https://google.github.io/googletest/primer.html
#include <gtest/gtest.h>
#include <marko/marko.hpp>
#include <string>
#include <vector>
#include <thread>
#include <iostream>

using namespace std;

string file = "./uds";
string host = "127.0.0.1";
constexpr int port = 9999;
constexpr int LOOP = 5;

struct data_t { int a; };

data_t test_data[LOOP]{{1},{2},{3},{4},{5}};

TEST(marko, addresses) {
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
}

TEST(marko, message_t) {
  data_t d{5};
  message_t m = pack<data_t>(d);
  data_t e = unpack<data_t>(m);
  EXPECT_EQ(d.a, e.a);
  EXPECT_EQ(sizeof(d), sizeof(e));
}

void callback(const message_t& m) {
  static int i = 0;
  data_t d = unpack<data_t>(m);
  EXPECT_EQ(d.a, test_data[i++].a);
  // cout << i << endl;
}

void sub_thread() {
  // cout << "start sub" << endl;
  SubscriberUDP s(sizeof(data_t));
  s.bind(host, port);
  s.register_cb( callback );
  for (int i=0; i < LOOP; ++i) {
    s.once();
  }
}

void pub_thread() {
  udpaddr_t addr = make_sockaddr(host,port);
  // cout << "start pub" << endl;
  PublisherUDP p;
  p.register_addr(addr);
  p.connect(host, port);
  for (int i=0; i < LOOP; ++i) {
    message_t m = pack<data_t>(test_data[i]);
    p.publish(m);
    // cout << "publish " << i << endl;
  }
}

TEST(marko, pub_sub_udp) {
  thread subth(sub_thread);
  thread pubth(pub_thread);

  subth.join();
  pubth.join();
}

// TEST(marko, request_reply) {
//   ReplyUDP p;
//   RequestUDPs;
//   // ReplyUDS<data_t,data_t> pp;
//   // RequestUDS<data_t,data_t> ss;
// }




// TEST(marko, pub_sub_uds) {
//   PublisherUDS p;
//   SubscriberUDS s;

//   string path = "./socket.uds";
//   unlink(path.c_str()); // remove from filesystem

//   PublisherUDS<data_t> pp;
//   pp.bind(path);

//   SubscriberUDS<data_t> ss;
//   // ss.bind(path);
//   ss.connect(path);
// }

// void cb(const message_t &s) {
//   data_t d = unpack<data_t>(s);
// }

// TEST(marko, pub_sub_udp) {
//   // Event e;

//   // PublisherUDP<data_t> p;
//   // SubscriberUDP<data_t> s;

//   string host = "127.0.0.1";
//   int port = 9999;
//   udpaddr_t addr = geckoUDP(host, port);

//   PublisherUDP p;
//   p.register_addr(addr);
//   p.bind(host, port);
//   // std::cerr << "bind" << std::endl;
//   // p.sendto((void*)&d,sizeof(d), addr);
//   // p.publish(d);

//   SubscriberUDP s(sizeof(data_t));
//   s.register_cb(cb);
//   s.connect(host, port);
//   // s.settimeout(1000);
//   // std::cerr << "connect" << std::endl;

//   data_t d{1.234};
//   message_t msg = pack<data_t>(d);
//   p.publish(msg);
//   // std::cerr << "publish" << std::endl;
//   // s.once();
//   // std::cerr << "once" << std::endl;

//   // p.sendto()
// }

// TEST(marko, request_reply) {
//   ReplyUDP<data_t,data_t> p;
//   RequestUDP<data_t,data_t> s;
//   ReplyUDS<data_t,data_t> pp;
//   RequestUDS<data_t,data_t> ss;
// }
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

void callback(const message_t& m) {
  static int i = 0;
  data_t d = unpack<data_t>(m);
  EXPECT_EQ(d.a, test_data[i++].a);
  // cout << i << endl;
}

void sub_thread() {
  // cout << "start sub" << endl;
  SubscriberUDP s(sizeof(data_t));
  // s.bind(port);
  s.bind("udp://*:" + to_string(port));
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
  // p.connect(host, port);
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
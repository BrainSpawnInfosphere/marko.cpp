#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <marko/marko.hpp>

using namespace std;

string HOST = get_host_ip();
int PORT    = 9999;

// publish data
struct data_t {
  double a;
  int b;
  void print() const {
    cout << "data_t " << a << " " << b << " size " << sizeof(*this) << endl;
  }
};

// publisher
void pub() {
  sockaddr_t hp = make_sockaddr(HOST, PORT);
  cout << "Publisher connecting to: " << HOST << ":" << PORT << endl;

  PublisherUDP<data_t> p;
  p.register_addr(hp);

  for (int i = 0; i < 20; ++i) {
    data_t d;
    d.a = 1.2;
    d.b = i;
    p.publish(d);
  }
}

// subscriber callback, this would process
// recieved data from the publisher, but this
// only prints it.
void cb(const data_t &s) { s.print(); }

// subscriber
void sub() {
  Event e;
  e.set();

  cout << "Subscriber binding to: " << HOST << ":" << PORT << endl;

  SubscriberUDP<data_t> s;
  s.bind(HOST, PORT);
  s.register_cb(cb); // you can have many callback functions
  s.loop(e);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: ./ps p|s" << endl;
    return 1;
  }

  string arg(argv[1]);
  if (arg == "p") pub();
  else if (arg == "s") sub();
  else cout << "Usage: ./ps p|s" << endl;

  return 0;
}

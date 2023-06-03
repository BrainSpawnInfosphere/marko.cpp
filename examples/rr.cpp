#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <marko/marko.hpp>

using namespace std;

string HOST = get_host_ip();
int PORT = 9999;

// request message
struct request_t {
  double a;
  int b;
  uint8_t c[128];
};

// response message
struct response_t {
  int a;
};

// requester
void request() {
  sockaddr_t addr = make_sockaddr(HOST, PORT);

  RequestUDP<request_t, response_t> r;
  request_t msg{1.1, 2};
  msg.c[0]        = 100;
  response_t resp = r.request(msg, addr);

  printf("resp: %d\n", resp.a);
}

// response callback, this processes the
// request message and returns a response message
response_t cb(const request_t &s) {
  printf("cb: %f %d %d %lu\n", s.a, s.b, s.c[0], sizeof(s));
  response_t r;
  r.a = 1;
  return r;
}

// reply
void reply() {
  Event e;
  e.set();
  ReplyUDP<request_t, response_t> r;
  r.bind(HOST, PORT);
  r.register_cb(cb); // you can have more than 1 callback
  r.loop(e);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: ./rr reply|request" << endl;
    return 1;
  }

  string arg(argv[1]);
  if (arg == "reply") reply();
  else if (arg == "request") request();
  else cout << "Usage: ./rr reply|request" << endl;

  return 0;
}

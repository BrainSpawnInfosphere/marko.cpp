#include <cstdio>
#include <iostream>
#include <marko/marko.hpp>
#include <string>
#include <vector>

using namespace std;

string HOST = get_host_ip();
int PORT    = 9999;

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
  geckoUDP_t addr = geckoUDP(HOST, PORT);
  cout << "Request connecting to: " << HOST << ":" << PORT << endl;

  RequestUDP<request_t, response_t> r;
  r.connect(HOST, PORT);
  request_t msg{1.1, 2};
  msg.c[0]        = 100;
  response_t resp = r.request(msg, addr);

  printf("resp: %d\n", resp.a);
}

// response callback, this processes the
// request message and returns a response message
response_t cb(const request_t &s) {
  static int a = 0;
  a += 1;
  printf("cb: %f %d %d %lu\n", s.a, s.b, s.c[0], sizeof(s));
  response_t r;
  r.a = a;
  return r;
}

// reply
void reply() {
  cout << "Reply binding to: " << HOST << ":" << PORT << endl;
  ReplyUDP<request_t, response_t> r;
  r.bind(HOST, PORT);
  // r.settimeout(1000);
  r.register_cb(cb); // you can have more than 1 callback
  r.loop();
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

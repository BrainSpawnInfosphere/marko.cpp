#include <cstdio>
#include <iostream>
#include <marko/marko.hpp>
#include <string>
#include <vector>

using namespace std;

string HOST = get_host_ip();
int PORT    = 9999;
constexpr int LOOP = 5;

// request message
struct request_t {
  double a;
  int b;
};

// response message
struct response_t {
  int a;
};

// requester
void request() {
  udpaddr_t addr = make_sockaddr(HOST, PORT);
  cout << "Request connecting to: " << HOST << ":" << PORT << endl;

  RequestUDP<request_t, response_t> r;
  r.connect(HOST, PORT);

  for (int i=0; i<LOOP; ++i) {
    request_t msg{1.2345*double(i), i};
    response_t resp = r.request(msg, addr);
    printf("resp: %d\n", resp.a);
  }
}

// response callback, this processes the
// request message and returns a response message
response_t cb(const request_t &s) {
  printf("cb: %f %d %lu\n", s.a, s.b, sizeof(s));
  response_t r;
  r.a = s.b;
  return r;
}

// reply
void reply() {
  cout << "Reply binding to: " << HOST << ":" << PORT << endl;
  ReplyUDP<request_t, response_t> r;
  r.bind(HOST, PORT);
  // r.settimeout(1000);
  r.register_cb(cb); // you can have more than 1 callback
  for (int i=0; i<LOOP; ++i) r.once();
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

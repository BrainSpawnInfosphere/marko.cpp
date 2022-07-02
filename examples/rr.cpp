#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include <marko/udpsocket.hpp>
#include <marko/utils.hpp>

using namespace std;

string HOST{"10.0.1.116"};
int PORT = 9999;

typedef struct Data {
    double a;
    int b;
    uint8_t c[128];
} data_t;

typedef struct RESP {
    int a;
} response_t;


void request(){
    sockaddr_t addr = make(HOST,PORT);

    TRequest<data_t,response_t> r;
    data_t msg{1.1,2};
    msg.c[0] = 100;
    response_t resp = r.request(msg, addr);

    printf("resp: %d\n", resp.a);
}

response_t cb(const data_t& s){
    printf("cb: %f %d %d %lu\n",s.a,s.b,s.c[0],sizeof(s));
    response_t r;
    r.a = 1;
    return r;
}

void reply(){
    Event e;
    e.set();
    TReply<data_t,response_t> r;
    r.bind(HOST,PORT);
    r.register_cb(cb);
    r.loop(e);
}

int main(int argc, char *argv[]){
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

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <marko/templates/udpsocket.hpp>
#include <marko/packer.hpp>
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

    cout << resp.a << endl;
}

response_t cb(const data_t& s){
    cout << "cb "<< s.a << " " << s.b << " " << s.c[0] << " "<< sizeof(s) << endl;
    response_t r;
    r.a = 1;
    return r;
}

void reply(){
    Event e;
    e.set();
    TReply<data_t,response_t> r(HOST,PORT);
    r.register_cb(cb);
    r.loop(e);
}

//
// void request(){
//
//     // Packer packer;
//     bytearray b;
//     sockaddr_t addr = make(HOST,PORT);
//
//     Request r;
//     b.pack("hello");
//     string msg = b.to_string();
//     string ans = r.request(msg, addr);
//
//     cout << ans << endl;
//     cout << sizeof("hello") << endl;
//     cout << sizeof(ans) << " " << ans.size() << endl;
//     cout << "msg " << msg << " " << msg.size() << endl;
//     cout << sizeof(b) << endl;
//
//     typedef struct MSG {
//         char a,b,c,d;
//         float e,f,g;
//     } msg_t;
//
//     TSubscriber<msg_t> sub;
//
//     msg_t m;
//     b.pack(&m, sizeof(m));
//     b.to_struct(ans, &m);
//     cout << m.e << endl;
// }
//
// string cb(const string& s){
//     bytearray packer;
//     cout << s << endl;
//     packer.packf("ccccfff",'a','b','c','d',1.2,2.3,3.4);
//
//     // string ss(packer.buffer.c, 3*sizeof(float));
//     string ss = packer.to_string();
//     return ss;
// }
//
// void reply(){
//     Reply r(HOST,PORT);
//     r.register_cb(cb);
//     r.loop();
// }

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

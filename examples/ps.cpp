#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <marko/templates/udpsocket.hpp>
#include <marko/utils.hpp>

using namespace std;

string HOST{"10.0.1.116"};
int PORT = 9999;

typedef struct Data {
    double a;
    int b;
    void print() const {
        cout << "data_t " << a << " " << b << " size " << sizeof(*this) << endl;
    }
} data_t;

void pub(){

    HostPort hp(HOST, PORT);
    cout << "Connecting to: " << hp << endl;

    TPublisher<data_t> p;
    p.clientaddr.push_back(hp.caddr);

    for (int i=0; i<20; ++i){
        data_t d;
        d.a = 1.2;
        d.b = i;
        p.publish(d);
    }
}


void cb(const data_t& s){
    s.print();
}

void sub(){
    Event e;
    e.set();

    TSubscriber<data_t> s;
    s.bind(HOST, PORT);
    s.register_cb(cb);
    s.loop(e);
}

int main(int argc, char *argv[]){
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

# Marko

A simple library to do reply and request over UDP. This does either multicast
or broadcast to send/receive a message.

> **Unicast:** point-to-point communications, can be UDP or TCP

> **Multicast:** one-to-many (group) communications, this can only be UDP

## Usage

This library requires a C++17 compiler and an up to date `cmake`.

```
mkdir build
cd build
cmake ..
```
## Template Examples

```c++
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <marko/templates/udpsocket.hpp>

using namespace std;

string HOST{"10.0.1.116"};
int PORT = 9999;

// request message
typedef struct Data {
    double a;
    int b;
    uint8_t c[128];
} data_t;

// resply message
typedef struct RESP {
    int a;
} response_t;


void request(){
    sockaddr_t addr = make(HOST,PORT);

    // create message
    data_t msg{1.1,2};
    msg.c[0] = 100;

    TRequest<data_t,response_t> r;
    response_t resp = r.request(msg, addr);

    cout << "Response: " << resp.a << endl;
}

// callback function, would realistically do much more, but just
// a demo
response_t cb(const data_t& s){
    cout << "cb got: "<< s.a << " " << s.b << " " << s.c[0] << " "<< sizeof(s) << endl;

    // create message to return
    response_t r;
    r.a = 1;
    return r;
}

void reply(){
    TReply<data_t,response_t> r(HOST,PORT);
    r.register_cb(cb);
    r.loop();
}

int main(int argc, char *argv[]){
    // check args
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
```

## Alternate

- https://github.com/nanomsg/nng

# MIT License

**Copyright (c) 2019 Kevin J. Walchko**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

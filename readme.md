# Marko

A simple library to do reply and request over UDP. This does either multicast
or broadcast to send/receive a message.

> **Unicast:** point-to-point communications, can be UDP or TCP

> **Multicast:** one-to-many (group) communications, this can only be UDP

## Features

- `Socket` modelled after python socket module
    - UDP and UDS
    - blocking/non-blocking
- `Publisher/Subscriber/Reply/Request`
    - one way: `Publisher` -> `Subscriber`
    - bi-directional: `Reply` <-> `Request`
    - all messaging uses `message_t` for data

## Usage

This library requires a C++17 compiler and an up to date `cmake`.

```
mkdir build
cd build
cmake ..
```

## Messsages

Message are just `struct`s that are packed to remove padding. You want
them to be **fixed length** though. Then they are put into `message_t`
which are really just `std::vector<uint8_t>` for transmitting.

```c
struct __attribute__((packed)) request_t {
  double a; // 8B
  int b;    // 4B
};

request_t r{1.234,22};
sizeof(r); // 12B
message_t msg = pack<request_t>(r);
sizeof(msg); // 12B
request_t rr = unpack<request_t>(msg);
```


## Alternate

- [ZeroMQ](https://zeromq.org/)
- [nanomsg-next-gen](https://github.com/nanomsg/nng)

## Todo

- [ ] Include Unix Domain Sockets in pub/sub/reply/request

# MIT License

**Copyright (c) 2014 Kevin J. Walchko**

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

/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#pragma once


#include <functional> // std::function
#include <string>
#include <vector>
#include <marko/socket.hpp>
#include <iostream>

// // https://github.com/troydhanson/network/blob/master/unixdomain/01.basic/srv.c
// // https://gist.github.com/Phaiax/ae7d1229e6f078457864dae712c51ae0
// class UDSSocket: public Socket {
// public:
//     UDSSocket(int timeout=0): Socket(AF_UNIX, SOCK_STREAM, 0){}
// };

/*
 * Simple UDP socket base class for query/response architecture. Not really
 * useful by itself. Look at classes that build upon it.
 */
class UDPSocket: public Socket {
public:
    UDPSocket(int timeout=0): Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP){}
};


class Base {
public:
    Base(){;}

    void info(){;}
    void bind(const std::string& addr, int port) {
        socket.bind(addr, port);
    }

protected:
    UDPSocket socket;
};

typedef std::function<void(const std::string&)> SubCallback_t;

class Subscriber: public Base {
public:
    Subscriber(){;}
    ~Subscriber(){;}

    void register_cb(SubCallback_t f){
        cb.push_back(f);
    }
    void loop(){
        while (true) {
            msgaddr_t ret = this->socket.recvfrom();
            for (const auto& callback: this->cb){
                callback(ret.data);
            }
        }
    }

protected:
    std::vector<SubCallback_t> cb;
};

class Publisher: public Base {
public:
    Publisher(){;}
    ~Publisher(){;}

    void publish(const std::string& data){
        for(const auto& addr: clientaddr){
            socket.sendto(data, addr);
        }
    }

    std::vector<sockaddr_t> clientaddr;
};

typedef std::function<std::string(const std::string&)> ReplyCallback_t;

class Reply: public Base {
public:
    Reply(const std::string& addr, int port){
        socket.bind(addr, port);
    }
    ~Reply(){}

    void register_cb(ReplyCallback_t f){
        cb.push_back(f);
    }
    void loop(){
        while (true) {
            msgaddr_t ret = this->socket.recvfrom();
            for (const auto& callback: this->cb){
                std::string reply = callback(ret.data);
                socket.sendto(reply, ret.address);
            }
        }
    }

protected:
    std::vector<ReplyCallback_t> cb;
};

class Request: public Base {
public:
    Request(){}
    ~Request(){}

    std::string request(
            const std::string& data,
            const sockaddr_t& addr){
        socket.sendto(data, addr);
        msgaddr_t ret = socket.recvfrom();
        return ret.data;
    }
};

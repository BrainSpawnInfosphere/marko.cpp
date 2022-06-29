/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#pragma once

#include <functional> // std::function
#include <string>
#include <vector>
#include <marko/udpsocket.hpp>
#include <iostream>


////////////////////////////////////////////////////////
//       TEMPLATES
////////////////////////////////////////////////////////


template <typename T>
class TSubscriber: public Base {
public:
    TSubscriber(){;}
    ~TSubscriber(){;}

    typedef std::function<void(const T&)> TSubCallback_t;

    void register_cb(TSubCallback_t f){
        cb.push_back(f);
    }
    void loop(){
        T s;
        sockaddr_t from_addr;
        while (true) {
            this->socket.recvfrom(&s, sizeof(s), from_addr);
            for (const auto& callback: this->cb){
                callback(s);
            }
        }
    }

protected:
    std::vector< TSubCallback_t > cb;
};

template <typename T>
class TPublisher: public Base {
public:
    TPublisher(){}
    ~TPublisher(){}

    void publish(const T& data){
        for(const auto& addr: clientaddr){
            socket.sendto(&data, sizeof(T), addr);
        }
    }

    std::vector<sockaddr_t> clientaddr;
};


template <typename T, typename V>
class TReply: public Base {
public:
    TReply(const std::string& addr, int port){
        socket.bind(addr, port);
    }
    ~TReply(){}

    typedef std::function<V(const T&)> TReplyCallback_t;

    void register_cb(TReplyCallback_t f){
        cb.push_back(f);
    }
    void loop(){
        T s;
        sockaddr_t addr;
        while (true) {
            socket.recvfrom(&s, sizeof(T), addr);
            for (const auto& callback: this->cb){
                V reply = callback(s);
                socket.sendto(&reply, sizeof(V), addr);
            }
        }
    }

protected:
    std::vector<TReplyCallback_t> cb;
};

template <typename T, typename V>
class TRequest: public Base {
public:
    TRequest(){}
    ~TRequest(){}

    V request(const T& data, const sockaddr_t& addr){
        socket.sendto(&data, sizeof(T), addr);
        V reply;
        sockaddr_t tmp;
        socket.recvfrom(&reply, sizeof(V), tmp);
        return reply;
    }
};

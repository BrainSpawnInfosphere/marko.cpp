/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#pragma once

#include <functional> // std::function
#include <string>
#include <vector>
#include <iostream>

#include <marko/udpsocket.hpp>
#include <marko/utils.hpp>


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
    // void loop() {
    //     Event event;
    //     loop(event);
    // }
    void loop(Event& event){
        T s;
        sockaddr_t from_addr;
        while (event.is_set()) {
            this->socket.recvfrom(&s, sizeof(s), from_addr);
            for (const auto& callback: this->cb){
                callback(s);
            }
        }
    }

    // void shutdown() {
    //     event.clear();
    // }

protected:
    std::vector< TSubCallback_t > cb;
    // Event event;
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


template <typename REQ, typename REPLY>
class TReply: public Base {
public:
    TReply(const std::string& addr, int port){
        socket.bind(addr, port);
    }
    ~TReply(){}

    typedef std::function<REPLY(const REQ&)> TReplyCallback_t;

    void register_cb(TReplyCallback_t f){
        cb.push_back(f);
    }
    void loop(Event& event){
        REQ s;
        sockaddr_t addr;
        while (event.is_set()) {
            socket.recvfrom(&s, sizeof(REQ), addr);
            for (const auto& callback: this->cb){
                REPLY reply = callback(s);
                socket.sendto(&reply, sizeof(REPLY), addr);
            }
        }
    }

    // void shutdown() {
    //     event.clear();
    // }

protected:
    std::vector<TReplyCallback_t> cb;
    // Event event;
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

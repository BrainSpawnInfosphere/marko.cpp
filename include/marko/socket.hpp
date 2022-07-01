/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/
// references that helped:
// http://www.kohala.com/start/mcast.api.txt
// http://www.kohala.com/start/
// https://rg3.name/201504241907.html
// https://www.cs.odu.edu/~cs779/spring17/lectures/multicasting.html
// https://beej.us/guide/bgnet/html//index.html#structs

#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>  // for sockaddr_in
#include <string>
// #include <exception>
#include <iostream>
#include "exceptions.hpp"

constexpr int MAX_LEN = 256;

typedef int SOCKET;
// constexpr int INVALID_SOCKET = -1;
// constexpr int SOCKET_ERROR = -1;
// constexpr int NO_ERROR = 0;
typedef struct sockaddr_in sockaddr_t;


// using MsgAddr = std::tuple<std::string, struct sockaddr_in>;
sockaddr_t make(const std::string& saddr, int port);
// struct sockaddr_in make(int port, int iaddr);
// FIXME: not sure I like this ... was a tuple
typedef struct cMsgAddr {
    cMsgAddr(
        const std::string& sdata,
        const sockaddr_t& addr
    ): data(sdata), address(addr){}

    sockaddr_t address;
    std::string data;
} msgaddr_t;

/**
 * useful? replaces sockaddr_t and make()
 */
class HostPort {
public:
    HostPort(const sockaddr_t& addr):
            addr(inet_ntoa(addr.sin_addr)),
            port(ntohs(addr.sin_port)), caddr(addr) {}

    HostPort(const std::string& addr, int port): addr(addr), port(port) {
        caddr = make(addr, port);
    }

    friend std::ostream& operator<<(std::ostream& os, const HostPort& d) {
        os << "(" << d.addr << ":" << d.port << ")";
        return os;
    }

    std::string addr;
    int port;
    sockaddr_t caddr;
};

// std::string host_ip(){
//     char hostbuffer[256];
//     char *IPbuffer;
//     struct hostent *host_entry;
//
//     // To retrieve hostname
//     int err = gethostname(hostbuffer, sizeof(hostbuffer));
//     string hostname = hostbuffer;
//
//     // see if .local is in hostname
//     if (hostname.find(".local") == string::npos) hostname += ".local";
//
//     // To retrieve host information
//     host_entry = gethostbyname(hostname.c_str());
//     // if (host_entry == NULL) throw HostnameError(); // FIXME
//
//     // To convert an Internet network address into ASCII string
//     return inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
// }

/**
 * Simple wrapper C++ around the C networking stuff I use.
 */
class Socket {
public:
    Socket(int family, int type, int proto);

    void close();
    void bind(const std::string& saddr, int port);
    std::string gethostname(); // get host name
    sockaddr_t getpeername();  // get remote address
    sockaddr_t getsockname();  // get local address
    msgaddr_t recvfrom();
    bool recvfrom(void* dst, int size, sockaddr_t& addr);
    // bool recvfrom_nb(void* dst, int size, sockaddr_t& from_addr);
    void sendto(const std::string& sdata, const sockaddr_t& addr);
    void sendto(const void* data, int size, const sockaddr_t& addr);
    void setsockopt(int level, int name, int val);
    void setnonblocking();
    void settimeout(int msec);

    void accept(){throw NotImplemented();}
    void connect(){throw NotImplemented();}
    void listen(){throw NotImplemented();}

    int family, proto, type;
    int timeout; // msec

protected:
    SOCKET sock;
    char recv_str[MAX_LEN];
};

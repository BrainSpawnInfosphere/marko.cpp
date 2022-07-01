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
#include <iostream>
#include "exceptions.hpp"

constexpr int MAX_LEN = 256;

typedef int SOCKET;
typedef struct sockaddr_in sockaddr_t;


// using MsgAddr = std::tuple<std::string, struct sockaddr_in>;

// sockaddr_t make_sockaddr_t(host,port)
sockaddr_t make(const std::string& saddr, int port);

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
struct HostPort {
public:
    HostPort() {
        caddr = {0};
    }

    HostPort(const sockaddr_t& addr):
            addr(inet_ntoa(addr.sin_addr)),
            port(ntohs(addr.sin_port)),
            caddr(addr) {}

    HostPort(const std::string& addr, const int port): addr(addr), port(port) {
        // memset(&caddr, 0, sizeof(caddr));
        caddr = {0};
        caddr.sin_family      = AF_INET;
        caddr.sin_addr.s_addr = inet_addr(addr.c_str());
        caddr.sin_port        = htons(port);
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
 * Simple C++ wrapper around the C networking stuff I use. Modelled after
 * python socket module.
 */
class Socket {
public:
    Socket(int family, int type, int proto);

    void close();
    void bind(const std::string& saddr, int port);
    std::string gethostname(); // get host name
    sockaddr_t getpeername();  // get remote address
    sockaddr_t getsockname();  // get local address
    inline int gettimeout(){return timeout;}
    msgaddr_t recvfrom(); // deprecated
    bool recvfrom(void* dst, int size, sockaddr_t& addr);
    bool ready(); // select to see if recv has info to read
    void sendto(const std::string& sdata, const sockaddr_t& addr);
    void sendto(const void* data, int size, const sockaddr_t& addr);
    void setsockopt(int level, int name, int val);
    void setnonblocking();
    void settimeout(int msec);

    // not sure I need these
    void accept(){throw NotImplemented();}
    void connect(){throw NotImplemented();}
    void listen(){throw NotImplemented();}

    int family, proto, type;

protected:
    int timeout; // msec
    SOCKET sock;
    char recv_str[MAX_LEN]; // remove?
};

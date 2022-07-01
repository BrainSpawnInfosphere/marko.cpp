#include "marko/socket.hpp"
#include <iostream>
#include <unistd.h>     // for close(), gethostname()
#include <errno.h>     // errno
#include <sys/ioctl.h>
#include <cstdio> // printf

using namespace std;

extern int errno; // don't like this global value

/**
 * Handles old C socket functions and throws SocketError with error description
 * from errno.
 */
int guard(int err, std::string msg){
    if (err < 0) {
        throw SocketError(msg + string(strerror( int(errno) )));
    }
    return err;
}

sockaddr_t make(const string& saddr, int port){
    sockaddr_t addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(saddr.c_str());
    addr.sin_port        = htons(port);
    // cout << ">> Addr: " << inet_ntoa(addr.sin_addr) << ":"
    // << ntohs(addr.sin_port) << endl;
    return std::move(addr);
}

// struct sockaddr_in make(int port, int iaddr){
//     struct sockaddr_in addr = {0};
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(port);
//     addr.sin_addr.s_addr = htonl(iaddr);
//     return std::move(addr);
// }


Socket::Socket(int family, int type, int proto) {
    // create a UDP socket
    if ((sock = socket(family, type, proto)) < 0) {
        throw SocketError("Socket() constructor failed");
    }

    this->family = family;
    this->type = type;
    this->proto = proto;
}

// INADDR_ANY - bind to all available interfaces
void Socket::bind(const std::string& saddr, int port){
    // allow multiple sockets to re-use the same address and port
    setsockopt(SOL_SOCKET, SO_REUSEPORT, 1);
    setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);

    sockaddr_t aaddr = make(saddr, port);
    if (::bind(sock, (struct sockaddr*) &aaddr, sizeof(aaddr)) < 0) {
        throw SocketError("bind() failed for ("+saddr+")");
    }
}

void Socket::close(){
    ::close(sock);
}

std::string Socket::gethostname(){
    char name[256] = {0};
    guard(::gethostname(name,256), "gethostname() failed");
    return std::string(name);
}

sockaddr_t Socket::getpeername(){
    sockaddr_t addr = {0};
    unsigned int addrlen = sizeof(addr);
    guard(::getpeername(sock,(struct sockaddr*)&addr,&addrlen), "getsockname(): ");
    return addr;
}

sockaddr_t Socket::getsockname(){
    sockaddr_t addr = {0};
    unsigned int addrlen = sizeof(addr);
    guard(::getsockname(sock,(struct sockaddr*)&addr,&addrlen),"getsockname(): ");
    return addr;
}

/**
 * This will cause a socket recvfrom to return after the timeout period
 */
void Socket::settimeout(int timeout){
    this->timeout = timeout;

    struct timeval tv;
    long sec = 0;
    long msec = 0;

    if (timeout >= 1000) {
        sec = (long)timeout/1000;
        timeout %= 1000;
    }

    tv.tv_sec = sec;
    tv.tv_usec = msec * 1000;

    guard(::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)),
        "settimeout(): ");
}


/**
 * This will throw a signal that must be caught
 */
void Socket::setnonblocking(){
    // int flags = guard(fcntl(sock, F_GETFL), "setblocking(): ");
    // guard(fcntl(sock, F_SETFL, flags | O_NONBLOCK), "setblocking(): ");

    int on = 1;
    guard(ioctl(sock, FIONBIO, (char *)&on), "setnonblocking(): ");
}

// [DEPRECATED]
msgaddr_t Socket::recvfrom(){
    // clear the receive buffers & structs
    memset(recv_str, 0, sizeof(recv_str)); // clear buffer

    sockaddr_t from_addr = {0};
    unsigned int from_len = sizeof(from_addr);
    int recv_len = 0;

    // block waiting to receive a packet
    recv_len = ::recvfrom(
        sock,
        recv_str, MAX_LEN, 0,
        (struct sockaddr*)&from_addr, &from_len
    );
    if (recv_len < 0) {
        throw SocketError("recvfrom() failed");
    }

    return msgaddr_t({recv_str}, from_addr);
}

bool Socket::recvfrom(void* dst, int size, sockaddr_t& from_addr){
    // if timeout set, use select to determine if data ready
    // if (/*timeout >*/ 0) {
    //     struct timeval tv;
    //     fd_set readfds;
    //     long sec = 0;
    //     long msec = (long)timeout;
    //
    //     if (msec >= 1000) {
    //         sec = msec/1000;
    //         msec %= 1000;
    //     }
    //
    //     tv.tv_sec = sec;
    //     tv.tv_usec = msec * 1000;
    //
    //     FD_ZERO(&readfds);
    //     FD_SET(sock, &readfds);
    //
    //     // don't care about writefds and exceptfds:
    //     int status = select(sock+1, &readfds, NULL, NULL, &tv);
    //     if (status == 0) return false;
    //     else if (status < 0) guard(status, "recvfrom(): ");
    //
    //     // if (FD_ISSET(sock, &readfds) == 0) {
    //     //     // cout << "no data" << endl;
    //     //     return false;
    //     // }
    //     // cout << "data!!" << endl;
    // }

    // blocking recvfrom
    memset(dst, 0, size); // clear buffer

    from_addr = {0};
    unsigned int from_len = sizeof(from_addr);
    int recv_len = 0;
    // int flags = MSG_DONTWAIT; // nonblocking - sends signal you must catch
    const int flags = 0;

    // block waiting to receive a packet
    recv_len = ::recvfrom(
        sock,
        dst, (size_t)size, flags,
        (struct sockaddr*)&from_addr, &from_len);

    if (recv_len != size) {
        return false;
    }

    return true;
}

void Socket::sendto(const std::string& sdata, const sockaddr_t& addr){
    if ((::sendto(sock, sdata.c_str(), sdata.size(), 0,
            (struct sockaddr *) &addr, sizeof(addr))) != sdata.size()) {
        throw SocketError("sendto() sent incorrect number of bytes");
    }
}

void Socket::sendto(const void* data, int size, const sockaddr_t& addr){
    int num = ::sendto(sock, data, (size_t) size, 0, (struct sockaddr *) &addr, sizeof(addr));
    if (num != size) {
        throw SocketError("sendto() sent incorrect number of bytes");
    }
}

void Socket::setsockopt(int level, int name, int val){
    guard(::setsockopt(sock, level, name, (char*) &val, sizeof(val)), "setsockopt(): ");
}

// value?
bool Socket::ready(){
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds); // get sock info

    // don't care about writefds and exceptfds:
    int status = select(sock+1, &readfds, NULL, NULL, &tv);
    if (status == 0) return false;
    else if (status < 0) guard(status, "recvfrom(): ");
    return true;
}

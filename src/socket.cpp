#include "marko/socket.hpp"
#include <iostream>
#include <unistd.h>     // for close(), gethostname()

using namespace std;

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

// static
// std::map<int, std::string> debug_setsockopt = {
//     {SO_REUSEADDR,       "SO_REUSEADDR"},
//     {IP_MULTICAST_TTL,   "IP_MULTICAST_TTL"},
//     {IP_MULTICAST_LOOP,  "IP_MULTICAST_LOOP"},
//     {IP_ADD_MEMBERSHIP,  "IP_ADD_MEMBERSHIP"},
//     {IP_DROP_MEMBERSHIP, "IP_DROP_MEMBERSHIP"}
// };

Socket::Socket(int family, int type, int proto, int timeout) {
    // create a UDP socket
    if ((sock = socket(family, type, proto)) < 0) {
        throw SocketError("Socket() constructor failed");
    }
    this->family = family;
    this->type = type;
    this->proto = proto;
    this->timeout = timeout;
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
    if ((::gethostname(name,256)) < 0) {
        throw SocketError("gethostname() failed");
    }
    return std::string(name);
}

sockaddr_t Socket::getpeername(){
    sockaddr_t addr = {0};
    unsigned int addrlen = sizeof(addr);
    if((::getpeername(sock,(struct sockaddr*)&addr,&addrlen)) < 0){
        throw SocketError("getsockname() failed");
    }
    return addr;
}

sockaddr_t Socket::getsockname(){
    sockaddr_t addr = {0};
    unsigned int addrlen = sizeof(addr);
    if((::getsockname(sock,(struct sockaddr*)&addr,&addrlen)) < 0){
        throw SocketError("getsockname() failed");
    }
    return addr;
}

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

    // std::string ss(recv_str);
    // std::cout << "recvfrom " << ss << " " << ss.size() << std::endl;

    return msgaddr_t({recv_str}, from_addr);
    // return msgaddr_t{{recv_str}, from_addr};
}

void Socket::recvfrom(void* dst, int size, sockaddr_t& from_addr){
    memset(dst, 0, size); // clear buffer

    from_addr = {0};
    unsigned int from_len = sizeof(from_addr);
    int recv_len = 0;

    // block waiting to receive a packet
    recv_len = ::recvfrom(
        sock,
        dst, (size_t)size, 0,
        (struct sockaddr*)&from_addr, &from_len);
    if (recv_len != size) {
        throw SocketError("recvfrom() failed");
    }
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
    int err = ::setsockopt(sock, level, name, (char*) &val, sizeof(val));
    if (err < 0){
        throw SocketError("setsockopt() failed");
    }
}

#include <marko/udpsocket.hpp>

// #include <sys/types.h>  // for type definitions
// #include <sys/socket.h> // for socket API calls
// #include <netinet/in.h> // for address structs
// #include <stdio.h>      // for printf() and fprintf()
// #include <stdlib.h>     // for atoi()
// #include <string.h>     // for strlen()
// #include <unistd.h>     // for close()
// #include <string>
// #include <iostream>
// #include <map>
// #include <errno.h>
// #include <netdb.h>

// using namespace std;

// inline constexpr int MAX_LEN = 1024;

//
// do not bend, ha, ha, ha ...
// crease crumple cramp
// - Neuman
//

// string host_ip(){
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
//
// string print_addr(const struct sockaddr_in& addr){
//     string s(inet_ntoa(addr.sin_addr));
//     s += ":";
//     s += to_string(ntohs(addr.sin_port));
//     return s;
// }


// UDPSocket::UDPSocket(int timeout): sock(AF_INET, SOCK_DGRAM, IPPROTO_UDP) {}
//
// UDPSocket::~UDPSocket(){
//     this->sock.close();
// }
//
//
// cMsgAddr UDPSocket::recvfrom(){
//
//     // cMsgAddr ret = recvfrom();
//
//     // return std::move(MsgAddr(msg, from_addr));
//     return sock.recvfrom();
// }
//
// // MsgAddr UDPSocket::recv_nb(long msec){
// //     if(ready(msec))
// //         return this->recv();
// //
// //     MsgAddr r = {{""}, {0}};
// //     return r;
// // }
//
// bool UDPSocket::sendto(const std::string& msg, const struct sockaddr_in& addr){
//     sock.sendto(msg, addr);
//     return true;
// }

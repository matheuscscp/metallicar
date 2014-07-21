/*
 * UDPSocket.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_network.hpp"

// lib
#ifdef _WIN32
  #include <ws2tcpip.h>
#else
  #include <netinet/in.h>
  #include <unistd.h>
#endif

using namespace std;

namespace metallicar {

UDPSocket::UDPSocket(uint32_t maxlen) : maxlen(maxlen) {
#ifdef _WIN32
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  ULONG optval = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(ULONG));
  setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(ULONG));
  SOCKADDR_IN addr;
  addr.sin_family = AF_INET;
  addr.sin_port = 0;
  addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  bind(sd, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
#else
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  int optval = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));
  setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int));
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = 0;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(sd, (sockaddr*)&addr, sizeof(sockaddr_in));
#endif
}

UDPSocket::UDPSocket(const string& port, uint32_t maxlen) : maxlen(maxlen) {
  Address nport("", port);
#ifdef _WIN32
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  ULONG optval = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(ULONG));
  setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(ULONG));
  SOCKADDR_IN addr;
  addr.sin_family = AF_INET;
  addr.sin_port = nport.port;
  addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  bind(sd, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
#else
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  int optval = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));
  setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int));
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = nport.port;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(sd, (sockaddr*)&addr, sizeof(sockaddr_in));
#endif
}

UDPSocket::UDPSocket(const Address& multicastAddress, uint32_t maxlen) : maxlen(maxlen) {
#ifdef _WIN32
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  ULONG optval = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(ULONG));
  setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(ULONG));
  ip_mreq mreq;
  mreq.imr_multiaddr.S_un.S_addr = multicastAddress.ip;
  mreq.imr_interface.S_un.S_addr = Address::local().ip;
  setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(ip_mreq));
  SOCKADDR_IN addr;
  addr.sin_family = AF_INET;
  addr.sin_port = multicastAddress.port;
  addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  bind(sd, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
#else
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  int optval = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));
  setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int));
  ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = multicastAddress.ip;
  mreq.imr_interface.s_addr = Address::local().ip;
  setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(ip_mreq));
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = multicastAddress.port;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(sd, (sockaddr*)&addr, sizeof(sockaddr_in));
#endif
}

UDPSocket::~UDPSocket() {
#ifdef _WIN32
  closesocket(sd);
#else
  ::close(sd);
#endif
}

void UDPSocket::send(const Address& address, const ByteQueue& data) {
#ifdef _WIN32
  SOCKADDR_IN servaddr;
  memset((void*)&servaddr, 0, sizeof(SOCKADDR_IN));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = address.ip;
  servaddr.sin_port = address.port;
  sendto(sd, (const char*)data.ptr(), data.size(), 0, (SOCKADDR*)&servaddr, sizeof(SOCKADDR_IN));
#else
  sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = address.ip;
  servaddr.sin_port = address.port;
  sendto(sd, data.ptr(), data.size(), 0, (const sockaddr*)&servaddr, sizeof(sockaddr_in));
#endif
}

ByteQueue UDPSocket::recv(Address& address) {
  ByteQueue data;
#ifdef _WIN32
  fd_set fds;
  timeval timeout;
  FD_ZERO(&fds);
  FD_SET(sd, &fds);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  if (select(0, &fds, nullptr, nullptr, &timeout) > 0) {
    SOCKADDR_IN addr;
    int addrsize = sizeof(SOCKADDR_IN);
    data.resize(maxlen);
    data.resize(uint32_t(recvfrom(SOCKET(sd), (char*)data.ptr(), maxlen, 0, (SOCKADDR*)&addr, &addrsize)));
    address = Address(addr.sin_addr.S_un.S_addr, addr.sin_port);
  }
#else
  fd_set fds;
  timeval timeout;
  FD_ZERO(&fds);
  FD_SET(sd, &fds);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  if (select(sd + 1, &fds, nullptr, nullptr, &timeout) > 0) {
    sockaddr_in addr;
    socklen_t addrsize = sizeof(sockaddr_in);
    data.resize(maxlen);
    data.resize(uint32_t(recvfrom(sd, data.ptr(), maxlen, 0, (sockaddr*)&addr, &addrsize)));
    address = Address(addr.sin_addr.s_addr, addr.sin_port);
  }
#endif
  return data;
}

} // namespace metallicar

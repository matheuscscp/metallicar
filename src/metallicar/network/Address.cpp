/*
 * Address.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_network.hpp"

// standard
#include <cstdio>
#include <cstdlib>

using namespace std;

namespace metallicar {

Address::Address(uint32_t ip, uint16_t port) : ip(ip), port(port) {
  
}

Address::Address(const string& ip, const string& port) : ip(0), port(0) {
  if (ip.size()) {
    IPaddress tmp;
    SDLNet_ResolveHost(&tmp, ip.c_str(), 0);
    this->ip = tmp.host;
  }
  if (port.size()) {
    sscanf(port.c_str(), "%hu", &this->port);
    this->port = htons(this->port);
  }
}

Address Address::local() {
  IPaddress addrs[100];
  int total = SDLNet_GetLocalAddresses(addrs, 100);
  if (!total) {
    fprintf(stderr, "No network interface was found\n");
    exit(0);
  }
  
  // look for a private network address
  for (int i = 0; i < total; i++) {
    IPaddress addr;
    SDLNet_ResolveHost(&addr, SDLNet_ResolveIP(&addrs[i]), 0);
    if (isPrivateNetwork(addr.host))
      return Address(addr.host, 0);
  }
  
  fprintf(stderr, "No private network address was found\n");
  exit(0);
}

string Address::toString() const {
  char tmp[30];
  if (port != 0) {
    sprintf(tmp, "%d.%d.%d.%d:%d",
      int(((uint8_t*)&ip)[0]),
      int(((uint8_t*)&ip)[1]),
      int(((uint8_t*)&ip)[2]),
      int(((uint8_t*)&ip)[3]),
      ntohs(port)
    );
  }
  else {
    sprintf(tmp, "%d.%d.%d.%d",
      int(((uint8_t*)&ip)[0]),
      int(((uint8_t*)&ip)[1]),
      int(((uint8_t*)&ip)[2]),
      int(((uint8_t*)&ip)[3])
    );
  }
  return string(tmp);
}

uint32_t Address::ntohl(uint32_t ip) {
  return SDLNet_Read32(&ip);
}

uint32_t Address::htonl(uint32_t ip) {
  return SDLNet_Read32(&ip);
}

uint16_t Address::ntohs(uint16_t port) {
  return SDLNet_Read16(&port);
}

uint16_t Address::htons(uint16_t port) {
  return SDLNet_Read16(&port);
}

bool Address::isPrivateNetwork() {
  if ((ip & 0x0000FFFF) == 0x0000A8C0) // 16-bit block
    return true;
  if ((ip & 0x0000F0FF) == 0x000010AC) // 20-bit block
    return true;
  if ((ip & 0x000000FF) == 0x0000000A) // 24-bit block
    return true;
  return false;
}

bool Address::isPrivateNetwork(uint32_t ip) {
  if ((ip & 0x0000FFFF) == 0x0000A8C0) // 16-bit block
    return true;
  if ((ip & 0x0000F0FF) == 0x000010AC) // 20-bit block
    return true;
  if ((ip & 0x000000FF) == 0x0000000A) // 24-bit block
    return true;
  return false;
}

} // namespace metallicar

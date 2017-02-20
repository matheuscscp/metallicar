/*
 * TCPConnection.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_network.hpp"

using namespace std;

namespace metallicar {

TCPConnection::TCPConnection(const Address& addr) {
  if (addr.port) {
    IPaddress tmp;
    tmp.host = addr.ip;
    tmp.port = addr.port;
    sd = SDLNet_TCP_Open(&tmp);
  }
}

TCPConnection& TCPConnection::send(const ByteQueue& data) {
  if (sd)
    SDLNet_TCP_Send(sd, data.ptr(), data.size());
  return *this;
}

TCPConnection& TCPConnection::send(const void* data, uint32_t maxlen) {
  if (sd)
    SDLNet_TCP_Send(sd, data, maxlen);
  return *this;
}

TCPConnection& TCPConnection::send(const string& data, bool withoutNullTermination) {
  if (sd) {
    SDLNet_TCP_Send(sd, (const void*)data.c_str(), data.size());
    if (!withoutNullTermination) {
      char c = 0;
      SDLNet_TCP_Send(sd, (const void*)&c, sizeof(char));
    }
  }
  return *this;
}

void TCPConnection::recv(ByteQueue& data) {
  uint32_t total = 0;
  if (sd)
    total = SDLNet_TCP_Recv(sd, data.ptr(), data.size());
  data.resize(total);
}

uint32_t TCPConnection::recv(void* data, uint32_t maxlen) {
  uint32_t total = 0;
  if (sd)
    total = SDLNet_TCP_Recv(sd, data, maxlen);
  return total;
}

string TCPConnection::recv(uint32_t maxlen) {
  string data;
  if (sd) {
    data.resize(maxlen, '0');
    data.resize(SDLNet_TCP_Recv(sd, (void*)data.c_str(), maxlen));
  }
  return data;
}

} // namespace metallicar

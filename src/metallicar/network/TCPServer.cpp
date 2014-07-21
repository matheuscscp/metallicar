/*
 * TCPServer.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_network.hpp"

using namespace std;

namespace metallicar {

TCPServer::TCPServer(const string& port) {
  if (port.size()) {
    IPaddress addr;
    SDLNet_ResolveHost(&addr, nullptr, Address::ntohs(Address("", port).port));
    sd = SDLNet_TCP_Open(&addr);
  }
}

TCPConnection* TCPServer::accept() {
  TCPConnection* conn = nullptr;
  if (sd) {
    TCPsocket newsd = SDLNet_TCP_Accept(sd);
    if (newsd) {
      conn = new TCPConnection(Address(0, 0));
      ((TCPServer*)conn)->sd = newsd;
    }
  }
  return conn;
}

} // namespace metallicar

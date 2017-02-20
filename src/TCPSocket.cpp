/*
 * TCPSocket.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_network.hpp"

namespace metallicar {

TCPSocket::TCPSocket() : sd(0) {
  
}

TCPSocket::~TCPSocket() {
  if (sd)
    SDLNet_TCP_Close(sd);
}

} // namespace metallicar

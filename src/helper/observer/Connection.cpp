/*
 * Connection.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "observer.hpp"

namespace observer {

Connection::Connection() : token(new uint8_t) {
  
}

bool Connection::connected() {
  return !token.unique();
}

void Connection::disconnect() {
  token = nullptr;
}

} // namespace observer

/*
 * Event.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "observer.hpp"

namespace observer {

Event::Event(uint32_t type) : type(type) {
  
}

uint32_t Event::getType() const {
  return type;
}

} // namespace observer

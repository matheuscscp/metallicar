/*
 * Time.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_time.hpp"

// lib
#include "SDL.h"

namespace metallicar {

uint32_t Time::get() {
  return SDL_GetTicks();
}

} // namespace metallicar

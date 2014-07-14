/*
 * Time.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "Time.hpp"

// lib
#include "SDL.h"

using namespace std;

namespace metallicar {
namespace engine {

uint32_t Time::get() {
  return SDL_GetTicks();
}

} // engine metallicar
} // namespace metallicar

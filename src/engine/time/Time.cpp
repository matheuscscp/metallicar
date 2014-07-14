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

using namespace std;

namespace metallicar {
namespace engine {

uint32_t Time::get() {
  return SDL_GetTicks();
}

} // namespace engine
} // namespace metallicar

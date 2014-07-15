/*
 * Input.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// lib
#include "SDL.h"

using namespace std;

namespace metallicar {
namespace engine {

bool quitRequested = false;

void Input::update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        engine::quitRequested = true;
        break;
        
      default:
        break;
    }
  }
}

bool Input::quitRequested() {
  return engine::quitRequested;
}

void Input::resetQuitRequest() {
  engine::quitRequested = false;
}

} // namespace engine
} // namespace metallicar

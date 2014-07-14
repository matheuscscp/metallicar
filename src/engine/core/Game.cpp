/*
 * Game.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "Game.hpp"

// standard
#include <cstdlib>

// lib
#include "SDL.h"

// local
#include "Util.hpp"

namespace metallicar {
namespace engine {

Game::Game() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
    util::Logger::log(util::Logger::ERROR, SDL_GetError());
    exit(0);
  }
}

Game::~Game() {
  SDL_Quit();
}

void Game::run() {
  
}

} // engine metallicar
} // namespace metallicar

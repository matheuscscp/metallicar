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
#include "SDL_image.h"

// local
#include "Util.hpp"

using namespace std;

namespace metallicar {
namespace engine {

static ScreenOptions screenOptions;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static bool started = false;

ScreenOptions::ScreenOptions() :
title("metallicar game"), width(800), height(600), fullscreen(false), icon("asset/icon.png")
{
  
}

ScreenOptions::ScreenOptions(const string& title, int width, int height, bool fullscreen, const string& icon) :
title(title), width(width), height(height), fullscreen(fullscreen), icon(icon)
{
  
}

void Game::init(const ScreenOptions& screenOptions) {
  if (started) {
    return;
  }
  
  engine::screenOptions = screenOptions;
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
    util::Logger::log(util::Logger::ERROR, SDL_GetError());
    exit(0);
  }
  if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)) {
    util::Logger::log(util::Logger::ERROR, IMG_GetError());
    exit(0);
  }
  if ((window = SDL_CreateWindow(
    screenOptions.title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    screenOptions.width,
    screenOptions.height,
    SDL_WINDOW_OPENGL | (screenOptions.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
  )) == nullptr) {
    util::Logger::log(util::Logger::ERROR, SDL_GetError());
    exit(0);
  }
  if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr) {
    util::Logger::log(util::Logger::ERROR, SDL_GetError());
    exit(0);
  }
  if (screenOptions.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(util::Path::get(screenOptions.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
}

void Game::close() {
  if (!window) {
    return;
  }
  
  SDL_DestroyRenderer(renderer);
  renderer = nullptr;
  SDL_DestroyWindow(window);
  window = nullptr;
  
  IMG_Quit();
  SDL_Quit();
}

void Game::start(GameState* firstState) {
  if (started) {
    return;
  }
  started = true;
  while (started) {
    SDL_RenderPresent(renderer);
    if (SDL_QuitRequested())
      stop();
  }
}

void Game::stop() {
  started = false;
}

void Game::setScreenOptions(const ScreenOptions& screenOptions) {
  //TODO
}

} // engine metallicar
} // namespace metallicar

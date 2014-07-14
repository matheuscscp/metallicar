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
#include <cstdio>
#include <list>

// lib
#include "SDL_image.h"

// local
#include "Util.hpp"

using namespace std;

namespace metallicar {
namespace engine {

// =============================================================================
// private types
// =============================================================================

enum ChangeOption {
  NA, CHANGE, PUSH, POP, QUIT
};

// =============================================================================
// private function declarations
// =============================================================================

static void close();
static void input();

// =============================================================================
// private globals
// =============================================================================

static ScreenOptions screenOptions;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static bool quitRequested = false;
static list<GameScene*> scenes;
static GameScene* scene_change = nullptr;
static ChangeOption change_option = ChangeOption::NA;
static void* pop_args = nullptr;

// =============================================================================
// public methods
// =============================================================================

ScreenOptions::ScreenOptions() :
title("metallicar game"), width(800), height(600), fullscreen(false), icon("asset/icon.png")
{
  
}

ScreenOptions::ScreenOptions(const string& title, int width, int height, bool fullscreen, const string& icon) :
title(title), width(width), height(height), fullscreen(fullscreen), icon(icon)
{
  
}

void Game::init(const ScreenOptions& screenOptions) {
  if (window) {
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
  SDL_GetWindowSize(window, &engine::screenOptions.width, &engine::screenOptions.height);
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

void Game::run(GameScene* firstScene) {
  if (scenes.size()) {
    return;
  }
  
  while (scenes.size()) {
    input();
    
    //update
    //render
    
    SDL_RenderPresent(renderer);
  }
  
  close();
}

void Game::changeScene(GameScene* scene) {
  scene_change = scene;
  change_option = ChangeOption::CHANGE;
}

void Game::pushScene(GameScene* scene) {
  scene_change = scene;
  change_option = ChangeOption::PUSH;
}

void Game::popScene(void* args) {
  pop_args = args;
  change_option = ChangeOption::POP;
}

void Game::quit() {
  change_option = ChangeOption::QUIT;
}

void Game::setScreenOptions(const ScreenOptions& screenOptions) {
  engine::screenOptions = screenOptions;
  
  SDL_SetWindowTitle(window, screenOptions.title.c_str());
  SDL_SetWindowFullscreen(window, screenOptions.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
  SDL_SetWindowSize(window, screenOptions.width, screenOptions.height);
  SDL_GetWindowSize(window, &engine::screenOptions.width, &engine::screenOptions.height);
  if (screenOptions.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(util::Path::get(screenOptions.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
}

bool Game::quitRequested() {
  return engine::quitRequested;
}

void Game::resetQuitRequest() {
  engine::quitRequested = false;
}

// =============================================================================
// private functions
// =============================================================================

static void close() {
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

static void input() {
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

} // engine metallicar
} // namespace metallicar

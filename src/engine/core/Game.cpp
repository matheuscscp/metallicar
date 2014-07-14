/*
 * Game.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// standard
#include <cstdlib>
#include <list>

// lib
#include "SDL_image.h"

// local
#include "metallicar_time.hpp"
#include "Log.hpp"
#include "Path.hpp"

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
static void updateStack();

// =============================================================================
// private globals
// =============================================================================

static WindowOptions windowOptions;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static bool quitRequested = false;
static list<GameScene*> scenes;
static GameScene* newScene = nullptr;
static void* popArgs = nullptr;
static ChangeOption changeOption = ChangeOption::NA;

// =============================================================================
// public methods
// =============================================================================

void Game::init(const WindowOptions& windowOptions) {
  if (window) {
    return;
  }
  
  engine::windowOptions = windowOptions;
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
    util::Log::message(util::Log::Error, SDL_GetError());
    exit(0);
  }
  if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)) {
    util::Log::message(util::Log::Error, IMG_GetError());
    exit(0);
  }
  if ((window = SDL_CreateWindow(
    windowOptions.title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    windowOptions.width,
    windowOptions.height,
    SDL_WINDOW_OPENGL | (windowOptions.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
  )) == nullptr) {
    util::Log::message(util::Log::Error, SDL_GetError());
    exit(0);
  }
  SDL_GetWindowSize(window, &engine::windowOptions.width, &engine::windowOptions.height);
  if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr) {
    util::Log::message(util::Log::Error, SDL_GetError());
    exit(0);
  }
  if (windowOptions.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(util::Path::get(windowOptions.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  DeltaTime::init();
}

void Game::run(GameScene* firstScene) {
  if (scenes.size()) {
    return;
  }
  
  scenes.push_back(firstScene);
  
  while (scenes.size()) {
    DeltaTime::update();
    
    input();
    
    // update scenes
    while (DeltaTime::periodReached()) {
      for (auto scene : scenes) {
        if (!scene->frozen) {
          scene->update();
        }
      }
    }
    DeltaTime::accumulate();
    
    // render scenes
    for (auto scene : scenes) {
      if (!scene->frozen || (scene->frozen && scene->visible)) {
        scene->render();
      }
    }
    
    SDL_RenderPresent(renderer);
    
    updateStack();
  }
  
  close();
}

void Game::changeScene(GameScene* scene) {
  newScene = scene;
  changeOption = ChangeOption::CHANGE;
}

void Game::pushScene(GameScene* scene) {
  newScene = scene;
  changeOption = ChangeOption::PUSH;
}

void Game::popScene(void* args) {
  popArgs = args;
  changeOption = ChangeOption::POP;
}

void Game::quit() {
  changeOption = ChangeOption::QUIT;
}

void Game::setWindowOptions(const WindowOptions& windowOptions) {
  engine::windowOptions = windowOptions;
  
  SDL_SetWindowTitle(window, windowOptions.title.c_str());
  SDL_SetWindowFullscreen(window, windowOptions.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
  SDL_SetWindowSize(window, windowOptions.width, windowOptions.height);
  SDL_GetWindowSize(window, &engine::windowOptions.width, &engine::windowOptions.height);
  if (windowOptions.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(util::Path::get(windowOptions.icon).c_str());
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

static void input() {//TODO
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

static void updateStack() {
  switch (changeOption) {
    case ChangeOption::NA:
      break;
      
    case ChangeOption::CHANGE:
      delete scenes.back();
      scenes.pop_back();
      scenes.push_back(newScene);
      break;
      
    case ChangeOption::PUSH:
      scenes.push_back(newScene);
      break;
      
    case ChangeOption::POP:
      delete scenes.back();
      scenes.pop_back();
      if (scenes.size())
        scenes.back()->wakeup(popArgs);
      break;
      
    case ChangeOption::QUIT:
      while (scenes.size()) {
        delete scenes.back();
        scenes.pop_back();
      }
      break;
      
    default:
      break;
  }
  newScene = nullptr;
  popArgs = nullptr;
  changeOption = ChangeOption::NA;
}

} // namespace engine
} // namespace metallicar

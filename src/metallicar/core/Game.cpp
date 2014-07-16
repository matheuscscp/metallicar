/*
 * Game.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// standard
#include <list>
#include <cstdio>

// lib
#include "SDL_image.h"

// local
#include "metallicar_time.hpp"
#include "Log.hpp"
#include "Path.hpp"

using namespace std;

namespace metallicar {

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
static void updateStack();

static void initDT();
static void updateDT();
static bool reachedDT();
static void accumulateDT();

// =============================================================================
// private globals
// =============================================================================

static WindowOptions windowOptions;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static list<GameScene*> scenes;
static GameScene* newScene = nullptr;
static GameArgs* popArgs = nullptr;
static ChangeOption changeOption = ChangeOption::NA;

static uint32_t last = 0;        // unit: milliseconds
static uint32_t ups = 32;        // unit: updates/second
static float dtFloat = 0.03125f; // unit: seconds
static uint32_t dtFixed = 31;    // unit: milliseconds
static uint32_t dt = 0;          // unit: milliseconds
static uint32_t updateID = 0;

// =============================================================================
// public methods
// =============================================================================

void Game::init(const WindowOptions& windowOptions) {
  if (window) {
    return;
  }
  
  metallicar::windowOptions = windowOptions;
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)) {
    Log::message(Log::Error, IMG_GetError());
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
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  SDL_GetWindowSize(window, &metallicar::windowOptions.width, &metallicar::windowOptions.height);
  if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  if (windowOptions.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(Path::get(windowOptions.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  SDL_ShowCursor(windowOptions.cursor ? 1 : 0);
  initDT();
}

void Game::run(GameScene* firstScene) {
  if (scenes.size()) {
    return;
  }
  
  scenes.push_back(firstScene);
  
  while (scenes.size()) {
    updateDT();
    
    Input::update();
    
    // update scenes
    while (reachedDT()) {
      for (auto scene : scenes) {
        if (!scene->frozen) {
          scene->update();
        }
      }
    }
    accumulateDT();
    
    // render scenes
    for (auto scene : scenes) {
      if (!scene->frozen || (scene->frozen && scene->visible)) {
        scene->render();
      }
    }
    
    // update window
    SDL_RenderPresent(renderer);
    
    updateStack();
  }
  
  close();
}

void Game::changeScene(GameScene* scene) {
  if (newScene) {
    delete newScene;
  }
  newScene = scene;
  changeOption = ChangeOption::CHANGE;
}

void Game::pushScene(GameScene* scene) {
  if (newScene) {
    delete newScene;
  }
  newScene = scene;
  changeOption = ChangeOption::PUSH;
}

void Game::popScene(GameArgs* args) {
  popArgs = args;
  changeOption = ChangeOption::POP;
}

void Game::quit() {
  changeOption = ChangeOption::QUIT;
}

WindowOptions Game::getWindowOptions() {
  return windowOptions;
}

void Game::setWindowOptions(WindowOptions& windowOptions) {
  SDL_SetWindowTitle(window, windowOptions.title.c_str());
  SDL_SetWindowFullscreen(window, 0);
  SDL_SetWindowSize(window, windowOptions.width, windowOptions.height);
  SDL_SetWindowFullscreen(window, windowOptions.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
  SDL_GetWindowSize(window, &windowOptions.width, &windowOptions.height);
  if (windowOptions.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(Path::get(windowOptions.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  SDL_ShowCursor(windowOptions.cursor ? 1 : 0);
  
  metallicar::windowOptions = windowOptions;
}

uint32_t Game::getUPS() {
  return ups;
}

void Game::setUPS(uint32_t ups) {
  metallicar::ups = ups;
  dtFloat = 1/(float)ups;
  dtFixed = 1000/ups;
}

float Game::getDT() {
  return dtFloat;
}

uint32_t Game::updateID() {
  return metallicar::updateID;
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
        scenes.back()->wakeup(*popArgs);
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

static void initDT() {
  last = Time::get() - dtFixed;
}

static void updateDT() {
  uint32_t now = Time::get();
  dt = now - last;
  last = now;
  metallicar::updateID = now - 1;
}

static bool reachedDT() {
  if (dt >= dtFixed) {
    dt -= dtFixed;
    metallicar::updateID++;
    return true;
  }
  return false;
}

static void accumulateDT() {
  last -= dt;
}

} // namespace metallicar

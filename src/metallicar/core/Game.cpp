/*
 * Game.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// standard
#include <cstdio>

// lib
#include "SDL_image.h"
#include "SDL_net.h"

// local
#include "metallicar_time.hpp"
#include "Log.hpp"

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
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;
  
  // SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  
  // IMG
  {
    int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if (IMG_Init(flags) != flags) {
      Log::message(Log::Error, IMG_GetError());
      exit(0);
    }
  }
  
  // SDLNet
  if (SDLNet_Init()) {
    Log::message(Log::Error, SDLNet_GetError());
    exit(0);
  }
  
  Window::init(windowOptions);
  
  SDL_StartTextInput();
  
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
    
    Window::update();
    
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
  if (popArgs) {
    delete popArgs;
  }
  popArgs = args;
  changeOption = ChangeOption::POP;
}

void Game::quit() {
  changeOption = ChangeOption::QUIT;
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
  SDL_StopTextInput();
  
  Window::close();
  
  SDLNet_Quit();
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
  if (newScene) {
    delete newScene;
  }
  newScene = nullptr;
  if (popArgs) {
    delete popArgs;
  }
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

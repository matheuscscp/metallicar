/*
 * Game.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// lib
#include "SDL_image.h"
#include "SDL_net.h"

// local
#include "metallicar_time.hpp"
#include "metallicar_io.hpp"
#include "metallicar_graphics.hpp"
#include "Log.hpp"

using namespace std;

namespace metallicar {

// =============================================================================
// private function declarations
// =============================================================================

static void initDT();
static void updateDT();
static bool reachedDT();
static void accumulateDT();

// =============================================================================
// private globals
// =============================================================================

static bool initialized;
static bool quit;

static uint32_t last;     // unit: milliseconds
static uint32_t ups;      // unit: updates/second
static float dtFloat;     // unit: seconds
static uint32_t dtFixed;  // unit: milliseconds
static uint32_t dt;       // unit: milliseconds
static uint32_t updateID;

static void initGlobals() {
  metallicar::initialized = false;
  metallicar::quit = false;
  
  metallicar::last = 0;           // unit: milliseconds
  metallicar::ups = 32;           // unit: updates/second
  metallicar::dtFloat = 0.03125f; // unit: seconds
  metallicar::dtFixed = 31;       // unit: milliseconds
  metallicar::dt = 0;             // unit: milliseconds
  metallicar::updateID = 0;
}

// =============================================================================
// public methods
// =============================================================================

void Game::init() {
  if (initialized) {
    return;
  }
  initGlobals();
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
  
  initDT();
}

void Game::close() {
  if (!initialized) {
    return;
  }
  initialized = false;
  
  GameScene::close();
  
  SDLNet_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::run() {
  if (!initialized) {
    return;
  }
  
  GameScene::change();
  
  while (!metallicar::quit && GameScene::loaded()) {
    GameScene& currentScene = GameScene::runningInstance();
    
    // update
    updateDT();
    while (reachedDT()) {
      GameRenderers::clear();
      Input::update();
      currentScene.update();
      currentScene.render();
    }
    accumulateDT();
    
    // render
    Graphics::prepareFrame();
    GameRenderers::render();
    Graphics::finalizeFrame();
    Window::update();
    
    GameScene::change();
  }
}

void Game::quit() {
  metallicar::quit = true;
}

uint32_t Game::getUPS() {
  return ups;
}

void Game::setUPS(uint32_t ups) {
  if (!ups) {
    return;
  }
  
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

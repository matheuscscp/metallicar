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
// private function declarations
// =============================================================================

static void close();

static void initDT();
static void updateDT();
static bool reachedDT();
static void accumulateDT();

// =============================================================================
// private globals
// =============================================================================

static bool quit = false;

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
  
  initDT();
}

void Game::run() {
  GameScene::change();
  
  while (!metallicar::quit) {
    GameScene& currentScene = GameScene::instance();
    
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
    GameRenderers::render();
    Window::update();
    
    GameScene::change();
  }
  
  close();
}

void Game::quit() {
  metallicar::quit = true;
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
  GameScene::close();
  
  Window::close();
  
  SDLNet_Quit();
  IMG_Quit();
  SDL_Quit();
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

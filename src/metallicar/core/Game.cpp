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

static void updateFPS();

// =============================================================================
// private globals
// =============================================================================

static map<double, list<function<void()>>> renderers;

static Game* instance;
static Game* newInstance;

static bool initialized;
static bool quit;

static float dt;                // unit: seconds
static uint32_t updateInterval; // unit: milliseconds
static uint32_t lastUpdate;     // unit: milliseconds
static uint32_t timeElapsed;    // unit: milliseconds

static float fps;               // unit: hertz
static uint32_t lastFrame;      // unit: milliseconds

static void initGlobals() {
  metallicar::renderers.clear();
  
  metallicar::instance = nullptr;
  metallicar::newInstance = nullptr;
  
  metallicar::initialized = false;
  metallicar::quit = false;
  
  metallicar::dt = 31/1000.0f;      // unit: seconds
  metallicar::updateInterval = 31;  // unit: milliseconds
  metallicar::lastUpdate = 0;       // unit: milliseconds
  metallicar::timeElapsed = 0;      // unit: milliseconds
  
  metallicar::fps = 60.0f;           // unit: hertz
  metallicar::lastFrame = 0;        // unit: milliseconds
}

// =============================================================================
// public methods
// =============================================================================

Game::Game() {
  delete newInstance;
  newInstance = this;
  
  quitEventConnection = Input::connect<Input::QuitEvent>(
    [](const observer::EventBase&) { Game::quit(); }
  );
}

Game::~Game() {
  
}

void Game::update() {
  
}

void Game::render() {
  
}

Game& Game::runningInstance() {
  return *instance;
}

void Game::changeInstance() {
  if (newInstance) {
    delete instance;
    instance = newInstance;
    newInstance = nullptr;
  }
}

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
  
  // cleaning instance
  delete newInstance;
  newInstance = nullptr;
  delete instance;
  instance = nullptr;
  
  // libs
  SDLNet_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::run() {
  if (!initialized) {
    return;
  }
  
  changeInstance();
  
  while (!metallicar::quit && instance) {
    updateFPS();
    
    // update
    updateDT();
    while (reachedDT()) {
      renderers.clear();
      Input::update();
      instance->update();
      instance->render();
    }
    accumulateDT();
    
    // render
    Graphics::prepareFrame();
    for (auto& kv : renderers) {
      for (auto& renderer : kv.second) {
        renderer();
      }
    }
    Graphics::finalizeFrame();
    Window::update();
    
    changeInstance();
  }
}

void Game::addRenderer(double z, const function<void()>& renderer) {
  renderers[z].push_back(renderer);
}

void Game::quit() {
  metallicar::quit = true;
}

float Game::dt() {
  return metallicar::dt;
}

void Game::setUpdateInterval(uint32_t updateInterval) {
  if (!updateInterval) {
    return;
  }
  metallicar::updateInterval = updateInterval;
  metallicar::dt = updateInterval/1000.0f;
}

float Game::fps() {
  return metallicar::fps;
}

// =============================================================================
// private functions
// =============================================================================

static void initDT() {
  lastUpdate = Time::get() - updateInterval;
}

static void updateDT() {
  uint32_t now = Time::get();
  timeElapsed = now - lastUpdate;
  lastUpdate = now;
}

static bool reachedDT() {
  if (timeElapsed >= updateInterval) {
    timeElapsed -= updateInterval;
    return true;
  }
  return false;
}

static void accumulateDT() {
  lastUpdate -= timeElapsed;
}

static void updateFPS() {
  uint32_t now = Time::get();
  fps = fps*0.95f + 0.05f*(1000.0f/(now - lastFrame));
  lastFrame = now;
}

} // namespace metallicar

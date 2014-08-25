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
#include "SDL_ttf.h"

// local
#include "metallicar_time.hpp"
#include "metallicar_io.hpp"
#include "metallicar_graphics.hpp"
#include "metallicar_asset.hpp"
#include "metallicar_concurrency.hpp"
#include "Log.hpp"

using namespace std;

namespace metallicar {

// =============================================================================
// private function declarations
// =============================================================================

static void updateTimeElapsed();
static bool timeElapsedGreaterThanStep();

static void updateFPS();

// =============================================================================
// private globals
// =============================================================================

// rendering globals
static map<double, list<function<void()>>> renderingBuffer1, renderingBuffer2;
static map<double, list<function<void()>>>* renderingFrontBuffer;
static map<double, list<function<void()>>>* renderingBackBuffer;
static Mutex renderingBuffersMutex;

static Game* instance;
static Game* newInstance;

static bool initialized;
static bool quit;

static float dt;              // unit: seconds
static uint32_t updateStep;   // unit: milliseconds
static uint32_t lastUpdate;   // unit: milliseconds
static uint32_t timeElapsed;  // unit: milliseconds

static float fps;             // unit: hertz
static uint32_t lastFrame;    // unit: milliseconds

static void initGlobals() {
  metallicar::renderingBuffer1.clear();
  metallicar::renderingBuffer2.clear();
  metallicar::renderingFrontBuffer = &renderingBuffer1;
  metallicar::renderingBackBuffer = &renderingBuffer2;
  
  metallicar::instance = nullptr;
  metallicar::newInstance = nullptr;
  
  metallicar::initialized = false;
  metallicar::quit = false;
  
  metallicar::dt = 31/1000.0f;  // unit: seconds
  metallicar::updateStep = 31;  // unit: milliseconds
  metallicar::lastUpdate = 0;   // unit: milliseconds
  metallicar::timeElapsed = 0;  // unit: milliseconds
  
  metallicar::fps = 60.0f;      // unit: hertz
  metallicar::lastFrame = 0;    // unit: milliseconds
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
  
  // TTF
  if (TTF_Init()) {
    Log::message(Log::Error, TTF_GetError());
    exit(0);
  }
  
  // SDLNet
  if (SDLNet_Init()) {
    Log::message(Log::Error, SDLNet_GetError());
    exit(0);
  }
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
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::run() {
  if (!initialized) {
    return;
  }
  
  instance = newInstance;
  newInstance = nullptr;
  Assets::clear();
  
  Thread updateThread([]() {
    lastUpdate = Time::get();
    
    while (!metallicar::quit && instance) {
      bool updated = false;
      
      updateTimeElapsed();
      while (timeElapsedGreaterThanStep()) {
        updated = true;
        
        renderingBackBuffer->clear();
        Input::pollEvents();
        instance->update();
        instance->render();
        
        if (newInstance) { // change instance
          delete instance;
          instance = newInstance;
          newInstance = nullptr;
          Assets::clear();
        }
      }
      
      if (updated) { // swap rendering buffers
        renderingBuffersMutex.lock();
        map<double, list<function<void()>>>* tmp = renderingFrontBuffer;
        renderingFrontBuffer = renderingBackBuffer;
        renderingBackBuffer = tmp;
        renderingBuffersMutex.unlock();
      }
      
      Thread::sleep(1);
    }
  });
  updateThread.start();
  
  // main thread (I/O)
  while (!metallicar::quit && instance) {
    updateFPS();
    
    Input::pollWindowEvents();
    
    // copy front buffer
    renderingBuffersMutex.lock();
    map<double, list<function<void()>>> renderers(*renderingFrontBuffer);
    renderingBuffersMutex.unlock();
    
    // render
    Graphics::prepareFrame();
    for (auto& kv : renderers) {
      for (auto& renderer : kv.second) {
        renderer();
      }
    }
    Graphics::finalizeFrame();
    
    Window::update();
  }
  
  updateThread.join();
}

void Game::addRenderer(double z, const function<void()>& renderer) {
  (*renderingBackBuffer)[z].push_back(renderer);
}

void Game::quit() {
  metallicar::quit = true;
}

float Game::dt() {
  return metallicar::dt;
}

void Game::setUpdateStep(uint32_t step) {
  if (!step) {
    return;
  }
  updateStep = step;
  metallicar::dt = step/1000.0f;
}

float Game::fps() {
  return metallicar::fps;
}

// =============================================================================
// private functions
// =============================================================================

static void updateTimeElapsed() {
  uint32_t now = Time::get();
  timeElapsed += now - lastUpdate;
  if (timeElapsed > 250) {
    timeElapsed = 250;
  }
  lastUpdate = now;
}

static bool timeElapsedGreaterThanStep() {
  if (timeElapsed >= updateStep) {
    timeElapsed -= updateStep;
    return true;
  }
  return false;
}

static void updateFPS() {
  uint32_t now = Time::get();
  float newMeasure = 1000.0f/(now - lastFrame);
  if (newMeasure > 500.0f) {
    newMeasure = 0.0f;
  }
  fps = fps*0.95f + 0.05f*newMeasure;
  lastFrame = now;
}

} // namespace metallicar

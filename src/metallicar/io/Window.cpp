/*
 * Window.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_io.hpp"

// standard
#include <cstdlib>

// lib
#include "SDL_image.h"
#include "SDL_opengl.h"

// local
#include "Log.hpp"
#include "Path.hpp"

namespace metallicar {

// =============================================================================
// private function declarations
// =============================================================================

static void initLegacyOpenGL();

// =============================================================================
// private globals
// =============================================================================

static WindowOptions options;
static SDL_Window* window = nullptr;

// =============================================================================
// public methods
// =============================================================================

void Window::init(const WindowOptions& options) {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;
  
  metallicar::options = options;
  
  // window
  if ((window = SDL_CreateWindow(
    options.title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    options.width,
    options.height,
    SDL_WINDOW_OPENGL |(options.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
  )) == nullptr) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  SDL_GetWindowSize(
    window, &metallicar::options.width, &metallicar::options.height
  );
  if (options.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(Path::get(options.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  SDL_ShowCursor(options.cursor ? 1 : 0);
  
  // context
  //TODO
  
  initLegacyOpenGL();
}

void Window::close() {
  
}

WindowOptions Window::getOptions() {
  return metallicar::options;
}

void Window::setOptions(const WindowOptions& options) {
  metallicar::options = options;
  
  SDL_SetWindowTitle(window, options.title.c_str());
  SDL_SetWindowFullscreen(window, 0);
  SDL_SetWindowSize(window, options.width, options.height);
  SDL_SetWindowFullscreen(
    window, options.fullscreen ? SDL_WINDOW_FULLSCREEN : 0
  );
  SDL_GetWindowSize(
    window, &metallicar::options.width, &metallicar::options.height
  );
  if (options.icon.size()) {
    SDL_Surface* iconsurface = IMG_Load(Path::get(options.icon).c_str());
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  SDL_ShowCursor(options.cursor ? 1 : 0);
}

void Window::update() {
  
}

// =============================================================================
// private functions
// =============================================================================

static void initLegacyOpenGL() {
  // enable texture
  glEnable(GL_TEXTURE_2D);
  
  // enable blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // backbuffer size
  glViewport(0, 0, options.width, options.height);
  
  // setup projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, options.width, options.height, 0, -1, 1);
  
  // clears matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

} // namespace metallicar

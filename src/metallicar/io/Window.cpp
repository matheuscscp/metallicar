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
static void updateProjection();

// =============================================================================
// private globals
// =============================================================================

static WindowOptions options;
static SDL_Window* window = nullptr;
static SDL_GLContext glContext = nullptr;

// =============================================================================
// public methods
// =============================================================================

void Window::init(const WindowOptions& options) {
  if (window) {
    return;
  }
  
  metallicar::options = options;
  
  // window
  if ((window = SDL_CreateWindow(
    options.title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    options.width,
    options.height,
    SDL_WINDOW_OPENGL |
    SDL_WINDOW_SHOWN |
    (options.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
  )) == nullptr) {
    Log::message(Log::Error, "Window could not be created");
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
  if ((glContext = SDL_GL_CreateContext(window)) == nullptr) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  if (SDL_GL_SetSwapInterval(1) < 0) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  initLegacyOpenGL();
}

void Window::close() {
  if (!window) {
    return;
  }
  
  SDL_GL_DeleteContext(glContext);
  glContext = nullptr;
  SDL_DestroyWindow(window);
  window = nullptr;
}

WindowOptions Window::getOptions() {
  return metallicar::options;
}

void Window::setOptions(const WindowOptions& options) {
  metallicar::options = options;
  
  // window
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
  
  updateProjection();
}

SDL_GLContext Window::getOpenGLContext() {
  return glContext;
}

void Window::setOpenGLContext(SDL_GLContext glContext) {
  if (!glContext) {
    Log::message(Log::Error, "Trying to set null OpenGL context");
    exit(0);
  }
  metallicar::glContext = glContext;
}

void Window::update() {
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  updateProjection();
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
  
  updateProjection();
}

static void updateProjection() {
  // viewport
  int x = 0, y = 0;
  int w = options.width, h = options.height;
  float gameRatio = float(options.gameWidth)/options.gameHeight;
  float ratio = float(options.width)/options.height;
  if (ratio > gameRatio) {
    w = h*gameRatio;
    x = (options.width - w)/2;
  }
  else if (ratio < gameRatio) {
    h = w/gameRatio;
    y = (options.height - h)/2;
  }
  glViewport(x, y, w, h);
  
  // projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, h, 0, -1, 1);
  glScalef(float(w)/options.gameWidth, float(h)/options.gameHeight, 0.0f);
  
  // modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

} // namespace metallicar

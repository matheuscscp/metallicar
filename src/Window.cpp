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
#include "SDL_opengl.h"

// local
#include "metallicar_asset.hpp"
#include "metallicar_graphics.hpp"
#include "Log.hpp"
#include "Path.hpp"

namespace metallicar {

// =============================================================================
// private globals
// =============================================================================

static WindowOptions options;
static SDL_Window* window = nullptr;
static SDL_GLContext glContext = nullptr;
static bool optionsChanged = false;

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
    SDL_Surface* iconsurface = Image::createSDL_Surface(options.icon);
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  SDL_ShowCursor(options.cursor ? 1 : 0);
  
  // gl context
  if ((glContext = SDL_GL_CreateContext(window)) == nullptr) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
  if (SDL_GL_SetSwapInterval(1) < 0) {
    Log::message(Log::Error, SDL_GetError());
    exit(0);
  }
}

void Window::close() {
  if (!window) {
    return;
  }
  
  if (glContext) {
    SDL_GL_DeleteContext(glContext);
    glContext = nullptr;
  }
  SDL_DestroyWindow(window);
  window = nullptr;
}

WindowOptions Window::getOptions() {
  return metallicar::options;
}

void Window::setOptions(const WindowOptions& options) {
  if (!window) {
    return;
  }
  
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
    SDL_Surface* iconsurface = Image::createSDL_Surface(options.icon);
    SDL_SetWindowIcon(window, iconsurface);
    SDL_FreeSurface(iconsurface);
  }
  SDL_ShowCursor(options.cursor ? 1 : 0);
  
  optionsChanged = true;
}

geometry::Point2 Window::gameCoordinates(
  const geometry::Point2& windowCoordinates, bool& out
) {
  return metallicar::options.gameCoordinates(windowCoordinates, out);
}

void Window::setOpenGLVersion(int major, int minor) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
}

SDL_GLContext Window::createOpenGLContext() {
  if (!window) {
    return nullptr;
  }
  return SDL_GL_CreateContext(window);
}

void Window::setOpenGLContext(SDL_GLContext glContext) {
  if (window) {
    metallicar::glContext = glContext;
    SDL_GL_MakeCurrent(window, glContext);
  }
}

SDL_GLContext Window::getOpenGLContext() {
  return glContext;
}

void Window::destroyOpenGLContext(SDL_GLContext glContext) {
  if (window) {
    SDL_GL_DeleteContext(glContext);
  }
}

void Window::update() {
  if (window) {
    if (optionsChanged) {
      optionsChanged = false;
      Graphics::updateProjection();
    }
    
    SDL_GL_SwapWindow(window);
  }
}

} // namespace metallicar

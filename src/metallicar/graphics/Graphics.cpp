/*
 * Graphics.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_graphics.hpp"

// lib
#include "SDL_opengl.h"

// local
#include "metallicar_io.hpp"

using namespace std;

namespace metallicar {

static function<void()> updateProjection([]() {});
static function<void()> prepareFrame([]() {});
static function<void()> finalizeFrame([]() {});

void Graphics::initDefaultFunctions() {
  metallicar::updateProjection = []() {
    // viewport
    WindowOptions options = Window::getOptions();
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
    
    glMatrixMode(GL_MODELVIEW);
  };
  
  metallicar::prepareFrame = []() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
  };
  
  metallicar::finalizeFrame = []() {
    
  };
  
  // enable texture
  glEnable(GL_TEXTURE_2D);
  
  // enable blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  metallicar::updateProjection();
}

void Graphics::setProjectionUpdater(const function<void()>& updater) {
  metallicar::updateProjection = updater;
}

void Graphics::setFramePreparation(const function<void()>& preparation) {
  metallicar::prepareFrame = preparation;
}

void Graphics::setFrameFinalization(const function<void()>& finalization) {
  metallicar::finalizeFrame = finalization;
}

void Graphics::updateProjection() {
  metallicar::updateProjection();
}

void Graphics::prepareFrame() {
  metallicar::prepareFrame();
}

void Graphics::finalizeFrame() {
  metallicar::finalizeFrame();
}

} // namespace metallicar

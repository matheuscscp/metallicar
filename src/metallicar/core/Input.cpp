/*
 * Input.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// lib
#include "SDL.h"

using namespace std;

namespace metallicar {

static bool quitRequested = false;
static map<SDL_Keycode, bool> keys;
static map<uint8_t, bool> buttons;
static Point mouse, mouseDown, mouseUp;
static observer::Subject subject;

Input::KeyEvent::KeyEvent(uint32_t eventType, SDL_Keycode keycode) :
observer::Event(eventType), keycode(keycode)
{
  
}

SDL_Keycode Input::KeyEvent::key() const {
  return keycode;
}

Input::ButtonEvent::ButtonEvent(uint32_t eventType, uint8_t buttoncode) :
observer::Event(eventType), buttoncode(buttoncode)
{
  
}

uint32_t Input::ButtonEvent::button() const {
  return buttoncode;
}

void Input::update() {
  int x, y;
  SDL_GetMouseState(&x, &y);
  metallicar::mouse.x = float(x);
  metallicar::mouse.y = float(y);
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (!event.key.repeat) {
          keys[event.key.keysym.sym] = true;
          subject.broadcast(KeyEvent(SDL_KEYDOWN, event.key.keysym.sym));
        }
        break;
        
      case SDL_KEYUP:
        keys[event.key.keysym.sym] = false;
        subject.broadcast(KeyEvent(SDL_KEYUP, event.key.keysym.sym));
        break;
        
      case SDL_MOUSEBUTTONDOWN:
        buttons[event.button.button] = true;
        metallicar::mouseDown = metallicar::mouse;
        subject.broadcast(ButtonEvent(SDL_MOUSEBUTTONDOWN, event.button.button));
        break;
        
      case SDL_MOUSEBUTTONUP:
        buttons[event.button.button] = false;
        metallicar::mouseUp = metallicar::mouse;
        subject.broadcast(ButtonEvent(SDL_MOUSEBUTTONUP, event.button.button));
        break;
        
      case SDL_QUIT:
        metallicar::quitRequested = true;
        subject.broadcast(observer::Event(SDL_QUIT));
        break;
        
      default:
        break;
    }
  }
}

bool Input::quitRequested() {
  return metallicar::quitRequested;
}

void Input::resetQuitRequest() {
  metallicar::quitRequested = false;
}

bool Input::key(SDL_Keycode keycode) {
  auto it = keys.find(keycode);
  if (it == keys.end()) {
    return false;
  }
  return it->second;
}

bool Input::button(uint8_t buttoncode) {
  auto it = buttons.find(buttoncode);
  if (it == buttons.end()) {
    return false;
  }
  return it->second;
}

Point Input::mouse() {
  return metallicar::mouse;
}

Point Input::mouseDown() {
  return metallicar::mouseDown;
}

Point Input::mouseUp() {
  return metallicar::mouseUp;
}

observer::Connection Input::connect(uint32_t eventType, const function<void(const observer::Event&)>& callback) {
  return subject.connect(eventType, callback);
}

} // namespace metallicar

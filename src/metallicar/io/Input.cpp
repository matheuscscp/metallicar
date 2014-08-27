/*
 * Input.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_io.hpp"

// lib
#include "SDL.h"

using namespace std;
using namespace geometry;

namespace metallicar {

static bool quitRequested = false;
static map<SDL_Keycode, bool> keys;
static map<uint8_t, bool> buttons;
static Point2 mouse, mouseDown, mouseUp;
observer::Subject Input::subject;

// =============================================================================
// EventQueue
// =============================================================================
struct EventNode {
  SDL_Event event;
  EventNode* next;
  EventNode() : next(nullptr) {
    
  }
};
static EventNode* firstEventNode = new EventNode;
static EventNode* lastEventNode = firstEventNode; // last node is always empty
class EventQueue {
  public:
    static void push(const SDL_Event& event) {
      EventNode* newNode = lastEventNode;
      lastEventNode = new EventNode;
      newNode->event = event;
      newNode->next = lastEventNode;
    }
    
    static bool pop(SDL_Event& event) {
      if (firstEventNode->next == nullptr) {
        return false;
      }
      event = firstEventNode->event;
      EventNode* tmp = firstEventNode;
      firstEventNode = firstEventNode->next;
      delete tmp;
      return true;
    }
};
// =============================================================================
// EventQueue
// =============================================================================

Input::KeyDownEvent::KeyDownEvent(SDL_Keycode keycode) : keycode(keycode) {
  
}

SDL_Keycode Input::KeyDownEvent::key() const {
  return keycode;
}

Input::KeyUpEvent::KeyUpEvent(SDL_Keycode keycode) : keycode(keycode) {
  
}

SDL_Keycode Input::KeyUpEvent::key() const {
  return keycode;
}

Input::ButtonDownEvent::ButtonDownEvent(uint8_t buttoncode) :
buttoncode(buttoncode)
{
  
}

uint32_t Input::ButtonDownEvent::button() const {
  return buttoncode;
}

Input::ButtonUpEvent::ButtonUpEvent(uint8_t buttoncode) :
buttoncode(buttoncode)
{
  
}

uint32_t Input::ButtonUpEvent::button() const {
  return buttoncode;
}

void Input::pollWindowEvents() {
  int pending;
  SDL_Event event;
  event.type = SDL_FIRSTEVENT;
  do {
    pending = SDL_PollEvent(&event);
    EventQueue::push(event);
  } while (pending);
}

void Input::pollEvents() {
  static bool updateMouse = true;
  int x, y;
  SDL_GetMouseState(&x, &y);
  bool out;
  Point2 mouse(Window::gameCoordinates(Point2(float(x), float(y)), out));
  if (!out) {
    metallicar::mouse = mouse;
    updateMouse = true;
  }
  else if (updateMouse) {
    updateMouse = false;
    metallicar::mouse = mouse;
  }
  SDL_Event event;
  while (EventQueue::pop(event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (!keys[event.key.keysym.sym]) {
          keys[event.key.keysym.sym] = true;
          subject.broadcast(KeyDownEvent(event.key.keysym.sym));
        }
        break;
        
      case SDL_KEYUP:
        keys[event.key.keysym.sym] = false;
        subject.broadcast(KeyUpEvent(event.key.keysym.sym));
        break;
        
      case SDL_MOUSEBUTTONDOWN:
        buttons[event.button.button] = true;
        metallicar::mouseDown = metallicar::mouse;
        subject.broadcast(ButtonDownEvent(event.button.button));
        break;
        
      case SDL_MOUSEBUTTONUP:
        buttons[event.button.button] = false;
        metallicar::mouseUp = metallicar::mouse;
        subject.broadcast(ButtonUpEvent(event.button.button));
        break;
        
      case SDL_QUIT:
        metallicar::quitRequested = true;
        subject.broadcast(QuitEvent());
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

Point2 Input::mouse() {
  return metallicar::mouse;
}

Point2 Input::mouseDown() {
  return metallicar::mouseDown;
}

Point2 Input::mouseUp() {
  return metallicar::mouseUp;
}

} // namespace metallicar

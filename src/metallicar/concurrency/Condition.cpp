/*
 * Condition.cpp
 *
 *  Created on: Jul 18, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Condition::Condition() : condition(SDL_CreateCond()), mutex(SDL_CreateMutex()) {
  
}

Condition::~Condition() {
  SDL_DestroyCond(condition);
  SDL_DestroyMutex(mutex);
}

bool Condition::wait(uint32_t ms) {
  bool ret = true;
  SDL_LockMutex(mutex);
  if (!ms) {
    SDL_CondWait(condition, mutex);
  }
  else {
    ret = (SDL_CondWaitTimeout(condition, mutex, ms) == 0);
  }
  SDL_UnlockMutex(mutex);
  return ret;
}

void Condition::signal() {
  SDL_LockMutex(mutex);
  SDL_CondSignal(condition);
  SDL_UnlockMutex(mutex);
}

void Condition::broadcast() {
  SDL_LockMutex(mutex);
  SDL_CondBroadcast(condition);
  SDL_UnlockMutex(mutex);
}

} // namespace metallicar

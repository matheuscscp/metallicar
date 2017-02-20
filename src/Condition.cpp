/*
 * Condition.cpp
 *
 *  Created on: Jul 18, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Condition::Condition() : condition(nullptr), mutex(nullptr) {
  
}

Condition::~Condition() {
  if (condition) {
    SDL_DestroyCond(condition);
    SDL_DestroyMutex(mutex);
  }
}

void Condition::init() {
  if (!condition) {
    condition = SDL_CreateCond();
    mutex = SDL_CreateMutex();
  }
}

void Condition::close() {
  if (condition) {
    SDL_DestroyCond(condition);
    SDL_DestroyMutex(mutex);
    condition = nullptr;
    mutex = nullptr;
  }
}

void Condition::wait() {
  if (condition) {
    SDL_LockMutex(mutex);
    SDL_CondWait(condition, mutex);
    SDL_UnlockMutex(mutex);
  }
}

bool Condition::wait(uint32_t ms) {
  if (condition) {
    bool ret;
    SDL_LockMutex(mutex);
    ret = (SDL_CondWaitTimeout(condition, mutex, ms) == 0);
    SDL_UnlockMutex(mutex);
    return ret;
  }
  return false;
}

void Condition::signal() {
  if (condition) {
    SDL_LockMutex(mutex);
    SDL_CondSignal(condition);
    SDL_UnlockMutex(mutex);
  }
}

void Condition::broadcast() {
  if (condition) {
    SDL_LockMutex(mutex);
    SDL_CondBroadcast(condition);
    SDL_UnlockMutex(mutex);
  }
}

} // namespace metallicar

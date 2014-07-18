/*
 * Mutex.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Mutex::Mutex() : mutex(SDL_CreateMutex()) {
  
}

Mutex::~Mutex() {
  SDL_DestroyMutex(mutex);
}

void Mutex::lock() {
  SDL_LockMutex(mutex);
}

void Mutex::unlock() {
  SDL_UnlockMutex(mutex);
}

bool Mutex::tryLock() {
  return SDL_TryLockMutex(mutex) == 0;
}

} // namespace metallicar

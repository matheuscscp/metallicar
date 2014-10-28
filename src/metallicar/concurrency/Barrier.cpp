/*
 * Barrier.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Barrier::Barrier() :
condition(nullptr),
mutex(nullptr),
n_threads(0),
blocked(0)
{
  
}

Barrier::~Barrier() {
  if (condition) {
    SDL_DestroyCond(condition);
    SDL_DestroyMutex(mutex);
  }
}

void Barrier::init(unsigned n_threads) {
  if (!condition) {
    condition = SDL_CreateCond();
    mutex = SDL_CreateMutex();
    this->n_threads = n_threads;
    blocked = 0;
  }
}

void Barrier::close() {
  if (condition) {
    SDL_DestroyCond(condition);
    SDL_DestroyMutex(mutex);
    condition = nullptr;
    mutex = nullptr;
  }
}

void Barrier::wait() {
  if (condition) {
    SDL_LockMutex(mutex);
    if (blocked == n_threads - 1) {
      blocked = 0;
      SDL_CondBroadcast(condition);
    }
    else {
      blocked++;
      SDL_CondWait(condition, mutex);
    }
    SDL_UnlockMutex(mutex);
  }
}

} // namespace metallicar

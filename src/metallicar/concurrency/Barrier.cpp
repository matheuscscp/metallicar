/*
 * Barrier.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Barrier::Barrier(unsigned n_threads) :
condition(SDL_CreateCond()),
mutex(SDL_CreateMutex()),
n_threads(n_threads),
blocked(0)
{
  
}

Barrier::~Barrier() {
  SDL_DestroyCond(condition);
  SDL_DestroyMutex(mutex);
}

void Barrier::wait() {
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

} // namespace metallicar

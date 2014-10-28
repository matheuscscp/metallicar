/*
 * Atomic.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

using namespace std;

namespace metallicar {

map<void*, SDL_mutex*> Atomic::mutexes;

Atomic::Guard::Guard(void* function) {
  auto it = mutexes.find(function);
  if (it != mutexes.end()) {
    mutex = it->second;
  }
  else {
    auto& mutex = mutexes[function];
    mutex = SDL_CreateMutex();
    this->mutex = mutex;
  }
  SDL_LockMutex(mutex);
}

Atomic::Guard::~Guard() {
  SDL_UnlockMutex(mutex);
}

void Atomic::clear() {
  for (auto it = mutexes.begin(); it != mutexes.end();) {
    auto to_erase = it;
    it++;
    SDL_DestroyMutex(to_erase->second);
    mutexes.erase(to_erase);
  }
}

} // namespace metallicar

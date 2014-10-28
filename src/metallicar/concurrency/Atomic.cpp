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

SDL_mutex* Atomic::getMutex(void* function) {
  auto it = mutexes.find(function);
  if (it != mutexes.end()) {
    return it->second;
  }
  auto& mutex = mutexes[function];
  mutex = SDL_CreateMutex();
  return mutex;
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

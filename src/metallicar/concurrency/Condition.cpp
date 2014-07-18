/*
 * Condition.cpp
 *
 *  Created on: Jul 18, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Condition::Condition() {
  condition = SDL_CreateCond();
}

Condition::~Condition() {
  SDL_DestroyCond(condition);
}

void Condition::wait(Mutex& mutex) {
  SDL_CondWait(condition, *((SDL_mutex**)&mutex));
}

void Condition::signal() {
  SDL_CondSignal(condition);
}

bool Condition::waitTimeout(Mutex& mutex, uint32_t ms) {
  return SDL_CondWaitTimeout(condition, *((SDL_mutex**)&mutex), ms) == 0;
}

void Condition::broadcast() {
  SDL_CondBroadcast(condition);
}

} // namespace metallicar

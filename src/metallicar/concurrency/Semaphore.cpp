/*
 * Semaphore.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Semaphore::Semaphore(uint32_t initialValue) {
  initial_value = initialValue != 0 ? initialValue : 1;
  semaphore = SDL_CreateSemaphore(initial_value);
}

Semaphore::~Semaphore() {
  SDL_DestroySemaphore(semaphore);
}

void Semaphore::wait() {
  SDL_SemWait(semaphore);
}

void Semaphore::post() {
  SDL_SemPost(semaphore);
}

bool Semaphore::tryWait() {
  return SDL_SemTryWait(semaphore) == 0;
}

bool Semaphore::waitTimeout(uint32_t ms) {
  return SDL_SemWaitTimeout(semaphore, ms) == 0;
}

uint32_t Semaphore::value() const {
  return SDL_SemValue(semaphore);
}

uint32_t Semaphore::initialValue() const {
  return initial_value;
}

} // namespace metallicar

/*
 * Semaphore.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Semaphore::Semaphore() : semaphore(nullptr) {
  
}

Semaphore::~Semaphore() {
  if (semaphore) {
    SDL_DestroySemaphore(semaphore);
  }
}

void Semaphore::init(uint32_t initialValue) {
  if (!semaphore) {
    semaphore = SDL_CreateSemaphore(initialValue);
  }
}

void Semaphore::close() {
  if (semaphore) {
    SDL_DestroySemaphore(semaphore);
    semaphore = nullptr;
  }
}

void Semaphore::wait() {
  if (semaphore) {
    SDL_SemWait(semaphore);
  }
}

bool Semaphore::wait(uint32_t ms) {
  if (semaphore) {
    return (SDL_SemWaitTimeout(semaphore, ms) == 0);
  }
  return false;
}

void Semaphore::post() {
  if (semaphore) {
    SDL_SemPost(semaphore);
  }
}

bool Semaphore::trywait() {
  if (semaphore) {
    return (SDL_SemTryWait(semaphore) == 0);
  }
  return false;
}

uint32_t Semaphore::value() {
  if (semaphore) {
    return SDL_SemValue(semaphore);
  }
  return 0;
}

} // namespace metallicar

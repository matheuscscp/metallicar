/*
 * Semaphore.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

namespace metallicar {

Semaphore::Semaphore(uint32_t initialValue) :
semaphore(SDL_CreateSemaphore(initialValue))
{
  
}

Semaphore::~Semaphore() {
  SDL_DestroySemaphore(semaphore);
}

void Semaphore::wait() {
  SDL_SemWait(semaphore);
}

bool Semaphore::wait(uint32_t ms) {
  return (SDL_SemWaitTimeout(semaphore, ms) == 0);
}

void Semaphore::post() {
  SDL_SemPost(semaphore);
}

bool Semaphore::trywait() {
  return (SDL_SemTryWait(semaphore) == 0);
}

uint32_t Semaphore::value() {
  return SDL_SemValue(semaphore);
}

} // namespace metallicar

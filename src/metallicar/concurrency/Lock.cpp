/*
 * Lock.cpp
 *
 *  Created on: Aug 29, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

using namespace std;

namespace metallicar {

Lock::Lock() : mutex_(SDL_CreateMutex()) {
  
}

Lock::~Lock() {
  SDL_DestroyMutex(mutex_);
}

void Lock::mutexlock() {
  SDL_LockMutex(mutex_);
}

void Lock::spinlock() {
  while (SDL_TryLockMutex(mutex_));
}

bool Lock::trylock() {
  return (SDL_TryLockMutex(mutex_) == 0);
}

void Lock::unlock() {
  SDL_UnlockMutex(mutex_);
}

void Lock::mutex(const function<void()>& callback) {
  SDL_LockMutex(mutex_);
  callback();
  SDL_UnlockMutex(mutex_);
}

void Lock::spin(const function<void()>& callback) {
  while (SDL_TryLockMutex(mutex_));
  callback();
  SDL_UnlockMutex(mutex_);
}

} // namespace metallicar

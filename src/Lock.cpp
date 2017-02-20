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

Lock::Lock() : mutex_(nullptr) {
  
}

Lock::~Lock() {
  if (mutex_) {
    SDL_DestroyMutex(mutex_);
  }
}

void Lock::init() {
  if (!mutex_) {
    mutex_ = SDL_CreateMutex();
  }
}

void Lock::close() {
  if (mutex_) {
    SDL_DestroyMutex(mutex_);
    mutex_ = nullptr;
  }
}

void Lock::mutexlock() {
  if (mutex_) {
    SDL_LockMutex(mutex_);
  }
}

void Lock::spinlock() {
  if (mutex_) {
    while (SDL_TryLockMutex(mutex_));
  }
}

bool Lock::trylock() {
  if (mutex_) {
    return (SDL_TryLockMutex(mutex_) == 0);
  }
  return false;
}

void Lock::unlock() {
  if (mutex_) {
    SDL_UnlockMutex(mutex_);
  }
}

void Lock::mutex(const function<void()>& callback) {
  if (mutex_) {
    SDL_LockMutex(mutex_);
    callback();
    SDL_UnlockMutex(mutex_);
  }
}

void Lock::spin(const function<void()>& callback) {
  if (mutex_) {
    while (SDL_TryLockMutex(mutex_));
    callback();
    SDL_UnlockMutex(mutex_);
  }
}

} // namespace metallicar

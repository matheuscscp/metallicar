/*
 * metallicar_concurrency.hpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_CONCURRENCY_HPP_
#define METALLICAR_CONCURRENCY_HPP_

// standard
#include <functional>
#include <memory>

// lib
#include "SDL.h"

namespace metallicar {

class Thread {
  private:
    std::function<void()> callback;
    SDL_Thread* thread;
    bool joined;
    std::shared_ptr<bool> terminated;
  public:
    enum Priority {
      LOW, NORMAL, HIGH
    };
    
    Thread(const std::function<void()>& callback = []() {});
    ~Thread();
    Thread(const Thread& other);
    Thread& operator=(const Thread& other);
    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
    
    void start();
    void join();
    bool running() const;
    uint32_t getID();
    
    static uint32_t ID();
    static void sleep(uint32_t ms, const bool* keepCondition = nullptr);
    static void setPriority(Priority priority);
};

class Semaphore {
  private:
    SDL_sem* semaphore;
  public:
    Semaphore(uint32_t initialValue);
    ~Semaphore();
    bool wait(uint32_t ms = 0);
    void post();
    bool trywait();
    uint32_t value();
};

class Mutex {
  private:
    SDL_mutex* mutex;
  public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    bool trylock();
    void run(const std::function<void()>& callback);
};

class Condition {
  private:
    SDL_cond* condition;
    SDL_mutex* mutex;
  public:
    Condition();
    ~Condition();
    bool wait(uint32_t ms = 0);
    void signal();
    void broadcast();
};

template <class T>
class Atomic {
  private:
    std::unique_ptr<T> val;
    Mutex mutex;
  public:
    template <typename... Args>
    Atomic(Args&&... args) : val(new T(std::forward<Args>(args)...)) {
      
    }
    
    ~Atomic() {
      
    }
    
    void lock() {
      mutex.lock();
    }
    
    void unlock() {
      mutex.unlock();
    }
    
    T& value() const {
      return *(val.get());
    }
};

} // namespace metallicar

#endif /* METALLICAR_CONCURRENCY_HPP_ */

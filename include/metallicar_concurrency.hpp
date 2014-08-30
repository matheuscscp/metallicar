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
    static void sleep(uint32_t ms);
    static void sleep(uint32_t ms, const bool& wakeup);
    static void setPriority(Priority priority);
};

class Semaphore {
  private:
    SDL_sem* semaphore;
  public:
    Semaphore(uint32_t initialValue);
    ~Semaphore();
    void wait();
    bool wait(uint32_t ms);
    void post();
    bool trywait();
    uint32_t value();
};

class Lock {
  private:
    SDL_mutex* mutex_;
  public:
    Lock();
    ~Lock();
    void mutexlock();
    void spinlock();
    bool trylock();
    void unlock();
    void mutex(const std::function<void()>& callback);
    void spin(const std::function<void()>& callback);
};

class Condition {
  private:
    SDL_cond* condition;
    SDL_mutex* mutex;
  public:
    Condition();
    ~Condition();
    void wait();
    bool wait(uint32_t ms);
    void signal();
    void broadcast();
};

} // namespace metallicar

#endif /* METALLICAR_CONCURRENCY_HPP_ */

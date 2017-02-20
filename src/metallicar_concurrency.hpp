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
#include <map>

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
    Semaphore();
    ~Semaphore();
    void init(uint32_t initialValue);
    void close();
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
    void init();
    void close();
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
    void init();
    void close();
    void wait();
    bool wait(uint32_t ms);
    void signal();
    void broadcast();
};

class Barrier {
  private:
    SDL_cond* condition;
    SDL_mutex* mutex;
    unsigned n_threads, blocked;
  public:
    Barrier();
    ~Barrier();
    void init(unsigned n_threads);
    void close();
    void wait();
};

template <class T>
class Monitor {
  public:
    class Guard {
      private:
        SDL_mutex* mutex;
      public:
        Guard(Monitor<T>& monitor) : mutex(monitor.mutex) {
          if (mutex) {
            SDL_LockMutex(mutex);
          }
        }
        
        ~Guard() {
          if (mutex) {
            SDL_UnlockMutex(mutex);
          }
        }
    };
  private:
    T object;
    SDL_mutex* mutex;
  public:
    template <typename... Args>
    Monitor(Args&&... args) :
    object(std::forward<Args>(args)...),
    mutex(nullptr)
    {
      
    }
    
    ~Monitor() {
      if (mutex) {
        SDL_DestroyMutex(mutex);
      }
    }
    
    void init() {
      if (!mutex) {
        mutex = SDL_CreateMutex();
      }
    }
    
    void close() {
      if (mutex) {
        SDL_DestroyMutex(mutex);
        mutex = nullptr;
      }
    }
    
    T* operator->() {
      return &object;
    }
    
    template <typename Method, typename... Args>
    typename std::result_of<Method(T*, Args...)>::type call(Method method, Args&&... args) {
      Guard guard(*this);
      return (object.*method)(std::forward<Args>(args)...);
    }
};

class Atomic {
  private:
    class Guard {
      private:
        SDL_mutex* mutex;
      public:
        Guard(void* function);
        ~Guard();
    };
    
    static std::map<void*, SDL_mutex*> mutexes;
  public:
    static void clear();
    
    template <typename Function, typename... Args>
    static typename std::result_of<Function(Args...)>::type call(Function function, Args&&... args) {
      Guard guard((void*)function);
      return (*function)(std::forward<Args>(args)...);
    }
};

} // namespace metallicar

#endif /* METALLICAR_CONCURRENCY_HPP_ */

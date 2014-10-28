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

class Barrier {
  private:
    SDL_cond* condition;
    SDL_mutex* mutex;
    unsigned n_threads, blocked;
  public:
    Barrier(unsigned n_threads);
    ~Barrier();
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
          SDL_LockMutex(mutex);
        }
        
        ~Guard() {
          SDL_UnlockMutex(mutex);
        }
    };
  private:
    SDL_mutex* mutex;
    T object;
  public:
    template <typename... Args>
    Monitor(Args&&... args) :
    mutex(SDL_CreateMutex()),
    object(std::forward<Args>(args)...)
    {
      
    }
    
    ~Monitor() {
      SDL_DestroyMutex(mutex);
    }
    
    T* operator->() {
      return &object;
    }
    
    template <typename Method, typename... Args>
    typename std::result_of<Method(T*, Args...)>::type func(Method method, Args&&... args) {
      SDL_LockMutex(mutex);
      typename std::result_of<Method(T*, Args...)>::type ret = (object.*method)(std::forward<Args>(args)...);
      SDL_UnlockMutex(mutex);
      return ret;
    }
    
    template <typename Method, typename... Args>
    void proc(Method method, Args&&... args) {
      SDL_LockMutex(mutex);
      (object.*method)(std::forward<Args>(args)...);
      SDL_UnlockMutex(mutex);
    }
};

class Atomic {
  private:
    static std::map<void*, SDL_mutex*> mutexes;
    
    static SDL_mutex* getMutex(void* function);
  public:
    static void clear();
    
    template <typename Function, typename... Args>
    static typename std::result_of<Function(Args...)>::type func(Function function, Args&&... args) {
      SDL_mutex* mutex = getMutex((void*)function);
      SDL_LockMutex(mutex);
      typename std::result_of<Function(Args...)>::type ret = (*function)(std::forward<Args>(args)...);
      SDL_UnlockMutex(mutex);
      return ret;
    }
    
    template <typename Procedure, typename... Args>
    static void proc(Procedure procedure, Args&&... args) {
      SDL_mutex* mutex = getMutex((void*)procedure);
      SDL_LockMutex(mutex);
      (*procedure)(std::forward<Args>(args)...);
      SDL_UnlockMutex(mutex);
    }
};

} // namespace metallicar

#endif /* METALLICAR_CONCURRENCY_HPP_ */

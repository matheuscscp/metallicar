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
    bool* terminated;
  public:
    enum Priority {
      LOW, NORMAL, HIGH
    };
    
    Thread(const std::function<void()>& callback);
    ~Thread();
    Thread(const Thread& other);
    Thread& operator=(const Thread& other);
    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
    
    void start();
    void join();
    bool running() const;
    uint32_t getID() const;
    
    static uint32_t ID();
    static void sleep(uint32_t ms, const bool* keepCondition = nullptr);
    static void setPriority(Priority priority);
};

class Mutex {
  private:
    SDL_mutex* mutex;
  public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    bool tryLock();
};

class Semaphore {
  private:
    uint32_t initial_value;
    SDL_sem* semaphore;
  public:
    Semaphore(uint32_t initialValue);
    ~Semaphore();
    void wait();
    void post();
    bool tryWait();
    bool waitTimeout(uint32_t ms);
    uint32_t value() const;
    uint32_t initialValue() const;
};

class Condition {
  private:
    SDL_cond* condition;
  public:
    Condition();
    ~Condition();
    void wait(Mutex& mutex);
    void signal();
    bool waitTimeout(Mutex& mutex, uint32_t ms);
    void broadcast();
};

class AtomicBase {
  protected:
    AtomicBase();
  public:
    virtual ~AtomicBase();
};

template <class T>
class Atomic : public AtomicBase {
  private:
    std::unique_ptr<T> val;
    Mutex mutex;
  public:
    Atomic(T* val = nullptr) : val(val ? val : new T) {
      
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

class AtomicGlobals {
  private:
    static std::map<std::string, AtomicBase*> atomics;
  public:
    template <typename T> static Atomic<T>* get(const std::string& key) {
      auto atomic = atomics.find(key);
      if (atomic == atomics.end()) {
        return nullptr;
      }
      return (Atomic<T>*)atomic->second;
    }
    static void put(const std::string& key, AtomicBase* value);
    static void erase(const std::string& key);
};

} // namespace metallicar

#endif /* METALLICAR_CONCURRENCY_HPP_ */

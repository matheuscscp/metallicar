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
    Thread(const std::function<void()>& callback);
    ~Thread();
    Thread(const Thread& other);
    Thread& operator=(const Thread& other);
    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
    
    void start();
    void join();
    bool running();
    
    static void sleep(uint32_t ms, bool* keepCondition = nullptr);
  private:
    static int exec(void* threadInfo);
};

class Mutex {
  private:
    SDL_mutex* mutex;
  public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
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
    Atomic(T* val) : val(val) {
      
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
    template <typename T> inline static Atomic<T>* get(const std::string& key) {
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

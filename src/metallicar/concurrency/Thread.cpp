/*
 * Thread.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

using namespace std;

namespace metallicar {

static int exec(void* threadInfo);

class ThreadInfo {
  public:
    function<void()> callback;
    shared_ptr<bool> terminated;
    ThreadInfo(
      const function<void()>& callback,
      const shared_ptr<bool>& terminated
    ) : callback(callback), terminated(terminated) {
      
    }
};

Thread::Thread(const function<void()>& callback) :
callback(callback), thread(nullptr), joined(false), terminated(new bool)
{
  *terminated = false;
}

Thread::~Thread() {
#ifdef _WIN32
  if (!joined) {
    SDL_DetachThread(thread);
  }
#endif
}

Thread::Thread(const Thread& other) :
callback(other.callback),
thread(other.thread),
joined(other.joined),
terminated(other.terminated)
{
  auto tmp = (Thread&)other;
  
  tmp.joined = true;
  tmp.terminated.reset(new bool);
  *(tmp.terminated) = true;
}

Thread& Thread::operator=(const Thread& other) {
  callback = other.callback;
  thread = other.thread;
  joined = other.joined;
  terminated = other.terminated;
  
  auto tmp = (Thread&)other;
  
  tmp.joined = true;
  tmp.terminated.reset(new bool);
  *(tmp.terminated) = true;
  
  return *this;
}

Thread::Thread(Thread&& other) :
callback(other.callback),
thread(other.thread),
joined(other.joined),
terminated(other.terminated)
{
  other.joined = true;
  other.terminated.reset(new bool);
  *(other.terminated) = true;
}

Thread& Thread::operator=(Thread&& other) {
  callback = other.callback;
  thread = other.thread;
  joined = other.joined;
  terminated = other.terminated;
  
  other.joined = true;
  other.terminated.reset(new bool);
  *(other.terminated) = true;
  
  return *this;
}

void Thread::start() {
  if (thread || joined) {
    return;
  }
  thread = SDL_CreateThread(exec, nullptr, new ThreadInfo(callback, terminated));
}

void Thread::join() {
  if (joined) {
    return;
  }
  SDL_WaitThread(thread, nullptr);
  joined = true;
}

bool Thread::running() const {
  return thread && !(*terminated);
}

uint32_t Thread::getID() const {
  if (!thread || (*terminated)) {
    return 0;
  }
  return SDL_GetThreadID(thread);
}

uint32_t Thread::ID() {
  return SDL_ThreadID();
}

void Thread::sleep(uint32_t ms, const bool* keepCondition) {
  // for naps, or if there is no wakeup condition
  if (ms <= 50 || keepCondition == nullptr) {
    SDL_Delay(ms);
    return;
  }
  
  uint32_t now = SDL_GetTicks();
  uint32_t time = now + ms;
  do {
    ms = time - now;
    SDL_Delay(ms < 50 ? ms : 50);
    now = SDL_GetTicks();
  } while (now < time && *keepCondition);
}

void Thread::setPriority(Priority priority) {
  switch (priority) {
    case LOW:     SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);     break;
    case NORMAL:  SDL_SetThreadPriority(SDL_THREAD_PRIORITY_NORMAL);  break;
    case HIGH:    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);    break;
    default:
      break;
  }
}

static int exec(void* threadInfo) {
  auto info = (ThreadInfo*)threadInfo;
  info->callback();
  *(info->terminated) = true;
  delete info;
  return 0;
}

} // namespace metallicar

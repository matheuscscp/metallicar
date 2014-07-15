/*
 * Stopwatch.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_time.hpp"

namespace metallicar {

Stopwatch::Stopwatch() :
started(false), paused(false), initialTime(0), pauseTime(0)
{
  
}

void Stopwatch::start() {
  started = true;
  paused = false;
  initialTime = Time::get();
}

void Stopwatch::pause() {
  if (!paused) {
    paused = true;
    pauseTime = Time::get();
  }
}

void Stopwatch::resume() {
  if (paused) {
    paused = false;
    initialTime += Time::get() - pauseTime;
  }
}

void Stopwatch::reset() {
  started = false;
}

uint32_t Stopwatch::time() {
  if (!started)
    return 0;
  if (paused)
    return pauseTime - initialTime;
  return Time::get() - initialTime;
}

} // namespace metallicar

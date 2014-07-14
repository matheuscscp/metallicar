/*
 * DeltaTime.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "Time.hpp"

using namespace std;

namespace metallicar {
namespace engine {

static uint32_t last = 0;        // unit: milliseconds
static uint32_t ups = 32;        // unit: updates/second
static float dtFloat = 0.03125f; // unit: seconds
static uint32_t dtFixed = 31;    // unit: milliseconds
static uint32_t dt = 0;          // unit: milliseconds
static uint32_t updateID = 0;

uint32_t DeltaTime::getUPS() {
  return ups;
}

void DeltaTime::setUPS(uint32_t ups) {
  engine::ups = ups;
  dtFloat = 1/(float)ups;
  dtFixed = 1000/ups;
}

float DeltaTime::getDT() {
  return dtFloat;
}

uint32_t DeltaTime::updateID() {
  return engine::updateID;
}

void DeltaTime::init() {
  last = Time::get() - dtFixed;
}

void DeltaTime::update() {
  uint32_t now = Time::get();
  dt = now - last;
  last = now;
  engine::updateID = now - 1;
}

bool DeltaTime::periodReached() {
  if (dt >= dtFixed) {
    dt -= dtFixed;
    engine::updateID++;
    return true;
  }
  return false;
}

void DeltaTime::accumulate() {
  last -= dt;
}

} // engine metallicar
} // namespace metallicar

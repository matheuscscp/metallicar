/*
 * AtomicGlobals.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_concurrency.hpp"

using namespace std;

namespace metallicar {

map<string, AtomicBase*> AtomicGlobals::atomics;

void AtomicGlobals::put(const string& key, AtomicBase* value) {
  auto atomic = atomics.find(key);
  if (atomic != atomics.end()) {
    delete atomic->second;
    atomic->second = value;
  }
  else {
    atomics[key] = value;
  }
}

void AtomicGlobals::erase(const string& key) {
  auto atomic = atomics.find(key);
  if (atomic != atomics.end()) {
    delete atomic->second;
    atomics.erase(atomic);
  }
}

} // namespace metallicar

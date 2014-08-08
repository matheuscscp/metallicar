/*
 * Assets.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

using namespace std;

namespace metallicar {

static Assets* instance = nullptr;

Assets::Assets() {
  metallicar::instance = this;
}

Assets::~Assets() {
  for (auto& kv : assets) {
    delete kv.second;
  }
}

Assets& Assets::instance() {
  return *metallicar::instance;
}

} // namespace metallicar

/*
 * Assets.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

namespace metallicar {

static Assets* instance = nullptr;

Assets::Assets() {
  metallicar::instance = this;
}

Assets& Assets::instance(Assets* newInstance) {
  if (newInstance) {
    metallicar::instance = newInstance;
  }
  return *metallicar::instance;
}

} // namespace metallicar

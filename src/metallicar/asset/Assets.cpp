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

Assets* Assets::instance = nullptr;

Assets::Assets() {
  instance = this;
}

Assets::~Assets() {
  for (auto& kv : assets) {
    delete kv.second;
  }
}

} // namespace metallicar

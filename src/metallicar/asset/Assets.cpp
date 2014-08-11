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

void Assets::put(const string& name, Asset* asset) {
  instance->assets[name] = shared_ptr<Asset>(asset);
}

} // namespace metallicar

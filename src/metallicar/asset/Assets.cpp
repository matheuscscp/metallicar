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

map<string, shared_ptr<Asset>> Assets::assets;

void Assets::clean() {
  for (auto it = assets.begin(); it != assets.end();) {
    if (it->second.use_count() == 1) {
      assets.erase(it++);
    }
    else {
      it++;
    }
  }
}

void Assets::clear() {
  assets.clear();
}

} // namespace metallicar

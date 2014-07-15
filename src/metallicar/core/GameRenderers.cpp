/*
 * GameRenderers.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {

void GameRenderers::add(int z, const function<void()>& renderer) {
  renderers[z].push_back(renderer);
}

void GameRenderers::render() {
  for (auto it = renderers.begin(); it != renderers.end(); renderers.erase(it++)) {
    while (it->second.size()) {
      it->second.back()();
      it->second.pop_back();
    }
  }
}

} // namespace metallicar

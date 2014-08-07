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

static map<double, list<function<void()>>> renderers;

void GameRenderers::add(double order, const function<void()>& renderer) {
  renderers[order].push_back(renderer);
}

void GameRenderers::render() {
  for (auto& kv : renderers) {
    for (auto& renderer : kv.second) {
      renderer();
    }
  }
}

void GameRenderers::clear() {
  renderers.clear();
}

} // namespace metallicar

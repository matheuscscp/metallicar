/*
 * Graphics.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_graphics.hpp"

using namespace std;

namespace metallicar {

static function<void()> transformations;

void Graphics::setTransformations(const function<void()>& transfs) {
  transformations = transfs;
}

void Graphics::performTransformations() {
  transformations();
}

} // namespace metallicar

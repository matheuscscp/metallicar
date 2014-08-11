/*
 * Assets.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

namespace metallicar {

Assets* Assets::instance = nullptr;

Assets::Assets() {
  instance = this;
}

} // namespace metallicar

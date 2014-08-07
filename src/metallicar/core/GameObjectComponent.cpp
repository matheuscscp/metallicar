/*
 * GameObjectComponent.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {

GameObjectComponent::~GameObjectComponent() {
  
}

vector<string> GameObjectComponent::depends() const {
  return {};
}

} // namespace metallicar

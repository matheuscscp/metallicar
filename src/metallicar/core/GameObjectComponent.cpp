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

string GameObjectComponent::family() const {
  return "";
}

vector<string> GameObjectComponent::depends() const {
  return {};
}

void GameObjectComponent::init() {
  
}

void GameObjectComponent::update() {
  
}

bool GameObjectComponent::destroy() {
  return false;
}

} // namespace metallicar

/*
 * GameScene.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {

GameScene::GameScene() : frozen(false), visible(false) {
  quitEventConnection = Input::connect<Input::QuitEvent>(
    [](const observer::EventBase&) { Game::quit(); }
  );
}

GameScene::~GameScene() {
  
}

} // namespace metallicar

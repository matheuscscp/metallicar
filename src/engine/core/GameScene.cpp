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
namespace engine {

GameScene::GameScene() : frozen(false), visible(false) {
  quitEventConnection = Input::connect(SDL_QUIT, [](const util::Event&) { Game::quit(); });
}

GameScene::~GameScene() {
  
}

} // namespace engine
} // namespace metallicar

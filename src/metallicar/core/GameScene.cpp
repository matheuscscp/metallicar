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

static GameScene* instance = nullptr;
static GameScene* newInstance = nullptr;

GameScene::GameScene() {
  if (metallicar::newInstance) {
    delete metallicar::newInstance;
  }
  metallicar::newInstance = this;
  
  quitEventConnection = Input::connect<Input::QuitEvent>(
    [](const observer::EventBase&) { Game::quit(); }
  );
}

GameScene::~GameScene() {
  
}

GameScene& GameScene::instance() {
  return *metallicar::instance;
}

void GameScene::change() {
  if (metallicar::newInstance) {
    if (metallicar::instance) {
      delete metallicar::instance;
    }
    metallicar::instance = metallicar::newInstance;
    metallicar::newInstance = nullptr;
  }
}

void GameScene::close() {
  if (metallicar::newInstance) {
    delete metallicar::newInstance;
  }
  metallicar::newInstance = nullptr;
  if (metallicar::instance) {
    delete metallicar::instance;
  }
  metallicar::instance = nullptr;
}

} // namespace metallicar

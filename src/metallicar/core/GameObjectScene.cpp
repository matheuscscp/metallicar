/*
 * GameObjectScene.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {

GameObjectScene::~GameObjectScene() {
  while (objects.size()) {
    delete objects.back();
    objects.pop_back();
  }
  while (newObjects.size()) {
    delete newObjects.back();
    newObjects.pop_back();
  }
}

void GameObjectScene::add(GameObject* object) {
  newObjects.push_back(object);
}

void GameObjectScene::update() {
  for (auto object : objects) {
    if (!object->destroy && !object->frozen) {
      object->updateTree();
    }
  }
  while (newObjects.size()) {
    objects.push_back(newObjects.front());
    newObjects.pop_front();
  }
}

void GameObjectScene::render() {
  GameRenderers renderers;
  for (auto it = objects.begin(); it != objects.end();) {
    auto object = *it;
    if (!object->destroy) {
      if (!object->frozen || (object->frozen && object->visible)) {
        object->renderTree(renderers);
      }
      it++;
    }
    else {
      delete object;
      objects.erase(it++);
    }
  }
  renderers.render();
}

void GameObjectScene::wakeup(const GameArgs& args) {
  for (auto object : objects) {
    object->wakeupTree(args);
  }
}

} // namespace metallicar

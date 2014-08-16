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
  
}

void GameObjectScene::update() {
  while (newObjects.size()) {
    objects.push_back(newObjects.front());
    newObjects.pop_front();
  }
  for (auto it = objects.begin(); it != objects.end();) {
    if ((*it)->dead()) {
      objects.erase(it++);
    }
    else {
      (*it)->update();
      it++;
    }
  }
}

void GameObjectScene::render() {
  for (auto& object : objects) {
    object->render();
  }
}

void GameObjectScene::addObjects(
  const vector<shared_ptr<GameObject>>& objects
) {
  for (auto object : objects) {
    newObjects.push_back(object);
  }
}

FieldTable& GameObjectScene::fields() {
  return fieldTable;
}

GameObjectScene& GameObjectScene::runningInstance() {
  return (GameObjectScene&)Game::runningInstance();
}

} // namespace metallicar

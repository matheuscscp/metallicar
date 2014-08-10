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

void GameObjectScene::update() {
  while (newObjects.size()) {
    objects.push_back(newObjects.front());
    newObjects.pop_front();
  }
  for (auto it = objects.begin(); it != objects.end();) {
    if ((*it)->destroy()) {
      delete *it;
      objects.erase(it++);
    }
    else {
      (*it)->update();
      it++;
    }
  }
}

void GameObjectScene::render() {
  for (auto object : objects) {
    object->render();
  }
}

void GameObjectScene::addObjects(const vector<GameObject*>& objects) {
  for (auto object : objects) {
    newObjects.push_back(object);
  }
}

FieldTable& GameObjectScene::fields() {
  return fieldTable;
}

GameObjectScene& GameObjectScene::runningInstance() {
  return (GameObjectScene&)GameScene::runningInstance();
}

} // namespace metallicar

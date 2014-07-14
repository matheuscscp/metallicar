/*
 * GameObject.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

// this
#include "core.hpp"

using namespace std;

namespace metallicar {
namespace engine {

GameObject::GameObject(): destroy(false), frozen(false), visible(false) {
  
}

GameObject::~GameObject() {
  while (objects.size()) {
    delete objects.back();
    objects.pop_back();
  }
  while (newObjects.size()) {
    delete newObjects.back();
    newObjects.pop_back();
  }
}

void GameObject::add(GameObject* object) {
  newObjects.push_back(object);
}

void GameObject::updateTree() {
  update();
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

void GameObject::renderTree(GameRenderers* renderers) {
  render(renderers);
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
}

void GameObject::wakeupTree(void* args) {
  wakeup(args);
  for (auto object : objects) {
    object->wakeupTree(args);
  }
}

} // namespace engine
} // namespace metallicar

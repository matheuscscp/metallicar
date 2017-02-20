/*
 * CompositeGameObject.cpp
 *
 *  Created on: Aug 16, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {

CompositeGameObject::CompositeGameObject(
  const vector<shared_ptr<GameObject>>& objects
) {
  addObjects(objects);
}

CompositeGameObject::~CompositeGameObject() {
  
}

void CompositeGameObject::update() {
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

void CompositeGameObject::render() {
  for (auto& object : objects) {
    object->render();
  }
}

bool CompositeGameObject::dead() {
  return !objects.size() && !newObjects.size();
}

void CompositeGameObject::addObjects(
  const vector<shared_ptr<GameObject>>& objects
) {
  for (auto& object : objects) {
    newObjects.push_back(object);
  }
}

FieldTable& CompositeGameObject::fields() {
  return fieldTable;
}

} // namespace metallicar

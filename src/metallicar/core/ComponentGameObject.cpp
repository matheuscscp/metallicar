/*
 * ComponentGameObject.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_core.hpp"

// standard
#include <set>

using namespace std;

namespace metallicar {

ComponentGameObject::ComponentGameObject(
  const vector<GameObjectComponent*>& components
) {
  addComponents(components);
}

ComponentGameObject::~ComponentGameObject() {
  while (components.size()) {
    delete components.back();
    components.pop_back();
  }
  while (newComponents.size()) {
    delete newComponents.back();
    newComponents.pop_back();
  }
}

void ComponentGameObject::update() {
  set<string> newCompsFamilies;
  for (auto comp : newComponents) {
    components.push_back(comp);
    newCompsFamilies.insert(comp->family());
  }
  while (newComponents.size()) {
    auto comp = newComponents.front(); newComponents.pop_front();
    auto depends = comp->depends();
    auto alreadyInit = depends.size(); alreadyInit = 0;
    for (auto dep : depends) {
      if (newCompsFamilies.find(dep) == newCompsFamilies.end()) {
        alreadyInit++;
      }
    }
    if (alreadyInit == depends.size()) {
      comp->init();
      newCompsFamilies.erase(comp->family());
    }
    else {
      newComponents.push_back(comp);
    }
  }
  for (auto it = components.begin(); it != components.end();) {
    if ((*it)->destroy()) {
      delete *it;
      components.erase(it++);
    }
    else {
      (*it)->update();
      it++;
    }
  }
}

void ComponentGameObject::render() {
  
}

bool ComponentGameObject::destroy() {
  return fieldTable.read<bool>("destroy");
}

void ComponentGameObject::addComponents(
  const vector<GameObjectComponent*>& components
) {
  for (auto component : components) {
    component->object = this;
    newComponents.push_back(component);
  }
}

FieldTable& ComponentGameObject::fields() {
  return fieldTable;
}

} // namespace metallicar

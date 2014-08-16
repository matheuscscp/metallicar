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

// =============================================================================
// ComponentGameObject::Component
// =============================================================================

ComponentGameObject::Component::~Component() {
  
}

string ComponentGameObject::Component::family() const {
  return "";
}

vector<string> ComponentGameObject::Component::depends() const {
  return {};
}

void ComponentGameObject::Component::init() {
  
}

void ComponentGameObject::Component::update() {
  
}

bool ComponentGameObject::Component::destroy() {
  return false;
}

// =============================================================================
// ComponentGameObject
// =============================================================================

ComponentGameObject::ComponentGameObject(const vector<Component*>& components) {
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

bool ComponentGameObject::dead() {
  return fieldTable.read<bool>("dead");
}

void ComponentGameObject::addComponents(const vector<Component*>& components) {
  for (auto component : components) {
    component->object = this;
    newComponents.push_back(component);
  }
}

FieldTable& ComponentGameObject::fields() {
  return fieldTable;
}

} // namespace metallicar

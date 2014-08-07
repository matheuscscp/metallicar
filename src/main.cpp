/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>

#include "metallicar.hpp"

using namespace std;
using namespace metallicar;

class Space2D : public GameObjectComponent {
  private:
    string family() const {
      return "spatial";
    }
    void init() {
      object->fields().write("x", 400.0f);
      object->fields().write("y", 300.0f);
    }
    void update() {
      
    }
    bool destroy() {
      return false;
    }
};

class Renderer : public GameObjectComponent {
  public:
    Renderer() {
      
    }
  private:
    string family() const {
      return "renderer";
    }
    vector<string> depends() const {
      return {"spatial"};
    }
    void init() {
      
    }
    void update() {
      
    }
    bool destroy() {
      return false;
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  new GameObjectScene({new CompositeGameObject({new Space2D, new Renderer})});
  Game::run();
  return 0;
}

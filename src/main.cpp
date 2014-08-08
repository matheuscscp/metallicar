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
      object->fields().write("x", 640.0f);
      object->fields().write("y", 360.0f);
    }
    void update() {
      
    }
    bool destroy() {
      return false;
    }
};

class Renderer : public GameObjectComponent {
  public:
    Sprite bg, spr;
    Renderer() : bg("asset/background.png"), spr("asset/metallicar.png") {
      
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
      GameRenderers::add(0.0, [this]() {
        bg.render();
        spr.render(object->fields().read<float>("x"), object->fields().read<float>("y"), Corner::CENTER);
      });
    }
    bool destroy() {
      return false;
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Graphics::setTransformations([]() {
    glRotatef(30,0,0,1);
    glScalef(0.7,0.7,1);
  });
  (new GameObjectScene())->addObjects({
    new CompositeGameObject({new Space2D, new Renderer})
  });
  Game::run();
  return 0;
}

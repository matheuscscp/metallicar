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
    Sprite spr;
    Renderer() : spr("asset/metallicar.png") {
      
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
        //glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        spr.render(object->fields().read<float>("x"), object->fields().read<float>("y"), Corner::CENTER);
      });
    }
    bool destroy() {
      return false;
    }
};

int main(int argc, char* argv[]) {
  Game::init(WindowOptions("metallicar test", 1280, 720));
  (new GameObjectScene())->addObjects({
    new CompositeGameObject({new Space2D, new Renderer})
  });
  Game::run();
  return 0;
}

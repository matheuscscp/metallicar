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
      //FIXME
      object->fields().write("x", 400.0f);
      object->fields().write("y", 300.0f);
      printf("%f %f\n", object->fields().read<float>("x"), object->fields().read<float>("y"));
      fflush(stdout);
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
        //spr.render(object->fields().read<float>("x"), object->fields().read<float>("y"), Corner::CENTER);
        spr.render(640,360,Corner::CENTER);
      });
    }
    bool destroy() {
      return false;
    }
};

int main(int argc, char* argv[]) {
  Game::init(WindowOptions("teste", 1280, 720));
  (new GameObjectScene())->addObjects({
    new CompositeGameObject({new Space2D, new Renderer})
  });
  Game::run();
  return 0;
}

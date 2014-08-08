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
    Sprite bg, spr, spr2;
    Renderer() : bg("asset/background.png"), spr("asset/metallicar.png"), spr2("asset/icon.png") {
      
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
        spr2.render(0,0,Corner::TOP_LEFT,1,0,1,1,Color::WHITE,Input::key(SDLK_h));
      });
    }
    bool destroy() {
      return false;
    }
};

class ProjUpdater : public GameObjectComponent {
  public:
    float scale;
    ProjUpdater() : scale(1) {
      
    }
  private:
    string family() const {
      return "projection";
    }
    void init() {
      
    }
    void update() {
      if (Input::key(SDLK_a)) {
        scale += Game::getDT();
      }
      if (Input::key(SDLK_s)) {
        scale -= Game::getDT();
      }
      WindowOptions opts = Window::getOptions();
      opts.width = opts.gameWidth*scale;
      opts.height = opts.gameHeight*scale;
      Window::setOptions(opts);
    }
    bool destroy() {
      return false;
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  (new GameObjectScene())->addObjects({
    new CompositeGameObject({new Space2D, new Renderer, new ProjUpdater})
  });
  Game::run();
  return 0;
}

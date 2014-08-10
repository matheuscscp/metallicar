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
using namespace geometry;

class Space2D : public GameObjectComponent {
  public:
    string family() const {
      return "spatial";
    }
    void init() {
      object->fields().write("x", 640.0f);
      object->fields().write("y", 360.0f);
    }
};

class Renderer : public GameObjectComponent {
  public:
    Sprite bg, spr, spr2;
    Renderer() :
    bg(Assets::get<Texture2D>("asset/background.png", "asset/background.png")),
    spr(Assets::get<Texture2D>("asset/metallicar.png", "asset/metallicar.png")),
    spr2(Assets::get<Texture2D>("asset/icon.png", "asset/icon.png"))
    {
      spr.setCorner(Corner::CENTER);
    }
    vector<string> depends() const {
      return {"spatial"};
    }
    void update() {
      GameRenderers::add(0.0, [this]() {
        bg.render();
        spr.setPosition(Point2(object->fields().read<float>("x"), object->fields().read<float>("y")));
        spr.render();
        spr2.render();
      });
    }
};

class ProjUpdater : public GameObjectComponent {
  public:
    observer::Connection conn;
    ProjUpdater() {
      conn = Input::connect<Input::KeyDownEvent>([this](const observer::EventBase& event) {
        const Input::KeyDownEvent& keyEvent = (const Input::KeyDownEvent&)event;
        switch (keyEvent.key()) {
          case SDLK_a: {
            WindowOptions opts = Window::getOptions();
            opts.width = 480;
            opts.height = 640;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_s: {
            WindowOptions opts = Window::getOptions();
            opts.width = 800;
            opts.height = 600;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_d: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1024;
            opts.height = 768;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_f: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1280;
            opts.height = 720;
            Window::setOptions(opts);
            break;
          }
          
          case SDLK_g: {
            WindowOptions opts = Window::getOptions();
            opts.width = 1920;
            opts.height = 1080;
            Window::setOptions(opts);
            break;
          }
          
          default:
            break;
        }
      });
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Window::init();
  Graphics::initDefault();
  (new GameObjectScene())->addObjects({
    new CompositeGameObject({new Space2D, new Renderer, new ProjUpdater})
  });
  Game::run();
  Window::close();
  Game::close();
  return 0;
}

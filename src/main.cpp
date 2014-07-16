/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>

#include "metallicar.hpp"

using namespace metallicar;

class FirstScene : public GameObjectScene {
  private:
    class Player : public GameObject {
      private:
        void update() {
          
        }
        
        void render(GameRenderers& renderers) {
          
        }
        
        void wakeup(const GameArgs& args) {
          
        }
    };
  public:
    FirstScene() {
      add(new Player);
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Game::run(new FirstScene());
  return 0;
}

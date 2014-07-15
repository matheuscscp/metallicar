/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>
#include "metallicar.hpp"

using namespace metallicar::engine;

class FirstScene : public GameScene {
  public:
    FirstScene() {
      
    }
  protected:
    void update() {
      if (Input::quitRequested()) {
        Game::quit();
      }
    }
    
    void render() {
      
    }
    
    void wakeup(void* args) {
      
    }
};

int main(int argc, char* argv[]) {
  Game::init();
  Game::run(new FirstScene());
  return 0;
}

/*
 * main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include <cstdio>
#include "metallicar.hpp"

using namespace metallicar;

class FirstScene : public engine::GameScene {
  public:
    FirstScene() {
      
    }
  protected:
    void update() {
      
    }
    
    void render() {
      
    }
    
    void wakeup(void* args) {
      
    }
};

int main(int argc, char* argv[]) {
  engine::Game::init();
  engine::Game::run(new FirstScene());
  return 0;
}

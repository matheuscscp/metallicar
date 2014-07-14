/*
 * Main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include "metallicar.hpp"

class FirstScene : public metallicar::engine::GameScene {
  protected:
    void update() {
      
    }
    
    void render() {
      
    }
    
    void wakeup(void* args) {
      
    }
};

int main(int argc, char* argv[]) {
  metallicar::engine::Game::init();
  metallicar::engine::Game::run(new FirstScene());
  return 0;
}

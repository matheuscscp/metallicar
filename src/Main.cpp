/*
 * Main.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Pimenta
 */

#include "Game.hpp"

class MyGame : public metallicar::Game {
  public:
    
};

int main(int argc, char* argv[]) {
  MyGame myGame;
  myGame.run();
  return 0;
}

#include "Game.h"

int main(int argc, char* arcv[]) {
  Game game;
  game.Initialize();
  game.Run();
  game.Destroy();
  return 0;
}

#ifndef GAME_H
#define GAME_H
#include <SDL.h>
class Game
{
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;


  public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Destroy();
    void ProcessInput();
    void Update();
    void Render();
  
};


#endif


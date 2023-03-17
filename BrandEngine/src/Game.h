#ifndef GAME_H
#define GAME_H
#include <SDL.h>

class Game
{

  const int FPS = 60;
  const int MILLISECOND_PER_FRAME = 1000/FPS;

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    int previousMillisec;


  public:
    Game();
    ~Game();
    void Initialize();
    void Setup();
    void Run();
    void Destroy();
    void ProcessInput();
    void Update();
    void Render();
  
};


#endif


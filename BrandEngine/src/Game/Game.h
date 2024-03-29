#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../AssetManagement/AssetStore.h"

class Game
{

  const int FPS = 60;
  const int MILLISECOND_PER_FRAME = 1000/FPS;

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    bool isDebug;
    int previousMillisec;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;


  public:
    Game();
    ~Game();
    void Initialize();
    void Setup();
    void LoadLevel(int level);
    void Run();
    void Destroy();
    void ProcessInput();
    void Update();
    void Render();
  
};


#endif


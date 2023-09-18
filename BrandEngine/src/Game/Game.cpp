#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderDebugSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../AssetManagement/AssetStore.h"
#include "../Tilemap/Tilemap.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

Game::Game()
{
  isRunning = false;
  isDebug = false;
  registry = std::make_unique<Registry>();
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
  Logger::Log("Game constructor called");
}

Game::~Game()
{
  Logger::Log("Game destructor called");
}

void Game::Initialize()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    Logger::Err("SDL failed to initialize");
  }
  
  int displayWidth = 800;
  int displayHeight = 600;

  window = SDL_CreateWindow(
    NULL,                                                   // Title
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,         // Window X, Y Position
    displayWidth, displayHeight,                                               // WIndow height, width
    SDL_WINDOW_BORDERLESS                                   // Special Flags
  );
  if(!window)
  {
    Logger::Err("Window failed to create");
    return;
  }

  renderer = SDL_CreateRenderer(
    window,                                                 // Created window which new rendered should attach to
    -1,                                                     // Specfic driver to attach initialize with (-1 is default)                                                                                          
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC    // Special Flags
  );

  if(!renderer)
  {
    Logger::Err("Renderer failed to create");
    return;
  }

  //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::LoadLevel(int level) {
  registry -> AddSystem<MovementSystem>();
  registry -> AddSystem<RenderSystem>();
  registry -> AddSystem<AnimationSystem>();
  registry -> AddSystem<CollisionSystem>();
  registry -> AddSystem<RenderDebugSystem>();
  registry -> AddSystem<DamageSystem>();
  registry -> AddSystem<KeyboardMovementSystem>();

  assetStore -> CreateTexture(renderer, "tank-right", "./assets/images/tank-panther-right.png");
  assetStore -> CreateTexture(renderer, "truck-left", "./assets/images/truck-ford-left.png");
  assetStore -> CreateTexture(renderer, "chopper-frames-image", "./assets/images/chopper.png");
  assetStore -> CreateTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

  // Maybe make into a tilemap object
  Tilemap map = Tilemap("tilemap-image", 32);
  map.CreateMap(registry, "./assets/tilemaps/jungle.map");

  // Adding components to entity
  Entity tank = registry -> CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(10,3), glm::vec2(1.0,1.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(40,0));
  tank.AddComponent<SpriteComponent>("tank-right", 32, 32, 2);
  tank.AddComponent<BoxColliderComponent>(32,32);


  Entity truck = registry -> CreateEntity();
  truck.AddComponent<TransformComponent>(glm::vec2(200,3), glm::vec2(1.0,1.0), 0.0);
  truck.AddComponent<RigidBodyComponent>(glm::vec2(-60,0));
  truck.AddComponent<SpriteComponent>("truck-left", 32, 32, 2);
  truck.AddComponent<BoxColliderComponent>(32,32);

  Entity chopper = registry -> CreateEntity();
  chopper.AddComponent<TransformComponent>(glm::vec2(0,0), glm::vec2(1.0,1.0), 0.0);
  chopper.AddComponent<RigidBodyComponent>(glm::vec2(100,10));
  chopper.AddComponent<SpriteComponent>("chopper-frames-image", 32, 32, 2);
  chopper.AddComponent<AnimationComponent>(2, 0, 10, true);
}

void Game::Setup()
{
  Game::LoadLevel(1);
}

void Game::Destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Game::Run()
{
  Setup();
  while (isRunning)
  {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::ProcessInput()
{
  SDL_Event sdlEvent;
  while(SDL_PollEvent(&sdlEvent)) { // Poll event every frame
    switch (sdlEvent.type) {
      case SDL_QUIT:          // Stop running game if window is closed
        isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) { // Stop running game if esc key is pressed
          isRunning = false;
        }
        if (sdlEvent.key.keysym.sym == SDLK_d) {
          isDebug = !isDebug;
        }
        eventBus->EmitEvent<KeyPressedEvent>('A');
    }
  }
}

void Game::Update()
{
  // Simple way to cap runtime to force a given FPS
  int timeToWait = MILLISECOND_PER_FRAME - (SDL_GetTicks() - previousMillisec);
  if (timeToWait > 0 && timeToWait < MILLISECOND_PER_FRAME)
  {
    SDL_Delay(timeToWait);
  }

  double deltaTime = (SDL_GetTicks() - previousMillisec) / 1000.0;

  previousMillisec = SDL_GetTicks();

  // Reset the event subscriptions
  eventBus -> Reset();

  // Perform the subscription of events for all systems
  registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
  registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(eventBus);

  registry->GetSystem<MovementSystem>().Update(deltaTime);
  registry->GetSystem<AnimationSystem>().Update(deltaTime);
  registry->GetSystem<CollisionSystem>().Update(eventBus);

  registry->Update();
}

void Game::Render()
{
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  registry->GetSystem<RenderSystem>().Render(renderer, assetStore);
  if (isDebug) {
    registry->GetSystem<RenderDebugSystem>().Render(renderer);
  }

  SDL_RenderPresent(renderer);
}

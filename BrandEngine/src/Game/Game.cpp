#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game()
{
  isRunning = false;
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


void Game::Setup()
{
  // TODO...
  // Entity tank = Registry.CreateEntity()
  // tank.AddComponent<TransformComponent>();
  // tank.AddComponent<SpriteComponent>();
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
  while(SDL_PollEvent(&sdlEvent)) // Poll event every frame
  {
    switch (sdlEvent.type)
    {
      case SDL_QUIT:          // Stop running game if window is closed
        isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)  // Stop running game if esc key is pressed
        {
          isRunning = false;
        }
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

  // TODO:
  // MovementSystem.Update();
}

void Game::Render()
{
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);


  SDL_RenderPresent(renderer);
}
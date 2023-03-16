#include "Game.h"
#include <SDL.h>
#include <iostream>

Game::Game()
{
  isRunning = false;
  std::cout << "Game constructor called" << std::endl;
}

Game::~Game()
{
  std::cout << "Game destructor called" << std::endl;
}

void Game::Initialize()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "SDL failed to initialize" << std::endl;
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
    std::cerr << "Window failed to create" << std::endl;
  }

  renderer = SDL_CreateRenderer(
    window,                                                 // Created window which new rendered should attach to
    -1,                                                     // Specfic driver to attach initialize with (-1 is default)                                                                                          
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC    // Special Flags
  );

  if(!renderer)
  {
    std::cerr << "Renderer failed to create" << std::endl;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::Destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Game::Run()
{
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
}

void Game::Render()
{
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}

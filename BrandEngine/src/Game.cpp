#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
  playerPosition = { 10.0, 20.0 };
  playerVelocity = { 1.0, 0.0 };
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
  previousMillisec = SDL_GetTicks();

  playerPosition.x += playerVelocity.x;
  playerPosition.y += playerVelocity.y;
}

void Game::Render()
{
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect destRect = {
    static_cast<int>(playerPosition.x),
    static_cast<int>(playerPosition.y),
    32,
    32
  };
  SDL_RenderCopy(renderer, texture, NULL, &destRect);
  SDL_DestroyTexture(texture);

  SDL_RenderPresent(renderer);
}

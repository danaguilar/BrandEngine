#include "Game.h"
#include <SDL.h>
#include <iostream>

Game::Game()
{
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

  SDL_Window* window = SDL_CreateWindow(
    NULL,                                                   // Title
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,         // Window X, Y Position
    800, 600,                                               // WIndow height, width
    SDL_WINDOW_BORDERLESS                                   // Special Flags
  );
  if(!window)
  {
    std::cerr << "Window failed to create" << std::endl;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(
    window,      // Created window which new rendered should attach to
    -1,          // Specfic driver to attach initialize with (-1 is default)                                                                                          
    0            // Special Flags
  );

  if(!renderer)
  {
    std::cerr << "Renderer failed to create" << std::endl;
  }
}

void Game::Run()
{
}

void Game::Destroy()
{
}

void Game::ProcessInput()
{
}

void Game::Update()
{
}

void Game::Render()
{
}

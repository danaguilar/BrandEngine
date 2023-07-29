#include "AssetStore.h"
#include <SDL_image.h>
#include <SDL.h>
#include "../Logger/Logger.h"


AssetStore::AssetStore()
{
  Logger::Log("Asset Store Created");
}

AssetStore::~AssetStore() 
{
  Clear();
  Logger::Log("Asset Store Destroyed");
}
void AssetStore::Clear()
{
  for (auto texture : textures)
  {
    SDL_DestroyTexture(texture.second);
  }
  textures.clear();
}

void AssetStore::CreateTexture(SDL_Renderer* renderer, const std::string& textureName, const std::string& imgPath)
{
  SDL_Surface* surface = IMG_Load(imgPath.c_str());
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  textures.emplace(textureName, texture);

  SDL_FreeSurface(surface);

  Logger::Log("Created texture with name " + textureName);
}

void AssetStore::RemoveTexture(std::string& textureName)
{
  SDL_Texture* texture = GetTexture(textureName);
  textures.erase(textureName);

  SDL_DestroyTexture(texture);

  Logger::Log("Destroyed texture with name " + textureName);
}


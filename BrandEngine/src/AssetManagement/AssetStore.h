#pragma once

#include <SDL.h>
#include <map>
#include <string>

class AssetStore
{
  std::map<std::string, SDL_Texture*> textures;
  //std::map<std::string, SDL_*> audios;
  //std::map<std::string, SDL_*> fonts;

  public:
    AssetStore();
    ~AssetStore();
    SDL_Texture* GetTexture(std::string& textureName) { return textures[textureName]; }

    void Clear();
    void RemoveTexture(std::string& textureName);
    void CreateTexture(SDL_Renderer* renderer, const std::string& textureName, const std::string& fileLocation);
};

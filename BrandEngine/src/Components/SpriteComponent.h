#pragma once

#include <glm/glm.hpp>
#include <SDL.h>
#include "../AssetManagement/AssetStore.h"

struct SpriteComponent {
   int width;
   int height;
   int zIndex;
   SDL_Rect srcRect;
   std::string assetName;

   SpriteComponent(const std::string& assetName = "", int width = 0, int height = 0, int zIndex = 0, int srcRectX = 0, int srcRectY = 0):
    width(width), height(height), zIndex(zIndex), assetName(assetName) {
      this -> srcRect = {srcRectX, srcRectY, width, height};
   }
};

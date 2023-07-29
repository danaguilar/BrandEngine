#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <SDL.h>
#include "../AssetManagement/AssetStore.h"

struct SpriteComponent 
{
   int width;
   int height;
   SDL_Rect srcRect;
   std::string assetName;

   SpriteComponent(const std::string& assetName = "", int width = 0, int height = 0, int srcRectX = 0, int srcRectY = 0)
   {
      this -> width = width;
      this -> height = height;
      this -> assetName = assetName;
      this -> srcRect = {srcRectX, srcRectY, width, height};
   }
};

#endif // !SPRITECOMPONENT_H

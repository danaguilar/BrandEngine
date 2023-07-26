#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>

struct SpriteComponent 
{
   float width;
   float height;

   SpriteComponent(float width = 0.0, float height = 0.0)
   {
      this -> width = width;
      this -> height = height;
   }
};

#endif // !SPRITECOMPONENT_H

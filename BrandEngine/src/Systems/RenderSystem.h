#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>

class RenderSystem: public System
{
  public:
    RenderSystem()
    {
      RegisterComponent<TransformComponent>();
      RegisterComponent<SpriteComponent>();
    }

    void Render(SDL_Renderer* renderer)
    {
      for (auto entity : GetEntities())
      {
        // Create SDL rect
        SDL_Rect rect; 
        rect.x = entity.GetComponent<TransformComponent>().position.x;
        rect.y = entity.GetComponent<TransformComponent>().position.y;
        rect.w = entity.GetComponent<SpriteComponent>().width;
        rect.h = entity.GetComponent<SpriteComponent>().height;

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &rect);
      }
    }

};




#endif

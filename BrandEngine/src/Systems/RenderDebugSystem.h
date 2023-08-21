#pragma once

#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class RenderDebugSystem : public System {

  public: 
    RenderDebugSystem() {
      RegisterComponent<TransformComponent>();
      RegisterComponent<BoxColliderComponent>();
    }

    void Render(SDL_Renderer* renderer) {
      for (auto entity : GetEntities()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& collider = entity.GetComponent<BoxColliderComponent>();

        SDL_Rect rect = {
          transform.position.x,
          transform.position.y,
          collider.width,
          collider.height
        };
        if (collider.isColliding) {
          SDL_SetRenderDrawColor(renderer, 255 , 0, 0, SDL_ALPHA_OPAQUE);
        }
        else {
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

        }
        SDL_RenderDrawRect(renderer, &rect);
      }
    }
};

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManagement/AssetStore.h"
#include <SDL.h>
#include <algorithm>

class RenderSystem: public System {
  public:
    RenderSystem() {
      RegisterComponent<TransformComponent>();
      RegisterComponent<SpriteComponent>();
    }

    void Render(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
      // Sort entities by zIndex. It's perhaps a better idea to keep entities ordered by zIndex when they are created/destroyed, but this will work
      // decently well for now
      auto entities = GetEntities();
      std::sort(
        entities.begin(), entities.end(),
        [](auto entA, auto entB) {
          return entA.GetComponent<SpriteComponent>().zIndex < entB.GetComponent<SpriteComponent>().zIndex;
        }
      );

      for (auto entity : entities) {
        auto sprite = entity.GetComponent<SpriteComponent>();
        auto transform = entity.GetComponent<TransformComponent>();

        SDL_Rect srcRect = sprite.srcRect;
        SDL_Rect destRect = {
          static_cast<int>(transform.position.x),
          static_cast<int>(transform.position.y),
          static_cast<int>(sprite.width * transform.scale.x),
          static_cast<int>(sprite.height * transform.scale.y)
        };


        SDL_RenderCopyEx(
          renderer,
          assetStore->GetTexture(sprite.assetName),
          &srcRect,
          &destRect,
          transform.rotation,
          NULL,
          SDL_FLIP_NONE
        );
      }
    }
};




#endif

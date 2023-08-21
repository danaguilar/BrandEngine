#pragma once

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"

class AnimationSystem: public System {
  public: 
    AnimationSystem() {
      RegisterComponent<AnimationComponent>();
      RegisterComponent<SpriteComponent>();
    }

    void Update (double deltaTime) {
      for (auto entity : GetEntities()) {
        auto& animationComponent = entity.GetComponent<AnimationComponent>();
        auto& spriteComponent = entity.GetComponent<SpriteComponent>();

        animationComponent.timeSinceFrameChange = animationComponent.timeSinceFrameChange + (deltaTime);
        if (animationComponent.timeSinceFrameChange > (1 / animationComponent.framesPerSecond)) {
          animationComponent.currentFrame++;
          if (animationComponent.currentFrame >= animationComponent.numberOfFrames) {
            if(animationComponent.shouldLoop) animationComponent.currentFrame = 0;
            else animationComponent.currentFrame--;
          }
          animationComponent.timeSinceFrameChange = 0;
          spriteComponent.srcRect.x = animationComponent.currentFrame * spriteComponent.width;
        }
      }
    }
};

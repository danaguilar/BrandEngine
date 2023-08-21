#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"

class CollisionSystem : public System {

  public:
    CollisionSystem() {
      RegisterComponent<BoxColliderComponent>();
      RegisterComponent<TransformComponent>();
    }

    void Update(double deltaTime) {
      auto entities = GetEntities();

      for (auto entity : entities) {
        entity.GetComponent<BoxColliderComponent>().isColliding = false;
      }

      for (auto i = entities.begin(); i != entities.end(); i++) {
        Entity a = *i;
        auto& aTransform = a.GetComponent<TransformComponent>();
        auto& aCollider = a.GetComponent<BoxColliderComponent>();

        for (auto j = i + 1; j != entities.end(); j++) {
          Entity b = *j;

          auto& bTransform = b.GetComponent<TransformComponent>();
          auto& bCollider = b.GetComponent<BoxColliderComponent>();

          bool collisionDetected = GetAABBCollision(
            aTransform.position.x,
            aTransform.position.y,
            aCollider.width,
            aCollider.height,
            bTransform.position.x,
            bTransform.position.y,
            bCollider.width,
            bCollider.height
          );

          if (collisionDetected) {
            aCollider.isColliding = true;
            bCollider.isColliding = true;
          }
        }
      }
    }

    bool GetAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
      return (
        aX + aW >= bX &&
        aX <= bX + bW &&
        aY + aH >= bY &&
        aY <= bY + bH
      );
    }
};

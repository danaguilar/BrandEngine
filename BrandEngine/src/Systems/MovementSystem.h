#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"

class MovementSystem: public System
{
  public: 
    MovementSystem()
    {
      RegisterComponent<TransformComponent>();
      RegisterComponent<RigidBodyComponent>();
    }

    void Update (double deltaTime)
    {
      for (auto entity : GetEntities())
      {
        auto& transform = entity.GetComponent<TransformComponent>();
        auto rigidBody = entity.GetComponent<RigidBodyComponent>();

        transform.position.x += rigidBody.velocity.x * deltaTime;
        transform.position.y += rigidBody.velocity.y * deltaTime;

        Logger::Log("Entity " + std::to_string(entity.GetID()) + ": New position is (" + std::to_string(transform.position.x) + ", " + std::to_string(transform.position.y) + ")");
      }
    }
};

#endif // !MOVEMENTSYSTEM_H



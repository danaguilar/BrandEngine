#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include <memory>

class DamageSystem: public System {
  public:
    DamageSystem() {
      RegisterComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
       eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent& event) {
        Logger::Log("The damage system recieved and event collision between entities " + std::to_string(event.a.GetID()) + " and " + std::to_string(event.b.GetID()));
        event.a.Kill();
        event.b.Kill();
    }

};

#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include "../Events/KeyPressedEvent.h"
#include <memory>

class KeyboardMovementSystem : public System {
  public:
    KeyboardMovementSystem() {
      //RegisterComponent<TransformComponent>();
    }
    
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
      eventBus->SubscribeToEvent(this, &KeyboardMovementSystem::onKeyPress);
    }

    void onKeyPress(KeyPressedEvent& event) {
      Logger::Log("Keypress event has been called");
    }

};

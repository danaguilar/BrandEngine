#pragma once
#include "../EventBus/Event.h"

class KeyPressedEvent : public Event {
  public: 
    char keyPressed;
    KeyPressedEvent(char keyPressed): keyPressed(keyPressed) {}
};

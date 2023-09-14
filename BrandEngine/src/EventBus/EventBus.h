#pragma once

#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <memory>
#include <list>

class IEventCallback {
  private:
    virtual void Call(Event& e) = 0;

  public:
    virtual ~IEventCallback() = default;

    void Execute(Event& e) {
      Call(e);
    }

};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
  private:
    typedef void(TOwner::*CallbackFunction)(TEvent&);

    TOwner* ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event& e) override {
      std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
    }

  public:
    EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
      this->ownerInstance = ownerInstance;
      this->callbackFunction = callbackFunction;
    }
    
    virtual ~EventCallback() override = default;
};

typedef  std::list<std::unique_ptr<IEventCallback>> CallbackList;

class EventBus {
  private:
    std::map<std::type_index, std::unique_ptr<CallbackList>> subscribers;

  public:
    EventBus() {
      Logger::Log("Constructor called on Entity Bus");
    }

    ~EventBus() {
      Logger::Log("Destructor called on Entity Bus");
    }

    // clears the subscriber list
    void Reset() {
      subscribers.clear();
    }

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&)) {
      if (!subscribers[typeid(TEvent)]) {
        subscribers[typeid(TEvent)] = std::make_unique<CallbackList>();
      }
      auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
      subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }


    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ...args) {
      auto callbacks = subscribers[typeid(TEvent)].get();
      if (!!callbacks) {
        for (auto it = callbacks->begin(); it != callbacks->end(); it++) {
          auto callback = it->get();
          TEvent event(std::forward<TArgs>(args)...);
          callback->Execute(event);

        }
      }
    }
    
};

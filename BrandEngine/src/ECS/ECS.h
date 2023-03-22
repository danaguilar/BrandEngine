#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

unsigned int const MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
  static int nextId();
};

template <typename TComponent>
class Component: public IComponent
{
  static int GetId()
  {
    static auto componentId = nextId++();
    return componentId;
  }
};

class Entity
{
  private:
    int id;

  public:
    Entity(int id): id(id) {};
    int GetID() const;
};

class System
{
  private:
    Signature componentSignature;
    std::vector<Entity> entities;

  public:
    System() = default;
    ~System() = default;
    void AddEntity(Entity);
    void RemoveEntity(Entity);
    std::vector<Entity>& GetEntities() const;
    Signature& GetComponentSignature() const;
    template<typename TComponent> void RegisterComponent();
};

class Registry
{

};

template<typename TComponent>
void System::RegisterComponent()
{
  auto componentId = Component<TComponent>.GetID();
  componentSignature.set(componentId);
}

#endif

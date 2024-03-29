#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <deque>
#include <set>
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "../Logger/Logger.h"

unsigned int const MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
  protected:
    static int nextId;
};

template <typename TComponent>
class Component: public IComponent {
  public:
    static int GetID()
    {
      static auto id = nextId++;
      return id;
    }
};

class Entity {
  private:
    int id;

  public:
    class Registry* registry;

    Entity(int id): id(id) {};
    int GetID() const;
    void Kill();
    bool operator ==(const Entity& other) const { return id == other.GetID(); };
    bool operator !=(const Entity& other) const { return id != other.GetID(); };
    bool operator >(const Entity& other) const { return id > other.GetID(); };
    bool operator <(const Entity& other) const { return id < other.GetID(); };

    template<typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template<typename TComponent> void RemoveComponent();
    template<typename TComponent> bool HasComponent();
    template<typename TComponent> TComponent& GetComponent();

};


class System {
  private:
    Signature componentSignature;
    std::vector<Entity> entities;

  public:
    System() = default;
    ~System() = default;
    void AddEntity(Entity);
    void RemoveEntity(Entity);
    std::vector<Entity> GetEntities() const;
    const Signature& GetComponentSignature() const;
    template<typename TComponent> void RegisterComponent();
};

class IPool {
  protected:
    virtual ~IPool() {};
};

template<typename T>
class Pool : public IPool {
  private:
    std::vector<T> data;
  
  public:
    Pool(int n = 100) { Resize(n); };
    ~Pool() {};
    void Add(T object) { data.push_back(object); }
    void Clear() { data.clear(); }
    int size() { return data.size(); }
    void Resize(int n) { data.resize(n); }
    void Set(int index, T object) { data[index] = object; }
    T& Get(int index) { return static_cast<T&>(data[index]); }
    T& operator[](unsigned int index) { return data[index]; }
};

////////////////////////////////////////////////////////////////////////////////////////
// REGISTRY
////////////////////////////////////////////////////////////////////////////////////////
// Manages the creation and destruction of enitites and adds systems and components
////////////////////////////////////////////////////////////////////////////////////////

class Registry {
  private:
    int numEntities = 0;

    std::deque<int> freeEntityIds;

    // Stores components of the same type into pools. Each pool is ordered by the entity of that component
    // [componentPool index = component id]
    // [pool index = entity id]
    std::vector<std::shared_ptr<IPool>> componentPools;

    // Signature of each entity
    // [vector indes = entity id]
    std::vector<Signature> entityComponentSignatures;
      
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeRemoved;
    
  public:
    Registry() { 
      Logger::Log("Registry Constructor Called");
    }

    ~Registry() {
      Logger::Log("Registry Destructor Called");
    }

    void Update();
    
    // Entity Management
    Entity CreateEntity();
    void DeleteEntity(Entity entity);

    // Component Management
    template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    template<typename TComponent> void RemoveComponent(Entity entity);
    template<typename TComponent> bool HasComponent(Entity entity);
    template<typename TComponent> TComponent& GetComponent(Entity entity);


    // Systems Management
    template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
    template<typename TSystem> void RemoveSystem();
    template<typename TSystem> bool HasSystem() const;
    template<typename TSystem> TSystem& GetSystem() const;

    // Adding entities to all systems
    void AddEntityToSystems(Entity entity);
    void RemoveEntityFromSystems(Entity entity);
};

template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args)
{
  std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem()
{
  auto system = systems.find(std::type_index(typeid(TSystem)));
  systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const
{
  return systems.find(std::type_index(typeid(TSystem))) != systems.end;
}

template<typename TSystem>
TSystem& Registry::GetSystem() const
{
  auto system = systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
  // Create component and add it in the appropriate pool (using component ID) and the appropriate space in that pool (using entity ID)
  // Find and expand a space for a component pool
  const int componentId = Component<TComponent>::GetID();
  if (componentId >= componentPools.size())
  {
    componentPools.resize(componentId + 1, nullptr);
  }
  if (!componentPools[componentId])
  {
    componentPools[componentId] =  std::make_shared<Pool<TComponent>>();
  }
  std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

  // Find and expand a space for an entity in a pool
  const int entityId = entity.GetID();
  if (entityId >= componentPool->size())
  {
    componentPool->Resize(numEntities);
  }

  // Create component
  TComponent newComponent(std::forward<TArgs>(args)...);
  // Add component with entity to the appropriate pool
  (*componentPool)[entityId] = newComponent;
  // Add and change entity signature
  entityComponentSignatures[entityId].set(componentId);

  Logger::Log("Component " + std::to_string(componentId) + " was added to entity " + std::to_string(entityId));
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity)
{
  const int componentId = Component<TComponent>::GetID();
  const int entityId = entity.GetID();
  return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
  const int componentId = Component<TComponent>::GetID();
  const int entityId = entity.GetID();

  entityComponentSignatures[entityId].reset(componentId);

  Logger::Log("Component " + std::to_string(componentId) + " was removed from entity " + std::to_string(entityId));
}

template<typename TComponent>
TComponent& Registry::GetComponent(Entity entity)
{
  const int componentId = Component<TComponent>::GetID();
  const int entityId = entity.GetID();

  auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
  return componentPool -> Get(entityId);
}

template<typename TComponent>
void System::RegisterComponent()
{
  auto componentId = Component<TComponent>::GetID();
  componentSignature.set(componentId);
}

template<typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs && ...args)
{
  registry -> AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::RemoveComponent()
{
  registry -> RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent()
{
  return registry -> HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent()
{
  return registry -> GetComponent<TComponent>(*this);
}

#endif


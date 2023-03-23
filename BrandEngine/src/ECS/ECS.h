#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
    bool operator ==(const Entity& other) const { return id == other.GetID(); };
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
    std::vector<Entity> GetEntities() const;
    const Signature& GetComponentSignature() const;
    template<typename TComponent> void RegisterComponent();
};

class IPool
{
  virtual ~IPool();
};

template<typename T>
class Pool : public IPool
{
  private:
    std::vector<T> data;
  
  public:
    Pool(int n = 100) { Resize(n); };
    ~Pool();
    void Add(T object) { data.push_back(object); }
    void Clear() { data.clear(); }
    void Resize(int n) { data.resize(n); }
    void Set(int index, T object) { data[index] = object; }
    T& Get(int index) { return static_cast<T&>(data[index]); }
    T& operator [](unsigned int index) { return data[index]; }
};

////////////////////////////////////////////////////////////////////////////////////////
// REGISTRY
////////////////////////////////////////////////////////////////////////////////////////
// Manages the creation and destruction of enitites and adds systems and components
////////////////////////////////////////////////////////////////////////////////////////
class Registry
{
  private:
    int numEntities = 0;

    // Stores components of the same type into pools. Each pool is ordered by the entity of that component
    // componentPool[componentID]
    // pool[entityID]
    std::vector<IPool*> componentPools;

    // Signature of each entity
    // [entityID]
    std::vector<Signature> entityComponentSignatures;
      
    std::unordered_map<std::type_index, System*> systems;

    

};

template<typename TComponent>
void System::RegisterComponent()
{
  auto componentId = Component<TComponent>.GetID();
  componentSignature.set(componentId);
}

#endif

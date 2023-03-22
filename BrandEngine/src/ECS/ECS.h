#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

unsigned int const MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

class Component
{

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
};

class Registry
{

};

#endif

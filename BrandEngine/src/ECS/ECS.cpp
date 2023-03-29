#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>

int IComponent::nextId = 0;

int Entity::GetID() const
{
  return id;
}

void System::AddEntity(Entity entity)
{
  entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
  entities.erase(
    std::remove_if(
      entities.begin(), entities.end(),
      [&entity](Entity other) { return entity == other; }
    ),
    entities.end()
  );
}

std::vector<Entity> System::GetEntities() const
{
  return entities;
}

const Signature& System::GetComponentSignature() const
{
  return componentSignature;
}

Entity Registry::CreateEntity()
{
  int entityID = numEntities++;
  if (entityID > entityComponentSignatures.size())
  {
    entityComponentSignatures.resize(entityID+1);
  }
  
  Entity entity(entityID);
  
  entitiesToBeAdded.insert(entity);

  Logger::Log("Entity Created with id = " + std::to_string(entityID));
  return entity;
}

void Registry::AddEntityToSystems(Entity entity)
{
  const auto entityId = entity.GetID();

  for (auto& system : systems) {
    const auto entitySig = entityComponentSignatures[entityId];
    const auto& systemSig = system.second -> GetComponentSignature();
    
    bool isInterested = (entitySig & systemSig) == systemSig;
    if (isInterested) {
      system.second -> AddEntity(entity);
    }
  }
}

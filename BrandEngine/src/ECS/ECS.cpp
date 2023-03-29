#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>

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

#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>
#include <algorithm>

int IComponent::nextId = 0;

////////////////// ENTITY DEFINITIONS ///////////////////////////
int Entity::GetID() const {
  return id;
}

void Entity::Kill() {
  registry->DeleteEntity(*this);
}

////////////////// SYSTEM DEFINITIONS ///////////////////////////
void System::AddEntity(Entity entity) {
  entities.push_back(entity);
}

void System::RemoveEntity(Entity entity) {
  entities.erase(
    std::remove_if(
      entities.begin(), entities.end(),
      [&entity](Entity other) { return entity == other; }
    ),
    entities.end()
  );
}

std::vector<Entity> System::GetEntities() const {
  return entities;
}

const Signature& System::GetComponentSignature() const {
  return componentSignature;
}

////////////////// REGISTRY DEFINITIONS ///////////////////////////
Entity Registry::CreateEntity() {
  int entityID;

  if (freeEntityIds.empty()) {
    entityID = numEntities++;
    if (entityID >= entityComponentSignatures.size()) {
      entityComponentSignatures.resize(entityID+1);
    }
  }

  else {
    entityID = freeEntityIds.front();
    freeEntityIds.pop_front();
  }
  
  Entity entity(entityID);
  entity.registry = this;
  entitiesToBeAdded.insert(entity);

  Logger::Log("Entity Created with id = " + std::to_string(entityID));
  return entity;
}

void Registry::DeleteEntity(Entity entity) {
  entitiesToBeRemoved.insert(entity);
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

void Registry::RemoveEntityFromSystems(Entity entity) {
  for (auto& system : systems) {
    system.second -> RemoveEntity(entity);
  }
}

void Registry::Update() {
  for (auto entity : entitiesToBeAdded) {
    AddEntityToSystems(entity);
  }
  entitiesToBeAdded.clear();

  for (auto entity : entitiesToBeRemoved) {
    RemoveEntityFromSystems(entity);
    entityComponentSignatures[entity.GetID()].reset();
    freeEntityIds.push_back(entity.GetID());
  }

  entitiesToBeRemoved.clear();
}

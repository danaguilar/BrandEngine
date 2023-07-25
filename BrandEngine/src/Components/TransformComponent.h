#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent
{
  glm::vec2 position;
  glm::vec2 rotation;
  double scale;

  TransformComponent(glm::vec2 position = glm::vec2(0.0,0.0), glm::vec2 rotation = glm::vec2(0.0,0.0), double scale = 1.0) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
  }
};

#endif

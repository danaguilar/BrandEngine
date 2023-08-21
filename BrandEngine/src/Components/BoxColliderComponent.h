#pragma once


struct BoxColliderComponent {
  int height;
  int width;
  int offsetX;
  int offsetY;
  bool isColliding;

  BoxColliderComponent(int height = 0, int width = 0, int offsetX = 0, int offsetY = 0, bool isColliding = false): height(height), width(width), offsetX(offsetX), offsetY(offsetY), isColliding(isColliding) {}
};

#pragma once

struct AnimationComponent {
  int numberOfFrames;
  int currentFrame;
  float framesPerSecond;
  bool shouldLoop;
  float timeSinceFrameChange;

  AnimationComponent(int numberOfFrames = 1, int currentFrame = 0, float framesPerSecond = 1, bool shouldLoop = false) {
    this -> numberOfFrames = numberOfFrames;
    this -> currentFrame = currentFrame;
    this -> framesPerSecond = framesPerSecond;
    this -> shouldLoop = shouldLoop;
    this -> timeSinceFrameChange = 0;
  }
};

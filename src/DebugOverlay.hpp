#pragma once

#include <SDL3/SDL.h>

class DebugPanel {
public:
  DebugPanel();
  ~DebugPanel();

  void ToggleDebugPanel();

  void ProcessDebugPanel();

  void RenderDebugPanel(SDL_Renderer *renderer);

private:
  /* ----- init ----- */
  bool enabled = false;

  /* ----- fps ----- */
  Uint64 lastTime = 0;
  float deltaTime = 0;
  float fps = 0.0f;
};

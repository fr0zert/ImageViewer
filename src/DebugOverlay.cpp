#include "DebugOverlay.hpp"

DebugPanel::DebugPanel() {}

DebugPanel::~DebugPanel() {}

void DebugPanel::ToggleDebugPanel() { enabled = !enabled; }

void DebugPanel::ProcessDebugPanel() {
  if (!enabled)
    return;

  Uint64 now = SDL_GetPerformanceCounter();
  Uint64 freq = SDL_GetPerformanceFrequency();
  // Guard against divide-by-zero, if delta is zero, leave fps unchanged.
  float delta = (float)(now - lastTime) / (float)freq;
  lastTime = now;
  if (delta > 0.0f) {
    deltaTime = delta;
    fps = 1.0f / deltaTime;
  }
}

void DebugPanel::RenderDebugPanel(SDL_Renderer *renderer) {
  if (!enabled)
    return;

  if (!renderer)
    return;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDebugTextFormat(renderer, 5, 5, "%.1f fps", fps); /* fps */
}

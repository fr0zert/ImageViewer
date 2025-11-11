#include "DebugPanel.h"
#include "AppState.h"
#include <SDL3/SDL.h>

namespace DebugPanel {

void InitDebugPanel(DebugPanelState &state) {
  state.debugMode = false;
  state.lastTime = SDL_GetPerformanceCounter();
  state.fps = 0.0f;
  state.deltaTime = 0.0f;
}

void ToggleDebugPanel(DebugPanelState &state) { state.debugMode = !state.debugMode; }

void ToggleDebugPanel(void *appstate) {
  if (!appstate)
    return;
  ImageViewerState *as = static_cast<ImageViewerState *>(appstate);
  // Toggle the embedded debug panel in the typed app state
  ToggleDebugPanel(as->debugPanel);
}

void RenderDebugPanel(SDL_Renderer *renderer, const DebugPanelState &state) {
  if (!renderer)
    return;
  if (!state.debugMode)
    return;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  // --- FPS ---
  SDL_RenderDebugTextFormat(renderer, 5, 5, "%.1f fps", state.fps);
}

void ProcessDebug(DebugPanelState &state) {
  if (!state.debugMode)
    return;
  Uint64 now = SDL_GetPerformanceCounter();
  Uint64 freq = SDL_GetPerformanceFrequency();
  // Guard against divide-by-zero; if delta is zero, leave fps unchanged.
  float delta = (float)(now - state.lastTime) / (float)freq;
  state.lastTime = now;
  if (delta > 0.0f) {
    state.deltaTime = delta;
    state.fps = 1.0f / state.deltaTime;
  }
}

} // namespace DebugPanel

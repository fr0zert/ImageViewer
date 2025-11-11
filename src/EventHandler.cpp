#include "EventHandler.h"
#include "DebugPanel.h"
#include <SDL3/SDL.h>

namespace HandleEvents {

SDL_AppResult HandleKeyEvents(SDL_Scancode key_code, ImageViewerState &state) {
  switch (key_code) {
  case SDL_SCANCODE_ESCAPE:
  case SDL_SCANCODE_Q:
    return SDL_APP_SUCCESS;
  case SDL_SCANCODE_D:
    DebugPanel::ToggleDebugPanel(state.debugPanel);
    break;
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

} // namespace HandleEvents

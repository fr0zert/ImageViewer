#pragma once

#include "AppState.h"
#include <SDL3/SDL.h>

namespace HandleEvents {

/*
 * HandleKeyEvents
 *
 * Process a key-down event. `state` is the application's typed state
 * (`ImageViewerState &`). The handler may access sub-fields (for example the
 * debug panel state) directly without unsafe casts.
 *
 * Returns an SDL_AppResult indicating whether the app should continue, quit,
 * or encountered a failure.
 */
SDL_AppResult HandleKeyEvents(SDL_Scancode key_code, ImageViewerState &state);

} // namespace HandleEvents

#pragma once

#include "DebugPanel.h"
#include <SDL3/SDL.h>

/*
 * ImageViewerState
 *
 * Central, strongly-typed application state. Prefer embedding small POD
 * subsystems (like DebugPanelState) by value here so initialization and
 * cleanup are automatic and there is no manual heap allocation responsibility.
 *
 * Ownership/semantics:
 *  - `window` and `renderer` are created by SDL and must be destroyed by the
 *    owner (typically in SDL_AppQuit).
 *  - `debugPanel` is embedded by value and requires no manual free.
 *  - `image_texture` is created via SDL_CreateTexture and must be destroyed by
 *    the owner (SDL_DestroyTexture) when clearing the app state.
 */
struct ImageViewerState {
  SDL_Window *window = nullptr;     // SDL window (nullable)
  SDL_Renderer *renderer = nullptr; // SDL renderer (nullable)

  // Embedded debug panel state — no manual allocation/free required.
  DebugPanel::DebugPanelState debugPanel{};

  // Texture for a loaded image (created via ImageHandler). Caller owns it and
  // must call SDL_DestroyTexture when replacing/freeing.
  SDL_Texture *image_texture = nullptr;

  // Dimensions of the loaded image (pixels)
  int image_width = 0;
  int image_height = 0;

  // Position on screen where the image will be rendered (top-left corner)
  int image_x = 0;
  int image_y = 0;

  // Convenience helper: check if fully initialized (basic)
  bool is_valid() const { return window != nullptr && renderer != nullptr; }
};

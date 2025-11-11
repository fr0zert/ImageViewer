#include <SDL3/SDL_render.h>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "AppState.h"
#include "DebugPanel.h"
#include "EventHandler.h"
#include "ImageHandler.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <sail-c++/sail-c++.h>

// `ImageViewerState` is defined in `AppState.h` and embeds `DebugPanelState` by value.

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  // Init AppMetaData
  // need expand later for all OSs
  // SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

  // Init AppState
  ImageViewerState *state = static_cast<ImageViewerState *>(SDL_calloc(1, sizeof(ImageViewerState)));
  if (!state) {
    SDL_Log("Couldn't initialize SDL appstate");
    return SDL_APP_FAILURE;
  }
  *appstate = state;

  // INIT SDL
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    // free allocated state to avoid leak
    SDL_free(state);
    *appstate = nullptr;
    return SDL_APP_FAILURE;
  }

  // INIT Window and Renderer
  if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 0, 0,
                                   SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS | SDL_WINDOW_TRANSPARENT |
                                       SDL_WINDOW_MAXIMIZED,
                                   &state->window, &state->renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    SDL_Quit();
    SDL_free(state);
    *appstate = nullptr;
    return SDL_APP_FAILURE;
  }
  // ASPECT RATIO STUFF (for later maybe)
  // SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_DISABLED);

  // VSYNC
  SDL_SetRenderVSync(state->renderer, 1);
  // Init debug panel
  DebugPanel::InitDebugPanel(state->debugPanel);

  // If user passed a file path as the first argv, load it now and center it.
  if (argc > 1 && argv[1] != nullptr) {
    if (!ImageHandler::LoadTextureFromFile(state->renderer, std::string(argv[1]), state->image_texture,
                                           &state->image_width, &state->image_height)) {
      SDL_Log("Failed to load image from path: %s", argv[1]);
      // continue without an image
    } else {
      // Center the image in the window
      int win_w = 0, win_h = 0;
      SDL_GetWindowSize(state->window, &win_w, &win_h);
      state->image_x = (win_w - state->image_width) / 2;
      state->image_y = (win_h - state->image_height) / 2;
    }
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  ImageViewerState *state = static_cast<ImageViewerState *>(appstate);
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  if (event->type == SDL_EVENT_KEY_DOWN) {
    return HandleEvents::HandleKeyEvents(event->key.scancode, *state);
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  // ----- AppState -----
  ImageViewerState *state = static_cast<ImageViewerState *>(appstate);

  // ----- LOGIC -----
  DebugPanel::ProcessDebug(state->debugPanel);

  // ----- START (RENDER) -----
  SDL_SetRenderDrawColorFloat(state->renderer, 0, 0, 0, 0.5f);
  SDL_RenderClear(state->renderer);

  // Render loaded image (if any) centered at image_x/image_y (use float rect for SDL3)
  if (state->image_texture) {
    SDL_FRect dstf;
    dstf.x = static_cast<float>(state->image_x);
    dstf.y = static_cast<float>(state->image_y);
    dstf.w = static_cast<float>(state->image_width);
    dstf.h = static_cast<float>(state->image_height);
    SDL_RenderTexture(state->renderer, state->image_texture, nullptr, &dstf);
  }

  // render debug panel on top
  DebugPanel::RenderDebugPanel(state->renderer, state->debugPanel);

  // ----- END (RENDER) -----
  SDL_RenderPresent(state->renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

  // If no app state was provided, still ensure SDL subsystems are shut down.
  if (!appstate) {
    SDL_Quit();
    return;
  }

  ImageViewerState *state = static_cast<ImageViewerState *>(appstate);
  if (state->renderer) {
    SDL_DestroyRenderer(state->renderer);
    state->renderer = nullptr;
  }

  if (state->window) {
    SDL_DestroyWindow(state->window);
    state->window = nullptr;
  }

  // DebugPanel is embedded by value in ImageViewerState; no manual free required.
  SDL_free(appstate);

  // Quit SDL subsystems last
  SDL_Quit();
}

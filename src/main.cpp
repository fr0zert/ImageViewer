#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "DebugOverlay.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <memory>

// TODO: move out of main file
struct AppState {
  std::unique_ptr<Window> window;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<DebugPanel> debug_panel;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  // TODO: Init AppMetadata
  // TODO: Expand AppMetadata
  // SDL_SetAppMetadata("Example", "1.0", "com.example.example-app");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto state = std::make_unique<AppState>();
  state->window = std::make_unique<Window>();
  state->renderer = std::make_unique<Renderer>(state->window->GetWindow());
  state->debug_panel = std::make_unique<DebugPanel>();

  SDL_SetRenderVSync(state->renderer->GetRenderer(), 1); // enable VSync

  *appstate = state.release();

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT)
    return SDL_APP_SUCCESS; /* exit */

  AppState *state = static_cast<AppState *>(appstate);

  switch (event->type) {
  case SDL_EVENT_KEY_DOWN:
    if (event->key.repeat) /* dont run in loop till key unpressed */
      break;
    if (event->key.scancode == SDL_SCANCODE_D)
      state->debug_panel->ToggleDebugPanel();
    break;
  default:
    break;
  }

  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *state = static_cast<AppState *>(appstate);
  /* debug_panel logic */
  state->debug_panel->ProcessDebugPanel();

  /* logic */

  /* ----- background ----- */
  SDL_SetRenderDrawColorFloat(state->renderer->GetRenderer(), 0, 0, 0, 0.5f);
  SDL_RenderClear(state->renderer->GetRenderer());

  /*  paint  */

  /* render debug_panel */
  state->debug_panel->RenderDebugPanel(state->renderer->GetRenderer());

  /* put new frame of screen */
  SDL_RenderPresent(state->renderer->GetRenderer());

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  std::unique_ptr<AppState> state(static_cast<AppState *>(appstate));
  if (!state)
    return;
}

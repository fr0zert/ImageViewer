#pragma once

#include <SDL3/SDL.h>

namespace DebugPanel {

/*
 * DebugPanelState
 *
 * Runtime state for the debug overlay (FPS, timing, enabled flag).
 * This is a small POD-type that can be embedded by value into a larger
 * application state to avoid manual heap allocation and unclear ownership.
 */
struct DebugPanelState {
  bool debugMode = false;
  Uint64 lastTime = 0;
  float deltaTime = 0.0f;
  float fps = 0.0f;
};

/*
 * ToggleDebugPanel
 *
 * Toggle the `debugMode` flag on a DebugPanelState instance.
 * Prefer the reference overload as it expresses clear ownership: the caller
 * supplies a concrete DebugPanelState (typically a member of the app state).
 */
void ToggleDebugPanel(DebugPanelState &state);

/*
 * ToggleDebugPanel (appstate overload - legacy)
 *
 * Backwards-compatible helper that accepts an opaque app state pointer.
 * This is provided for short-term compatibility with code that still passes
 * `void* appstate`. Prefer the typed-API above. Implementations may cast the
 * appstate to a struct that contains a `DebugPanelState` member and toggle it.
 */
void ToggleDebugPanel(void *appstate);

/*
 * InitDebugPanel
 *
 * Initialize an existing DebugPanelState instance. This function does not
 * allocate memory; it simply sets sensible initial values for the provided
 * reference.
 */
void InitDebugPanel(DebugPanelState &state);

/*
 * RenderDebugPanel
 *
 * Render the debug overlay (if enabled) using the provided renderer.
 * The DebugPanelState is passed as a const reference since rendering should
 * not mutate state.
 */
void RenderDebugPanel(SDL_Renderer *renderer, const DebugPanelState &state);

/*
 * ProcessDebug
 *
 * Per-frame logic for the debug overlay (calculating delta time and fps).
 * Mutates the provided DebugPanelState.
 */
void ProcessDebug(DebugPanelState &state);

} // namespace DebugPanel

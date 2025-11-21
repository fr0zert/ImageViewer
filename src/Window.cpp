#include "Window.hpp"

Window::Window() : window_title("Test") { // TODO: Change to normal title name
  window =
      SDL_CreateWindow(window_title, 0, 0, // 0 0 since we gonna maximize window
                       SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_MAXIMIZED);

  if (!window) {
    SDL_Log("Couldn't create window: %s", SDL_GetError());
    SDL_Quit();
  }
}

Window::~Window() {
  if (window && SDL_WasInit(SDL_INIT_VIDEO))
    SDL_DestroyWindow(window);
}

SDL_Window *Window::GetWindow() const { return window; }

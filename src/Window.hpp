#pragma once

#include <SDL3/SDL.h>

class Window {
public:
  Window();
  ~Window();

  SDL_Window *GetWindow() const;

private:
  SDL_Window *window = nullptr;
  const char *window_title = "";
};

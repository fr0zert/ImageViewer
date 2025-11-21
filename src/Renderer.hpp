#pragma once

#include <SDL3/SDL.h>

class Renderer {
public:
  Renderer(SDL_Window *window);
  ~Renderer();

  SDL_Renderer *GetRenderer() const;

private:
  SDL_Renderer *renderer = nullptr;
};

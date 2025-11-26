#include "Renderer.hpp"

Renderer::Renderer(SDL_Window *window) {
  renderer = SDL_CreateRenderer(window, NULL);

  if (!renderer) {
    SDL_Log("Couldn't create renderer: %s", SDL_GetError());
    SDL_Quit();
  }
}

Renderer::~Renderer() {
  if (renderer)
    SDL_DestroyRenderer(renderer);
}

SDL_Renderer *Renderer::GetRenderer() const { return renderer; }

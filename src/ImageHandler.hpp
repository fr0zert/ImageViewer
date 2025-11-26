#pragma once
#include <SDL3/SDL.h>
#include <sail-c++/sail-c++.h>
class ImageHandler {
public:
  ImageHandler(SDL_Renderer *renderer);
  ~ImageHandler();

  bool LoadImage(const std::string &path);
  void UnloadImage();

  void DrawImage();
  void CenterImage();
  void ScaleImage(float scale);

private:
  SDL_Renderer *renderer;
  SDL_Texture *image;
  SDL_FRect image_convas;

  bool LoadImageFileWithSail(const std::string &path);
};

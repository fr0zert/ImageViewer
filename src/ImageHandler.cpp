#include "ImageHandler.hpp"
#include <SDL3/SDL.h>

ImageHandler::ImageHandler(SDL_Renderer *renderer) : renderer(renderer), image(nullptr) { image_convas = {0, 0, 0, 0}; }

ImageHandler::~ImageHandler() { UnloadImage(); }

void ImageHandler::UnloadImage() {
  if (image) {
    SDL_DestroyTexture(image);
    image = nullptr;
  }
}

bool ImageHandler::LoadImage(const std::string &path) {
  UnloadImage();
  return LoadImageFileWithSail(path);
}

bool ImageHandler::LoadImageFileWithSail(const std::string &path) {
  sail::image simage(path);
  SDL_PixelFormat sdl_pixel_format;

  if (!simage.is_valid()) {
    SDL_Log("IV: ImageHandler::LoadImageFileWithSail: cant load image by path");
    return false;
  }

  switch (simage.pixel_format()) {
  case SAIL_PIXEL_FORMAT_BPP24_RGB:
    sdl_pixel_format = SDL_PIXELFORMAT_RGB24;
    break;

  case SAIL_PIXEL_FORMAT_BPP32_RGBA:
    sdl_pixel_format = SDL_PIXELFORMAT_RGBA32;
    break;

  default:
    if ((simage.can_convert(SAIL_PIXEL_FORMAT_BPP32_RGBA)) == SAIL_OK) {
      simage.convert_to(SAIL_PIXEL_FORMAT_BPP32_RGBA);
      sdl_pixel_format = SDL_PIXELFORMAT_RGBA32;
    } else {
      SDL_Log("IV: ImageHandler::LoadImageFileWithSail: cant convert image pixel format");
      return false;
    }
    break;
  }

  image = SDL_CreateTexture(renderer, sdl_pixel_format, SDL_TEXTUREACCESS_STATIC, simage.width(), simage.height());

  if (!image) {
    SDL_Log("IV: ImageHandler::LoadImageFileWithSail: cant create texture from sail image data");
    return false;
  }

  SDL_UpdateTexture(image, nullptr, simage.pixels(), simage.bytes_per_line());

  image_convas.w = static_cast<float>(simage.width());
  image_convas.h = static_cast<float>(simage.height());

  return true;
}

void ImageHandler::DrawImage() {
  if (image) {
    SDL_RenderTexture(renderer, image, nullptr, &image_convas);
  }
  // TODO: display (nothing loaded or smth)
}

void ImageHandler::CenterImage() {
  int rw, rh;
  SDL_GetCurrentRenderOutputSize(renderer, &rw, &rh);
  image_convas.x = (rw - image_convas.w) / 2.0f;
  image_convas.y = (rh - image_convas.h) / 2.0f;
}

void ImageHandler::ScaleImage(float scale) {
  int rw, rh;
  SDL_GetCurrentRenderOutputSize(renderer, &rw, &rh);

  float targetW = rw * scale;
  float targetH = rh * scale;

  float imgRatio = image_convas.w / image_convas.h;
  float targetRatio = targetW / targetH;

  if (imgRatio > targetRatio) {
    // Image is wider → match width
    image_convas.w = targetW;
    image_convas.h = targetW / imgRatio;
  } else {
    // Image is taller → match height
    image_convas.h = targetH;
    image_convas.w = targetH * imgRatio;
  }
}

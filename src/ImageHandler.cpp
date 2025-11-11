#include "ImageHandler.h"

#include <SDL3/SDL.h>
#include <sail-c++/image.h>
#include <sail-common/status.h>

namespace ImageHandler {

// TODO: this pixel format is only for WINDOWS
// other platforms may use other PIXEL_FORMAT
// should be SDL_PIXELFORMAT_RGBA8888 but cant test it now
static const SDL_PixelFormat kSDLPreferredFormat = SDL_PIXELFORMAT_ABGR8888;

// Helper: convert sail::image to a pixel format SDL can consume (RGBA32)
static bool ensure_rgba32(sail::image &img) {
  // If already in desired pixel format, nothing to do.
  if (img.pixel_format() == SAIL_PIXEL_FORMAT_BPP32_RGBA) {
    return true;
  }

  // If conversion is possible, do it.
  if (img.can_convert(SAIL_PIXEL_FORMAT_BPP32_RGBA)) {
    sail_status_t st = img.convert(SAIL_PIXEL_FORMAT_BPP32_RGBA);
    if (st != SAIL_OK) {
      SDL_Log("Sail: failed to convert image to RGBA32 (status=%d)", st);
      return false;
    }
    return true;
  }

  SDL_Log("Sail: image cannot be converted to RGBA32");
  return false;
}

bool LoadSailImage(const std::string &filepath, sail::image &out_img) {
  try {
    // Constructing with path will read a single frame; use load() to get status easily.
    sail_status_t st = out_img.load(filepath);
    if (st != SAIL_OK) {
      SDL_Log("Sail: failed to load image '%s' (status=%d)", filepath.c_str(), st);
      return false;
    }
  } catch (...) {
    SDL_Log("Sail: exception while loading image '%s'", filepath.c_str());
    return false;
  }

  return true;
}

bool LoadTextureFromFile(SDL_Renderer *renderer, const std::string &filepath, SDL_Texture *&out_texture, int *out_w,
                         int *out_h) {
  if (!renderer) {
    SDL_Log("LoadTextureFromFile: renderer is null");
    return false;
  }

  // Initialize out_texture to nullptr for clarity on failure.
  out_texture = nullptr;

  sail::image img;
  if (!LoadSailImage(filepath, img)) {
    // LoadSailImage logs details.
    return false;
  }

  // Ensure image is in a 32-bit RGBA layout that SDL can easily consume.
  if (!ensure_rgba32(img)) {
    SDL_Log("LoadTextureFromFile: cannot convert image to RGBA32: %s", filepath.c_str());
    return false;
  }

  // Retrieve image properties
  unsigned width_u = img.width();
  unsigned height_u = img.height();
  unsigned bytes_per_line_u = img.bytes_per_line();
  const void *pixels = img.pixels();

  if (!pixels || width_u == 0 || height_u == 0) {
    SDL_Log("LoadTextureFromFile: invalid image data for '%s'", filepath.c_str());
    return false;
  }

  // SDL expects int values
  int width = static_cast<int>(width_u);
  int height = static_cast<int>(height_u);
  int pitch = static_cast<int>(bytes_per_line_u);

  // Create an SDL texture compatible with the renderer.
  // Use streaming/static texture as appropriate; we use static since Sail provides the full buffer.
  SDL_Texture *tex = SDL_CreateTexture(renderer, kSDLPreferredFormat, SDL_TEXTUREACCESS_STATIC, width, height);
  if (!tex) {
    SDL_Log("SDL: Failed to create texture for '%s': %s", filepath.c_str(), SDL_GetError());
    return false;
  }

  // Update the texture with pixel data from Sail image.
  // SDL_UpdateTexture returns true on success and false on failure.
  if (!SDL_UpdateTexture(tex, nullptr, pixels, pitch)) {
    SDL_Log("SDL: Failed to update texture for '%s': %s", filepath.c_str(), SDL_GetError());
    SDL_DestroyTexture(tex);
    return false;
  }

  // Enable blending so alpha from images is respected when rendering
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

  // Return results
  out_texture = tex;
  if (out_w)
    *out_w = width;
  if (out_h)
    *out_h = height;

  return true;
}

} // namespace ImageHandler

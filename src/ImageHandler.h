#pragma once

#include <SDL3/SDL.h>

#include <sail-c++/image.h>

namespace ImageHandler {

/*
 * LoadTextureFromFile
 *
 * Loads an image from disk using the Sail library and creates an SDL_Texture
 * for immediate rendering with an SDL_Renderer.
 *
 * Parameters:
 *  - renderer : the SDL_Renderer to create the texture for (must be non-null)
 *  - filepath : path to the image file to load (PNG, JPEG, etc.)
 *  - out_texture : output parameter; on success this will be set to a valid
 *                  SDL_Texture* which the caller owns and must destroy with
 *                  SDL_DestroyTexture when finished. On failure it is left
 *                  unchanged (or set to nullptr).
 *  - out_w, out_h : optional output parameters for the pixel width/height.
 *
 * Returns:
 *  - true on success (texture created and out_w/out_h set if provided)
 *  - false on failure (logs via SDL_Log are emitted by the implementation)
 *
 * Notes:
 *  - This function uses Sail to decode and, if needed, convert the decoded
 *    image pixels into an SDL-friendly pixel layout suitable for creating an
 *    SDL_Texture (e.g. 32-bit RGBA). The implementation ensures the created
 *    texture has the expected pixel format for the renderer.
 *  - The function does not manage the lifetime of the renderer; the caller
 *    must ensure the renderer remains valid while the texture is used.
 */
bool LoadTextureFromFile(SDL_Renderer *renderer, const std::string &filepath, SDL_Texture *&out_texture,
                         int *out_w = nullptr, int *out_h = nullptr);

/*
 * Optional: load raw pixels into a sail::image and return it to the caller.
 * Useful if you want to do additional processing with Sail before converting to
 * an SDL_Texture.
 *
 * Returns true on success and sets 'img' to a valid sail::image containing the
 * decoded pixels (caller receives ownership of the sail::image object).
 */
bool LoadSailImage(const std::string &filepath, sail::image &img);

} // namespace ImageHandler

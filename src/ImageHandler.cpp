#include "ImageHandler.hpp"
#include <FreeImage.h>
#include <algorithm>

Texture2D LoadTextureFreeImage(const char *imagePath) {
  // Get the image format
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagePath, 0);
  // If detection fails, try guessing by the file extension.
  if (format == FIF_UNKNOWN) {
    format = FreeImage_GetFIFFromFilename(imagePath);
  }
  // If still unknown, or the format doesn’t support reading, stop here.
  if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format)) {
    TraceLog(LOG_ERROR, "Unsupported or unreadable image format: %s", imagePath);
    return {0}; // invalid texture
  }

  // Load the image
  FIBITMAP *dib = FreeImage_Load(format, imagePath);
  if (!dib) {
    TraceLog(LOG_ERROR, "Failed to load image via FreeImage: %s", imagePath);
    return {0};
  }
  // Convert to 32-bit RGBA
  FIBITMAP *converted = FreeImage_ConvertTo32Bits(dib);
  FreeImage_Unload(dib);
  dib = converted;

  // Flip vertically to match OpenGL texture coordinates, bruh FreeImage why
  FreeImage_FlipVertical(dib);

  int width = FreeImage_GetWidth(dib);
  int height = FreeImage_GetHeight(dib);
  BYTE *bits = FreeImage_GetBits(dib);

  // FreeImage stores BGRA, Raylib expects RGBA
  for (int i = 0; i < width * height; i++) {
    BYTE *pixel = bits + i * 4;
    std::swap(pixel[0], pixel[2]); // swap B and R
  }

  Image image = {
      .data = bits,
      .width = width,
      .height = height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

  Texture2D texture = LoadTextureFromImage(image);

  // Now safe to free FreeImage data
  FreeImage_Unload(dib);

  return texture;
}

ImageHandler::ImageHandler(const char *imagePath, int screenWidth, int screenHeight, float initialZoom) {
  this->screenWidth = screenWidth;
  this->screenHeight = screenHeight;
  // this->texture = LoadTexture(imagePath); raylib
  this->texture = LoadTextureFreeImage(imagePath);
  this->initialZoom = initialZoom;

  // checks if the image is loaded
  if (texture.id) isValidImage = true;
  // for future: PNGs dont need to be blurry
  SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

  float baseScaleX = (float)screenWidth / texture.width;
  float baseScaleY = (float)screenHeight / texture.height;
  baseScale = (baseScaleX < baseScaleY) ? baseScaleX : baseScaleY;

  currentScale = baseScale * initialZoom;
  targetScale = currentScale;
}

ImageHandler::~ImageHandler() { UnloadTexture(texture); }

void ImageHandler::HandleZoom(float wheel) {
  if (wheel != 0.0f) {
    targetScale *= (1.0f + wheel * 0.1f);
    targetScale = fmaxf(baseScale * 0.5f, fminf(targetScale, baseScale * 5.0f));
  }
  currentScale = Lerp(currentScale, targetScale, 0.1f);
}

void ImageHandler::DrawImage() {
  if (!isValidImage) {
    DrawText("Unsupported format", (screenWidth - MeasureText("Unsupported format", 40)) / 2, (screenHeight - 40) / 2, 40, RED);
    return;
  }
  float destWidth = texture.width * currentScale;
  float destHeight = texture.height * currentScale;
  float destX = (screenWidth - destWidth) / 2 + imageOffset.x;
  float destY = (screenHeight - destHeight) / 2 + imageOffset.y;

  Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
  dest = {destX, destY, destWidth, destHeight};
  Vector2 origin = {0.0f, 0.0f};

  DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

void ImageHandler::HandleMouseMovement(Vector2 mousePosition) {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, dest)) {
    dragging = true;
    dragStart = mousePosition;
    offsetStart = imageOffset;
  }
  if (dragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    Vector2 delta = Vector2Subtract(mousePosition, dragStart);
    imageOffset = Vector2Add(offsetStart, delta);
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    dragging = false;
  }
}

void ImageHandler::ResetImagePosition() {
  imageOffset = {0, 0};
  dragStart = {0, 0};
  offsetStart = {0, 0};
  dragging = false;
}

void ImageHandler::ResetImagePositionAndZoom() {
  ResetImagePosition();
  currentScale = baseScale * initialZoom;
  targetScale = currentScale;
}
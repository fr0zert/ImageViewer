#pragma once
#include <raylib.h>
#include <raymath.h>

class ImageHandler {
public:
  ImageHandler(const char *imagePath, int screenWidth, int screenHeight, float initialZoom);
  ~ImageHandler();

  void HandleZoom(float wheel);
  void DrawImage();
  void HandleMouseMovement(Vector2 mousePosition);
  void ResetImagePosition();
  void ResetImagePositionAndZoom();

private:
  Texture2D texture;
  bool isValidImage = false;
  float baseScale;
  float currentScale;
  float targetScale;
  float initialZoom;
  int screenWidth;
  int screenHeight;
  bool dragging = false;
  Vector2 imageOffset = {0, 0};
  Vector2 dragStart = {0, 0};
  Vector2 offsetStart = {0, 0};
  Rectangle dest = {0, 0, 0, 0};
};
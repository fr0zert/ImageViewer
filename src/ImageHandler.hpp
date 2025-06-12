#pragma once
#include <raylib.h>
#include <raymath.h>

class ImageHandler {
   public:
    ImageHandler(const char* imagePath, int screenWidth, int screenHeight, float initialZoom);
    ~ImageHandler();

    void HandleZoom(float wheel);
    void DrawImage();
    void HandleMouseMovement();  // TODO

   private:
    Texture2D texture;
    float baseScale;
    float currentScale;
    float targetScale;
    int screenWidth;
    int screenHeight;
};
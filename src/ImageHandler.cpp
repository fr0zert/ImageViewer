#include "ImageHandler.hpp"

ImageHandler::ImageHandler(const char* imagePath, int screenWidth, int screenHeight, float initialZoom) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->texture = LoadTexture(imagePath);

    // for future: PNGs dont need to be blurry
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

    float baseScaleX = (float)screenWidth / texture.width;
    float baseScaleY = (float)screenHeight / texture.height;
    baseScale = (baseScaleX < baseScaleY) ? baseScaleX : baseScaleY;

    currentScale = baseScale * initialZoom;
    targetScale = currentScale;
}

ImageHandler::~ImageHandler() {
    UnloadTexture(texture);
}

void ImageHandler::HandleZoom(float wheel) {
    if (wheel != 0.0f) {
        targetScale *= (1.0f + wheel * 0.1f);
        targetScale = fmaxf(baseScale * 0.5f, fminf(targetScale, baseScale * 5.0f));
    }
    currentScale = Lerp(currentScale, targetScale, 0.1f);
}

void ImageHandler::DrawImage() {
    float destWidth = texture.width * currentScale;
    float destHeight = texture.height * currentScale;
    float destX = (screenWidth - destWidth) / 2;
    float destY = (screenHeight - destHeight) / 2;

    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    Rectangle dest = {destX, destY, destWidth, destHeight};
    Vector2 origin = {0.0f, 0.0f};

    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

void ImageHandler::HandleMouseMovement() {}
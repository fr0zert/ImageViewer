
#include <FreeImage.h>
#include <filesystem>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "ImageHandler.hpp"

namespace Settings {
  const float initialZoom = 0.75f;
  bool drawDebugInfo = false;
} // namespace Settings

int main(int argc, char *argv[]) {
  FreeImage_Initialise();
#ifdef _DEBUG
  TraceLog(LOG_INFO, "FreeImage version %s", FreeImage_GetVersion());
#endif
  if (argc <= 1) return 1;
  std::filesystem::path workingDir = std::filesystem::current_path(); // get current working directory
  SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
  InitWindow(0, 0, "ImageViewer");
  MaximizeWindow();
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  // load image
  ImageHandler initImage(argv[1], screenWidth, screenHeight, Settings::initialZoom);
  while (!WindowShouldClose()) {
    float wheelMove = GetMouseWheelMove();
    Vector2 mousePosition = GetMousePosition();
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
      if (IsKeyPressed(KEY_W)) {
        break;
      }
      if (IsKeyPressed(KEY_D)) Settings::drawDebugInfo = !Settings::drawDebugInfo;
    }
    if (IsKeyPressed(KEY_SPACE)) initImage.ResetImagePosition();
    if (IsKeyPressed(KEY_R)) initImage.ResetImagePositionAndZoom();

    // Handle zoom with mouse wheel
    initImage.HandleMouseMovement(mousePosition);
    initImage.HandleZoom(wheelMove);

    BeginDrawing();
    ClearBackground({0, 0, 0, 126});
    // Draw the image
    initImage.DrawImage();
    if (Settings::drawDebugInfo) DrawFPS(10, 10);
    EndDrawing();
  }
  FreeImage_DeInitialise();
  CloseWindow();
}
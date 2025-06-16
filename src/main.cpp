#include <raylib.h>
#include <raymath.h>

#include "ImageHandler.hpp"

namespace Settings {
const float initialZoom = 0.75f;  // Initial zoom level
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        return 1;
    }

    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(0, 0, "ImageViewer");
    MaximizeWindow();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    bool drawDebugInfo = false;
    // Calculate scale to fit image inside the window (keeping aspect ratio)
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
            if (IsKeyPressed(KEY_D)) {
                drawDebugInfo = !drawDebugInfo;
            }
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
        if (drawDebugInfo) DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
}
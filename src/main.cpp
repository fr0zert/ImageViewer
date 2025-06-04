#include <raylib.h>
#include <raymath.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        return 1;
    }

    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT);
    InitWindow(0, 0, "ImageViewer");
    MaximizeWindow();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    bool drawDebugInfo = false;
    Texture2D texture = LoadTexture(argv[1]);
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    // Calculate scale to fit image inside the window (keeping aspect ratio)
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float baseScaleX = (float)screenWidth / texture.width;
    float baseScaleY = (float)screenHeight / texture.height;
    float baseScale = (baseScaleX < baseScaleY) ? baseScaleX : baseScaleY;

    float scale = baseScale * 0.75f;  // Current animated scale
    float targetScale = scale;        // Target scale to animate toward
    // RenderTexture2D target = LoadRenderTexture(100, 100);

    while (!WindowShouldClose()) {
        float wheel = GetMouseWheelMove();
        if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
            if (IsKeyPressed(KEY_W)) {
                break;
            }
            if (IsKeyPressed(KEY_D)) {
                drawDebugInfo = !drawDebugInfo;
            }
        }

        // Handle zoom with mouse wheel
        if (wheel != 0.0f) {
            targetScale *= (1.0f + wheel * 0.1f);  // Zoom in/out by 10% per step

            // Clamp zoom range
            targetScale = fmaxf(baseScale * 0.5f, fminf(targetScale, baseScale * 5.0f));
        }

        // Smooth zoom animation
        scale = Lerp(scale, targetScale, 0.1f);  // 0.1 = smoothing speed

        // Compute centered destination rect
        float destWidth = texture.width * scale;
        float destHeight = texture.height * scale;
        float destX = (screenWidth - destWidth) / 2;
        float destY = (screenHeight - destHeight) / 2;

        Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        Rectangle dest = {destX, destY, destWidth, destHeight};
        Vector2 origin = {0.0f, 0.0f};

        BeginDrawing();
        ClearBackground({30, 30, 30, 126});
        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
        // DrawTexture(target.texture, 0, 0, WHITE);
        if (drawDebugInfo) DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadTexture(texture);
    // UnloadRenderTexture(target);
    CloseWindow();
}
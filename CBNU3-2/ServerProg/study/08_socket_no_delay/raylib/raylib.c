#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "raylib HighDPI 예제");
    SetTargetFPS(60);

    Vector2 playerPos = { screenWidth / 2.0f, screenHeight / 2.0f };

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += 200.0f * GetFrameTime();
        if (IsKeyDown(KEY_LEFT))  playerPos.x -= 200.0f * GetFrameTime();
        if (IsKeyDown(KEY_UP))    playerPos.y -= 200.0f * GetFrameTime();
        if (IsKeyDown(KEY_DOWN))  playerPos.y += 200.0f * GetFrameTime();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("HighDPI 적용 완료!", 190, 200, 20, DARKGRAY);
            DrawCircleV(playerPos, 25.0f, MAROON);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

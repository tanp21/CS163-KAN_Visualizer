#include "raylib.h"

const int screenWidth = 1920;
const int screenHeight = 1080;

int main() {
    InitWindow(screenWidth, screenHeight, "KAN Visualization");

    Texture2D background = LoadTexture("resources/StartPage/background.png");

    SetTargetFPS(60);
    ToggleFullscreen();

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(background, 95, 7, WHITE);

        EndDrawing();
    }

    return 0;
}
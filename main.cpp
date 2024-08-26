// Frontend
#include "raylib.h"
#include "Engine.h"

// Backend
#include "./KAN/KAN.h"

const int screenWidth = 1920;
const int screenHeight = 1080;

int main() {
    InitWindow(screenWidth, screenHeight, "KAN Visualization");

    // Texture2D background = LoadTexture("resources/StartPage/background.png");

    SetTargetFPS(60);
    ToggleFullscreen();

    AutoTexture tex("resources/StartPage/background.png");

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(tex.getTexture(), 95, 7, WHITE);

        EndDrawing();
    }

    return 0;
}
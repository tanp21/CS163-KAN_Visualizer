// Frontend
#include "raylib.h"
#include <string>
#include "EngineSupport.h"

// Backend
#include "./KAN/KAN.h"

#define START_PAGE_PATH "./resources/StartPage/"

const int screenWidth = 1920;
const int screenHeight = 1080;

class StartPage {
    private:
    AutoTexture background;
    AutoTexture title;
    AutoTexture train_button;
    AutoTexture infer_button;
    AutoTexture info_button;
    AutoTexture man_button;

    public:
    StartPage()
        : background(START_PAGE_PATH"background.png", 95, 7),
          title(START_PAGE_PATH"title.png", 171, 123),
          train_button(START_PAGE_PATH"training_button.png", 756, 581),
          infer_button(START_PAGE_PATH"inference_button.png", 756, 815),
          info_button(START_PAGE_PATH"info.png", 59, 932),
          man_button(START_PAGE_PATH"manual.png", 1760, 932) {}

    void show() {
        background.draw();
        title.draw();
        train_button.draw();
        infer_button.draw();
        info_button.draw();
        man_button.draw();
    }
};

int main() {
    InitWindow(screenWidth, screenHeight, "KAN Visualization");

    SetTargetFPS(60);
    ToggleFullscreen();

    StartPage st;

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        st.show();

        EndDrawing();
    }

    return 0;
}
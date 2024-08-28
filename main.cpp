// Frontend
#include "raylib.h"
#include <string>
#include "EngineSupport.h"

// Backend
#include "./KAN/KAN.h"

#define START_PAGE_PATH "./resources/StartPage/"
#define VISUAL_PAGE_PATH "./resources/VisualizingPage/"

const int screenWidth = 1920;
const int screenHeight = 1080;

enum State {
    STARTING_PAGE,
    TRAINING,
    INFERENCE
};

State current_state;

class StartPage {
    private:
    // textures
    AutoTexture background;
    AutoTexture title;

    // buttons
    ButtonTexture train_button;
    ButtonTexture infer_button;
    ButtonTexture info_button;
    ButtonTexture man_button;

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

        train_button.action();
        infer_button.action();
        info_button.action();
        man_button.action();

        train_button.draw();
        infer_button.draw();
        info_button.draw();
        man_button.draw();
    }
};

class VisualizePage {
private:
    // states
    enum PageState {
        PLAYING,
        PAUSED
    };

    PageState state;

    // play bar
    AutoTexture play_bar;
    AutoTexture progress_bar;
    AutoTexture loss_text;

    // buttons
    ButtonTexture play_button;
    ButtonTexture pause_button;
    ButtonTexture prev_button;
    ButtonTexture next_button;
    ButtonTexture setting_button;

public:
    VisualizePage()
        :   play_bar(VISUAL_PAGE_PATH"play_bar.png", 0, 973),
            progress_bar(VISUAL_PAGE_PATH"progress_bar.png", 348, 1005),
            loss_text(VISUAL_PAGE_PATH"loss_text.png", 1274, 992),
            play_button(VISUAL_PAGE_PATH"play_button.png", 136, 996),
            pause_button(VISUAL_PAGE_PATH"pause_button.png", 141, 1000),
            prev_button(VISUAL_PAGE_PATH"prev_button.png", 59, 1007),
            next_button(VISUAL_PAGE_PATH"next_button.png", 229, 1007),
            setting_button(VISUAL_PAGE_PATH"setting_button.png", 1789, 992),
            state(PAUSED) {}

    void play_or_pause() {
        if(state == PAUSED) {
            if(play_button.action()) {
                state = PLAYING;
            }
            play_button.draw();
        }
        else {
            if(pause_button.action()) {
                state = PAUSED;
            }
            pause_button.action();
            pause_button.draw();
        }
    }

    void show() {
        play_bar.draw();
        progress_bar.draw();
        loss_text.draw();

        play_or_pause();

        prev_button.action();
        next_button.action();
        setting_button.action();

        prev_button.draw();
        next_button.draw();
        setting_button.draw();
    }
};

int main() {
    InitWindow(screenWidth, screenHeight, "KAN Visualization");

    SetTargetFPS(60);
    ToggleFullscreen();

    StartPage st;
    VisualizePage viz;

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // st.show();
        viz.show();

        EndDrawing();
    }

    return 0;
}
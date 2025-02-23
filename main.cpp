// Frontend
#include "raylib.h"
#include <string>
#include "EngineSupport.h"
#include "./KAN/visualizer.h"
#include <cassert>
#include <cmath>
#include <algorithm>

// Backend
#include "KAN/KAN.h"

#define START_PAGE_PATH "./resources/StartPage/"
#define VISUAL_PAGE_PATH "./resources/VisualizingPage/"
#define DATA_VIS_PATH "./resources/Visualizer/"

// Constants
const int screenWidth = 1920;
const int screenHeight = 1080;

enum State {
    STARTING_PAGE,
    TRAINING,
    INFERENCE,
    SETTING,
    STEP_BY_STEP,
    LOAD_MODEL
};

State current_state;

class StartPage {
    private:
    // textures
    AutoTexture background;
    AutoTexture title;

    // buttons
    ButtonTexture train_button;
    ButtonTexture load_button;
    ButtonTexture info_button;
    ButtonTexture man_button;

    public:
    StartPage()
        : background(START_PAGE_PATH"background.png", 95, 7),
          title(START_PAGE_PATH"title.png", 171, 123),
          train_button(START_PAGE_PATH"training_button.png", 756, 581),
          load_button(START_PAGE_PATH"load_button.png", 756, 815),
          info_button(START_PAGE_PATH"info.png", 59, 932),
          man_button(START_PAGE_PATH"manual.png", 1760, 932) {}

    void show() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        background.draw();
        title.draw();

        if(train_button.action()) {
            current_state = SETTING;
        }
        if(load_button.action() && KANN::cnt > 0) {
            current_state = LOAD_MODEL;
        }
        // infer_button.action();
        // info_button.action();
        // man_button.action();

        train_button.draw();
        load_button.draw();
        // infer_button.draw();
        // info_button.draw();
        // man_button.draw();
        EndDrawing();
    }
};

const int MAX_INPUT_CHARS = 64;

int DrawTextBoxWithInput(const char *title, int posX, int posY, int minVal, int maxVal) {
    char inputText[MAX_INPUT_CHARS + 1] = "\0"; // Buffer to store input text
    int letterCount = 0; // Number of characters in the inputText

    Rectangle textBox = { posX, posY, 400, 50 }; // Define the textbox dimensions and position
    bool mouseOnText = false; // Check if mouse is hovering over the textbox
    int framesCounter = 0; // Counter to manage cursor blinking

    // Main loop to handle the textbox
    while (!WindowShouldClose())
    {
        // Update
        // Check if mouse is inside the textbox
        mouseOnText = CheckCollisionPointRec(GetMousePosition(), textBox);

        // If mouse is inside the textbox, allow input
        if (mouseOnText)
        {
            // Set the window to use the standard input
            if (IsKeyPressed(KEY_ENTER)) // Pressing ENTER finishes input
            {
                bool valid = true;
                int ans = 0;
                for (int i = 0; inputText[i]; i++) {
                    if(inputText[i] >= '0' && inputText[i] <= '9') {
                        ans = ans*10 + (inputText[i] - '0');
                    }
                    else {
                        valid = false;
                        break;
                    }
                }
                if(valid && ans >= minVal && ans <= maxVal) return ans;
                // Do something with the inputText, e.g., print it or pass it to another function
            }
            else if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) // Handle backspace
            {
                letterCount--;
                inputText[letterCount] = '\0';
            }
            else if (letterCount < MAX_INPUT_CHARS)
            {
                int key = GetCharPressed(); // Get pressed character (unicode)

                // Check for valid key characters
                if ((key >= 32) && (key <= 125))
                {
                    inputText[letterCount] = (char)key;
                    letterCount++;
                    inputText[letterCount] = '\0';
                }
            }
        }

        // Cursor blinking logic
        if (mouseOnText)
        {
            framesCounter++;
        }
        else
        {
            framesCounter = 0;
        }

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the title text
        DrawText(title, posX, posY - 30, 20, DARKGRAY);

        DrawRectangleRec(textBox, LIGHTGRAY); // Draw the textbox
        if (mouseOnText) DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
        else DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);

        DrawText(inputText, textBox.x + 5, textBox.y + 15, 20, MAROON);

        // Draw blinking cursor
        if (mouseOnText)
        {
            if (((framesCounter / 20) % 2) == 0) DrawText("_", textBox.x + 8 + MeasureText(inputText, 20), textBox.y + 15, 20, MAROON);
        }

        EndDrawing();
    }
    return -1;
}

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
    // AutoTexture progress_bar;
    // AutoTexture loss_text;

    // buttons
    ButtonTexture play_button;
    ButtonTexture pause_button;
    ButtonTexture prev_button;
    ButtonTexture next_button;
    ButtonTexture home_button;

public:
    VisualizePage()
        :   play_bar(VISUAL_PAGE_PATH"play_bar.png", 0, 973),
            play_button(VISUAL_PAGE_PATH"play_button.png", 136, 996),
            pause_button(VISUAL_PAGE_PATH"pause_button.png", 141, 1000),
            prev_button(VISUAL_PAGE_PATH"prev_button.png", 59, 1007),
            next_button(VISUAL_PAGE_PATH"next_button.png", 229, 1007),
            home_button(VISUAL_PAGE_PATH"home_button.png", 1789, 992),
            state(PAUSED) {}

    // void play_or_pause() {
    //     if(state == PAUSED) {
    //         if(play_button.action()) {
    //             state = PLAYING;
    //         }
    //         play_button.draw();
    //     }
    //     else {
    //         if(pause_button.action()) {
    //             state = PAUSED;
    //         }
    //         pause_button.action();
    //         pause_button.draw();
    //     }
    // }

    void show() {
        int cur = KANN::cnt-1;
        KANN::KAN tmp({1, 2, 1});

        while(!WindowShouldClose() && current_state != STARTING_PAGE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            play_bar.draw();
            // progress_bar.draw();
            // loss_text.draw();

            // play_or_pause();

            if(prev_button.action()) cur--;
            if(next_button.action()) cur++;
            if(home_button.action()) {
                current_state = STARTING_PAGE;
            }

            cur = std::min(cur, KANN::cnt-1);
            cur = std::max(cur, 0);
            tmp.loadParameters("history/" + std::to_string(cur) + ".bin");


            prev_button.draw();
            next_button.draw();
            home_button.draw();

            EndDrawing();
        }
    }
};

const float PII = acos(-1);

float func(float x, float y) {
    return exp(sin(PII * x) + y*y);
}

float randomFloat(float l, float r) {
    // Normalize rand() to [0, 1]
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    // Scale to the range [l, r]
    float result = l + random * (r - l);
    
    return result;
}

int main(int argc, char* argv[]) {
    srand(atoi(argv[1]));


    // int cnt = 0;


    InitWindow(screenWidth, screenHeight, "KAN Visualization");

    SetTargetFPS(60);
    ToggleFullscreen();

    StartPage st;
    VisualizePage viz;

    current_state = STARTING_PAGE;
    int num_epochs, num_sample, numlayer;
    std::vector<int> layer_in4;
    KANN::set_grid_range(-1, 1, 5);

    while(!WindowShouldClose()) {
        if(current_state == STARTING_PAGE) {
            st.show();
        }
        else if(current_state == SETTING) {
            num_epochs = DrawTextBoxWithInput("Enter number of epochs", (screenWidth - 400)/2, (screenHeight - 50)/2, 1, 20000);
            num_sample = DrawTextBoxWithInput("Enter number of sample for each epochs", (screenWidth - 400)/2, (screenHeight - 50)/2, 1, 1000);
            numlayer = DrawTextBoxWithInput("Enter number of layer", (screenWidth - 400)/2, (screenHeight - 50)/2, 1, 6);
            layer_in4.resize(numlayer + 2);
            layer_in4[0] = 2; layer_in4[numlayer+1] = 1;
            for (int i = 1; i <= numlayer; i++) {
                layer_in4[i] = DrawTextBoxWithInput(("Enter number of neurons for layer " + std::to_string(i)).c_str(), (screenWidth - 400)/2, (screenHeight - 50)/2, 1, 6);
            }
            KANN::prev_in4 = layer_in4;
            current_state = TRAINING;
        }
        else if(current_state == TRAINING) {
            KANN::KAN kan(layer_in4);

            std::vector<std::pair<std::vector<float>, std::vector<float>>> dataset;
            for(int i = 0; i < num_sample; i++) {
                float x = randomFloat(-1, 1);
                float y = randomFloat(-1, 1);

                dataset.push_back({{x, y}, {func(x, y)}});
            }

            kan.train(dataset, num_epochs);
            current_state = STEP_BY_STEP;
        }
        else if(current_state == STEP_BY_STEP) {
            viz.show();
        }
        else if(current_state == LOAD_MODEL) {
            viz.show();
        }
        
    }

    return 0;
}

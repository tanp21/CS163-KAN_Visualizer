#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>

class AutoTexture {
private:
    Texture2D texture;
    float x, y;
public:
    AutoTexture(const std::string& filePath, float initialX = 0, float initialY = 0) 
        : x(initialX), y(initialY) {
        texture = LoadTexture(filePath.c_str());
        if (texture.id == 0) {
            std::cerr << "Failed to load texture: " << filePath << std::endl;
        } else {
            std::cout << "Texture loaded: " << filePath << std::endl;
        }
    }

    ~AutoTexture() {
        if (texture.id != 0) {
            UnloadTexture(texture);
            std::cout << "Texture unloaded." << std::endl;
        }
    }

    virtual void draw() const {
        DrawTexture(texture, x, y, WHITE);
    }

    virtual void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }

    int getX() const { return x; }
    int getY() const { return y; }

    Texture2D getTexture() const {
        return texture;
    }

    AutoTexture(const AutoTexture&) = delete;
    AutoTexture& operator=(const AutoTexture&) = delete;

    AutoTexture(AutoTexture&& other) noexcept : texture(other.texture) {
        other.texture.id = 0;
    }

    AutoTexture& operator=(AutoTexture&& other) noexcept {
        if (this != &other) {
            if (texture.id != 0) {
                UnloadTexture(texture);
            }
            texture = other.texture;
            other.texture.id = 0;
        }
        return *this;
    }
};

class ButtonTexture : public AutoTexture {
private:
    enum ButtonState {
        NORMAL,
        HOVER,
        PRESS,
    };

    ButtonState state;

    Rectangle bounds;    // Rectangle to hold the position and size of the button
    float scaleFactor;   // Factor to scale the button when pressed
    Color normalColor;   // Normal state color (white)
    Color hoverColor;    // Hover state color (darker)
    Color pressedColor;  // Pressed state color (even darker)
    Color currentColor;  // Current state color

public:
    ButtonTexture(const std::string& filePath, float initialX = 0, float initialY = 0) 
        : AutoTexture(filePath, initialX, initialY), scaleFactor(0.9f), normalColor(WHITE), hoverColor(GRAY), pressedColor(DARKGRAY), state(NORMAL) {
        
        if (getTexture().id != 0) {
            bounds = { initialX, initialY, static_cast<float>(getTexture().width), static_cast<float>(getTexture().height) };
            currentColor = normalColor;
        }
    }

    bool action() {
        bool is_action = false;

        if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                state = PRESS;
                currentColor = pressedColor;
            }
            else {
                state = HOVER;
                currentColor = hoverColor;
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) is_action = true;
        }
        else {
            state = NORMAL;
            currentColor = normalColor;
        }

        return is_action;
    }

    void draw() const override {
        if (state == PRESS) {
            DrawTextureEx(getTexture(), { bounds.x + bounds.width * (1 - scaleFactor) / 2, 
                                          bounds.y + bounds.height * (1 - scaleFactor) / 2 }, 
                          0.0f, scaleFactor, currentColor);
        } else {
            DrawTexture(getTexture(), bounds.x, bounds.y, currentColor);
        }
    }

    void setPosition(float newX, float newY) override {
        AutoTexture::setPosition(newX, newY);
        bounds.x = newX;
        bounds.y = newY;
    }

    float getX() const { return bounds.x; }
    float getY() const { return bounds.y; }
};

class LineDrawer {
private:
    Vector2 start;
    Vector2 end;
    Color defaultColor;
    Color currentColor;

    Color calculateColor(float scaleFactor) {
        float clampedScale = std::clamp(scaleFactor, 0.0f, 255.0f);

        float scaleFactorAdjustment = std::min(clampedScale, 255.0f) / 255.0f;

        int red = static_cast<int>(defaultColor.r * (1.0f - scaleFactorAdjustment));
        int green = static_cast<int>(defaultColor.g * (1.0f - scaleFactorAdjustment));
        int blue = static_cast<int>(defaultColor.b * (1.0f - scaleFactorAdjustment));

        return { red, green, blue, defaultColor.a };
    }

public:
    LineDrawer(float startX, float startY, float endX, float endY, Color color)
        : start{startX, startY}, end{endX, endY}, defaultColor(color), currentColor(color) {}

    void draw() const {
        DrawLineV(start, end, currentColor);
    }

    void setScaleFactor(float scaleFactor) {
        currentColor = calculateColor(scaleFactor);
    }

    void setPosition(float startX, float startY, float endX, float endY) {
        start = { startX, startY };
        end = { endX, endY };
    }
};

#endif
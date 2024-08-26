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

    void draw() const {
        DrawTexture(texture, x, y, WHITE);
    }

    void setPosition(int newX, int newY) {
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

#endif
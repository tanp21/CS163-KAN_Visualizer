#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>

class AutoTexture {
private:
    Texture2D texture;
public:
    AutoTexture(const std::string& filePath) {
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
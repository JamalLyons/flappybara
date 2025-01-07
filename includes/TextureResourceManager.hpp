//
// Created by codingwithjamal on 1/3/2025.
//

#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>
#include "Logger.hpp"

class TextureResourceManager {
public:
    TextureResourceManager();
    ~TextureResourceManager();

    void loadTextureFromHeader(const std::string &key, const Image &image);
    void loadTextureResources();
    void addTexture(const std::string &key, const std::string &path);
    Texture2D getTexture(const std::string &key) const;
    void unloadTexture(const std::string &key);
    void unloadAllTextures();
    void buildTextureHeaders();

private:
    std::unordered_map<std::string, Texture2D> textureResources;
    std::unordered_map<std::string, std::string> predefinedTextures = {
            {"floor", "../resources/textures/base.png"},
            {"background-day", "../resources/textures/background_day.png"},
            {"background-night", "../resources/textures/background_night.png"},
            {"pipe-green", "../resources/textures/pipe_green.png"},
            {"pipe-red", "../resources/textures/pipe_red.png"},
            {"player", "../resources/textures/player.png"},{}
    };
};

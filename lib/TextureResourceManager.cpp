//
// Created by codingwithjamal on 1/3/2025.
//

#include "TextureResourceManager.h"
#include <filesystem>

#include "../resources/textures/headers/background_day_texture.h"
#include "../resources/textures/headers/background_night_texture.h"
#include "../resources/textures/headers/base_texture.h"
#include "../resources/textures/headers/pipe_green_texture.h"
#include "../resources/textures/headers/pipe_red_texture.h"

TextureResourceManager::TextureResourceManager() {
    loadTextureResources();
}

TextureResourceManager::~TextureResourceManager() {
    unloadAllTextures();
}

void TextureResourceManager::loadTextureResources() {
    constexpr Image background_day_img = {
        .data = BACKGROUND_DAY_TEXTURE_DATA,
        .width = BACKGROUND_DAY_TEXTURE_WIDTH,
        .height = BACKGROUND_DAY_TEXTURE_HEIGHT,
        .format = BACKGROUND_DAY_TEXTURE_FORMAT,
    };

    constexpr Image background_night_img = {
        .data = BACKGROUND_NIGHT_TEXTURE_DATA,
        .width = BACKGROUND_NIGHT_TEXTURE_WIDTH,
        .height = BACKGROUND_NIGHT_TEXTURE_HEIGHT,
        .format = BACKGROUND_NIGHT_TEXTURE_FORMAT,
    };

    constexpr Image base_img = {
        .data = BASE_TEXTURE_DATA,
        .width = BASE_TEXTURE_WIDTH,
        .height = BASE_TEXTURE_HEIGHT,
        .format = BASE_TEXTURE_FORMAT,
    };

    constexpr Image pipe_green_img = {
        .data = PIPE_GREEN_TEXTURE_DATA,
        .width = PIPE_GREEN_TEXTURE_WIDTH,
        .height = PIPE_GREEN_TEXTURE_HEIGHT,
        .format = PIPE_GREEN_TEXTURE_FORMAT,
    };

    constexpr Image pipe_red_img = {
        .data = PIPE_RED_TEXTURE_DATA,
        .width = PIPE_RED_TEXTURE_WIDTH,
        .height = PIPE_RED_TEXTURE_HEIGHT,
        .format = PIPE_RED_TEXTURE_FORMAT,
    };

    loadTextureFromHeader("background-day", background_day_img);
    loadTextureFromHeader("background-night", background_night_img);
    loadTextureFromHeader("floor", base_img);
    loadTextureFromHeader("pipe-green", pipe_green_img);
    loadTextureFromHeader("pipe-red", pipe_red_img);
}

void TextureResourceManager::loadTextureFromHeader(const std::string &key, const Image &image) {
    if (textureResources.contains(key)) {
        std::cerr << "Warning: Texture key '" << key << "' already exists. Skipping load." << std::endl;
        return;
    }

    const Texture2D texture = LoadTextureFromImage(image);
    if (texture.id == 0) {
        throw std::runtime_error("Error: Failed to load texture from header data for key: " + key);
    }

    textureResources[key] = texture;
    std::cout << "Loaded texture '" << key << "' with ID: " << texture.id << std::endl;
}

Texture2D TextureResourceManager::getTexture(const std::string &key) const {
    if (!textureResources.contains(key)) {
        throw std::runtime_error("Error: Texture key '" + key + "' not found!");
    }
    return textureResources.at(key);
}

void TextureResourceManager::unloadTexture(const std::string &key) {
    if (textureResources.contains(key)) {
        UnloadTexture(textureResources[key]);
        textureResources.erase(key);
        std::cout << "Unloaded texture: " << key << std::endl;
    } else {
        std::cerr << "Error: Texture key '" << key << "' not found!" << std::endl;
    }
}

void TextureResourceManager::unloadAllTextures() {
    for (auto &[key, texture] : textureResources) {
        UnloadTexture(texture);
    }
    textureResources.clear();
    std::cout << "Unloaded all textures." << std::endl;
}

void TextureResourceManager::buildTextureHeaders() {

    if constexpr (!Config::buildTextureHeaders) {
        std::cout << "Texture headers building disabled." << std::endl;
        return;
    }

    const std::string outputDir = "../resources/textures/headers/";
    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directories(outputDir);
        std::cout << "Created output directory: " << outputDir << std::endl;
    }

    for (const auto &[key, path] : predefinedTextures) {
        // Load the image file
        const Image image = LoadImage(path.c_str());
        if (!image.data) {
            std::cerr << "Error: Failed to load image data: " << path << std::endl;
            continue;
        }

        // Generate a sanitized output header file name
        const std::string filename = path.substr(path.find_last_of("/\\") + 1);
        const std::string sanitizedFilename = filename.substr(0, filename.find_last_of('.')) + "_texture.h";
        const std::string outputPath = outputDir + sanitizedFilename;

        std::cout << "Building header: " << outputPath << std::endl;

        // Export the image as a C header
        if (!ExportImageAsCode(image, outputPath.c_str())) {
            TraceLog(LOG_ERROR, "Failed to export image as C header file: %s", outputPath.c_str());
        } else {
            TraceLog(LOG_INFO, "Image exported as C header file successfully: %s", outputPath.c_str());
        }

        // Unload the image to free memory
        UnloadImage(image);
    }

    std::cout << "Finished building texture headers." << std::endl;
}
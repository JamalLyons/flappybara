//
// Created by codingwithjamal on 1/3/2025.
//

#include <filesystem>
#include "TextureResourceManager.hpp"
#include <iostream>

#include "constants.hpp"

#include "../resources/textures/headers/background_day_texture.h"
// #include "../resources/textures/headers/background_night_texture.h"
#include "../resources/textures/headers/base_texture.h"
#include "../resources/textures/headers/pipe_green_texture.h"
// #include "../resources/textures/headers/pipe_red_texture.h"
#include "../resources/textures/headers/player_texture.h"

TextureResourceManager::TextureResourceManager() {
    loadTextureResources();
}

TextureResourceManager::~TextureResourceManager() {
    unloadAllTextures();
}

void TextureResourceManager::loadTextureResources() {
    Logger& logger = Logger::getInstance();
    logger.log(LogLevel::INFO, "Loading texture resources.");

    constexpr Image background_day_img = {
        .data = BACKGROUND_DAY_TEXTURE_DATA,
        .width = BACKGROUND_DAY_TEXTURE_WIDTH,
        .height = BACKGROUND_DAY_TEXTURE_HEIGHT,
        .mipmaps = 1,
        .format = BACKGROUND_DAY_TEXTURE_FORMAT,
    };

    // constexpr Image background_night_img = {
    //     .data = BACKGROUND_NIGHT_TEXTURE_DATA,
    //     .width = BACKGROUND_NIGHT_TEXTURE_WIDTH,
    //     .height = BACKGROUND_NIGHT_TEXTURE_HEIGHT,
    //     .mipmaps = 1,
    //     .format = BACKGROUND_NIGHT_TEXTURE_FORMAT,
    // };

    constexpr Image base_img = {
        .data = BASE_TEXTURE_DATA,
        .width = BASE_TEXTURE_WIDTH,
        .height = BASE_TEXTURE_HEIGHT,
        .mipmaps = 1,
        .format = BASE_TEXTURE_FORMAT,
    };

    constexpr Image pipe_green_img = {
        .data = PIPE_GREEN_TEXTURE_DATA,
        .width = PIPE_GREEN_TEXTURE_WIDTH,
        .height = PIPE_GREEN_TEXTURE_HEIGHT,
        .mipmaps = 1,
        .format = PIPE_GREEN_TEXTURE_FORMAT,
    };

    // constexpr Image pipe_red_img = {
    //     .data = PIPE_RED_TEXTURE_DATA,
    //     .width = PIPE_RED_TEXTURE_WIDTH,
    //     .height = PIPE_RED_TEXTURE_HEIGHT,
    //     .mipmaps = 1,
    //     .format = PIPE_RED_TEXTURE_FORMAT,
    // };

    constexpr Image player_img = {
        .data = PLAYER_TEXTURE_DATA,
        .width = PLAYER_TEXTURE_WIDTH,
        .height = PLAYER_TEXTURE_HEIGHT,
        .mipmaps = 1,
        .format = PLAYER_TEXTURE_FORMAT,
    };

    loadTextureFromHeader("background-day", background_day_img);
    // loadTextureFromHeader("background-night", background_night_img);
    loadTextureFromHeader("floor", base_img);
    loadTextureFromHeader("pipe-green", pipe_green_img);
    // loadTextureFromHeader("pipe-red", pipe_red_img);
    loadTextureFromHeader("player", player_img);

    logger.log(LogLevel::INFO, "Texture resources loaded successfully.");
}

void TextureResourceManager::loadTextureFromHeader(const std::string &key, const Image &image) {
    Logger& logger = Logger::getInstance();

    if (textureResources.contains(key)) {
        logger.log(LogLevel::WARNING, "Texture key '" + key + "' already exists. Skipping load.");
        return;
    }

    const Texture2D texture = LoadTextureFromImage(image);
    if (texture.id == 0) {
        logger.log(LogLevel::ERROR, "Error: Failed to load texture from header data for key: " + key);
        throw std::runtime_error("Error: Failed to load texture from header data for key: " + key);
    }

    textureResources[key] = texture;
    logger.log(LogLevel::INFO, "Loaded texture '" + key + "' with ID: " + std::to_string(texture.id));
}

Texture2D TextureResourceManager::getTexture(const std::string &key) const {
    Logger& logger = Logger::getInstance();

    if (!textureResources.contains(key)) {
        logger.log(LogLevel::ERROR, "Error: Texture key '" + key + "' not found!");
        throw std::runtime_error("Error: Texture key '" + key + "' not found!");
    }
    return textureResources.at(key);
}

void TextureResourceManager::unloadTexture(const std::string &key) {
    Logger& logger = Logger::getInstance();
    if (textureResources.contains(key)) {
        UnloadTexture(textureResources[key]);
        textureResources.erase(key);
        logger.log(LogLevel::INFO, "Unloaded texture '" + key + "'.");
    } else {
        std::cerr << "Error: Texture key '" << key << "' not found!" << std::endl;
    }
}

void TextureResourceManager::unloadAllTextures() {
    Logger& logger = Logger::getInstance();

    for (auto &[key, texture] : textureResources) {
        UnloadTexture(texture);
    }
    textureResources.clear();

    logger.log(LogLevel::INFO, "Unloaded all textures.");
}

void TextureResourceManager::addTexture(const std::string &key, const std::string &path) {
    Logger& logger = Logger::getInstance();

    if (textureResources.contains(key)) {
        logger.log(LogLevel::WARNING, "Texture key '" + key + "' already exists. Skipping load.");
        return;
    }

    const Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        logger.log(LogLevel::ERROR, "Error: Failed to load texture from path: " + path);
        throw std::runtime_error("Error: Failed to load texture from path: " + path);
    }

    textureResources[key] = texture;

    logger.log(LogLevel::INFO, "Loaded texture '" + key + "' with ID: " + std::to_string(texture.id));
}


void TextureResourceManager::buildTextureHeaders() {
    Logger& logger = Logger::getInstance();

    if constexpr (!Config::buildTextureHeaders) {
        logger.log(LogLevel::INFO, "Texture headers building disabled.");
        return;
    }

    const std::string outputDir = "../resources/textures/headers/";
    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directories(outputDir);
        logger.log(LogLevel::INFO, "Created missing output directory for texture headers: " + outputDir);
    }

    for (const auto &[key, path] : predefinedTextures) {
        // Load the image file
        const Image image = LoadImage(path.c_str());
        if (!image.data) {
            logger.log(LogLevel::ERROR, "Error: Failed to load image data: " + path);
            continue;
        }

        // Generate a sanitized output header file name
        const std::string filename = path.substr(path.find_last_of("/\\") + 1);
        const std::string sanitizedFilename = filename.substr(0, filename.find_last_of('.')) + "_texture.h";
        const std::string outputPath = outputDir + sanitizedFilename;

        logger.log(LogLevel::INFO, "Building header: " + outputPath);

        // Export the image as a C header
        if (!ExportImageAsCode(image, outputPath.c_str())) {
            logger.log(LogLevel::ERROR, "Failed to export image as C header file: " + outputPath);
        } else {
            logger.log(LogLevel::INFO, "Image exported as C header file successfully: " + outputPath);
        }

        // Unload the image to free memory
        UnloadImage(image);
    }

    logger.log(LogLevel::INFO, "Texture headers building completed.");
}
//
// Created by codingwithjamal on 1/2/2025.
//
#pragma once

#include "raylib.h"

namespace Config {
    static constexpr int FPS = 60;
    static constexpr int WindowWidth = 800;
    static constexpr int WindowHeight = 450;
    static constexpr auto WindowTitle = "FlappyBara";

    // Enable audio file header building in development.
    // Remember to disable this in release builds.
    static constexpr bool buildAudioHeaders = false;
    static constexpr bool buildTextureHeaders = false;
}

namespace GlobalVariables {
    static constexpr float defaultSpeed = 0.0f;
    static constexpr Vector2 defaultPosition = { Config::WindowHeight / 4.0f, Config::WindowHeight / 2.0f };
}

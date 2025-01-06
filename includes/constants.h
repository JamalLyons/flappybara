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
    static constexpr float gravity = 400.0f;      // pixels per second ^ 2
    static constexpr float jumpHeight = -250.0f;

    static constexpr float defaultSpeed = 0.0f;
    static constexpr Vector2 defaultPosition = { Config::WindowHeight / 4.0f, Config::WindowHeight / 2.0f };

    static constexpr int pipeWidth = 80;
    static constexpr int pipeGap = 150;
    static constexpr int pipeSpeed = 200;

    static Rectangle pipes[2] = {
        { Config::WindowWidth, 0, pipeWidth, 200 },                                                 // Top pipe
        { Config::WindowWidth, 200 + pipeGap, pipeWidth, Config::WindowHeight - 200 - pipeGap }     // Bottom pipe
    };
}

enum class GameActivityState {
    MENU,
    PLAYING,
    PAUSED,
    LOADING,
    SETTINGS,
    GAME_OVER
};
//
// Created by codingwithjamal on 1/2/2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"

namespace Config {
    static constexpr int FPS = 60;
    static constexpr int WindowWidth = 800;
    static constexpr int WindowHeight = 450;
    static constexpr auto WindowTitle = "FlappyBara";

    // Enable audio file header building in development.
    // Remember to disable this in release builds.
    static constexpr bool buildAudioHeaders = false;
}

namespace GlobalVariables {
    static constexpr float gravity = 400.0f;      // pixels per second ^ 2
    static constexpr float jumpHeight = -250.0f;

    static constexpr float defaultSpeed = 0.0f;
    static constexpr Vector2 defaultPosition = { Config::WindowHeight / 4.0f, Config::WindowHeight / 2.0f };
}

enum class GameActivityState {
    MENU,
    PLAYING,
    PAUSED,
    LOADING,
    SETTINGS,
    GAME_OVER
};


#endif //CONSTANTS_H

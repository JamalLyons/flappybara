//
// Created by codingwithjamal on 1/1/2025.
//

#include <iostream>
#include "main.h"
#include "game.h"

int main() {
    InitWindow(Config::WindowWidth, Config::WindowHeight, Config::WindowTitle);
    SetTargetFPS(Config::FPS);
    SetExitKey(0);

    AudioResourceManager audioManager;
    audioManager.buildAudioHeaders();

    GameState game_state{
        .activity_state = GameActivityState::MENU,
        .score = 0,
        .playerSpeed = GlobalVariables::defaultSpeed,
        .playerPosition = GlobalVariables::defaultPosition,
    };

    while (!WindowShouldClose()) {
        // Update game logic
        update(game_state, audioManager);

        // Draw the game
        BeginDrawing();
        ClearBackground(BLACK);
        draw(game_state);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// Update game logic based on the current state
void update(GameState& game_state, AudioResourceManager& audioManager) {
    if (game_state.activity_state == GameActivityState::PLAYING) {
        run_playing(game_state, audioManager);
    }

    if (game_state.activity_state == GameActivityState::GAME_OVER) {
        reset_game(game_state);
    }

    if (game_state.activity_state == GameActivityState::MENU && IsKeyPressed(KEY_ENTER)) {
        game_state.activity_state = GameActivityState::PLAYING;
    }

    if (game_state.activity_state == GameActivityState::PLAYING && IsKeyPressed(KEY_P)) {
        game_state.activity_state = GameActivityState::PAUSED;
    }

    if (game_state.activity_state == GameActivityState::PAUSED && IsKeyPressed(KEY_R)) {
        game_state.activity_state = GameActivityState::PLAYING;
    }

    if (IsKeyPressed(KEY_K)) {
        game_state.activity_state = GameActivityState::SETTINGS;
    }

    if (game_state.activity_state == GameActivityState::SETTINGS && IsKeyPressed(KEY_ESCAPE)) {
        game_state.activity_state = GameActivityState::PLAYING;
    }

    if (game_state.activity_state == GameActivityState::GAME_OVER && IsKeyPressed(KEY_ENTER)) {
        game_state.activity_state = GameActivityState::MENU;
    }
}

void draw(const GameState& game_state) {
    // Render visuals based on the current state
    switch (game_state.activity_state) {
        case GameActivityState::MENU:
            DrawText("Menu Screen - Press ENTER to Play", Config::WindowWidth / 2 - 200, Config::WindowHeight / 2, 20, WHITE);
            break;
        case GameActivityState::PLAYING:
            DrawText("Playing - Press P to Pause", Config::WindowWidth / 2 - 100, Config::WindowHeight / 2, 20, WHITE);
            draw_playing(game_state);
            break;
        case GameActivityState::PAUSED:
            DrawText("Paused - Press R to Resume", Config::WindowWidth / 2 - 100, Config::WindowHeight / 2, 20, WHITE);
            break;
        case GameActivityState::LOADING:
            DrawText("Loading...", Config::WindowWidth / 2 - 50, Config::WindowHeight / 2, 20, WHITE);
            break;
        case GameActivityState::SETTINGS:
            DrawText("Settings - Press ESC to Return", Config::WindowWidth / 2 - 100, Config::WindowHeight / 2, 20, WHITE);
            break;
        case GameActivityState::GAME_OVER:
            DrawText("Game Over - Press ENTER to Restart", Config::WindowWidth / 2 - 150, Config::WindowHeight / 2, 20, WHITE);
            break;
        default:
            DrawText("Unknown State", Config::WindowWidth / 2 - 50, Config::WindowHeight / 2, 20, RED);
            break;
    }
}
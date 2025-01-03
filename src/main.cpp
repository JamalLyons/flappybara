//
// Created by codingwithjamal on 1/1/2025.
//

#include "main.h"
#include "raylib.h"
#include "AudioResourceManager.h"

#include "../resources/audio/headers/spring_audio.h"

int main() {
    InitWindow(Config::WindowWidth, Config::WindowHeight, Config::WindowTitle);
    SetTargetFPS(Config::FPS);

    AudioResourceManager audioManager;
    audioManager.buildAudioHeaders();

    GameState game_state{ .activity_state = GameActivityState::MENU };

    while (!WindowShouldClose()) {
        // Update game logic
        update(game_state);

        // Draw the game
        BeginDrawing();
        ClearBackground(BLACK);
        draw(game_state);

        if (IsKeyPressed(KEY_SPACE)) {
            Wave wave{
                .frameCount = SPRING_AUDIO_FRAME_COUNT,
                .sampleRate = SPRING_AUDIO_SAMPLE_RATE,
                .sampleSize = SPRING_AUDIO_SAMPLE_SIZE,
                .channels = SPRING_AUDIO_CHANNELS,
                .data = SPRING_AUDIO_DATA
            };
            audioManager.playRawAudio("spring-effect", wave);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// Update game logic based on the current state
void update(GameState& game_state) {
    switch (game_state.activity_state) {
        case GameActivityState::MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                game_state.activity_state = GameActivityState::PLAYING;
            }
        break;
        case GameActivityState::PLAYING:
            if (IsKeyPressed(KEY_P)) {
                game_state.activity_state = GameActivityState::PAUSED;
            }
        break;
        case GameActivityState::PAUSED:
            if (IsKeyPressed(KEY_R)) {
                game_state.activity_state = GameActivityState::PLAYING;
            }
        break;
        case GameActivityState::LOADING:
            // Simulate loading or transition logic
                game_state.activity_state = GameActivityState::MENU;
        break;
        case GameActivityState::SETTINGS:
            if (IsKeyPressed(KEY_TAB)) {
                game_state.activity_state = GameActivityState::MENU;
            }
        break;
        case GameActivityState::GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) {
                game_state.activity_state = GameActivityState::MENU;
            }
        break;
        default:
            game_state.activity_state = GameActivityState::MENU;
        break;
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
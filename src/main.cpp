//
// Created by codingwithjamal on 1/1/2025.
//

#include "main.hpp"

int main() {
    InitWindow(Config::WindowWidth, Config::WindowHeight, Config::WindowTitle);
    SetTargetFPS(Config::FPS);
    // SetExitKey(0);

    TextureResourceManager textureManager;
    textureManager.buildTextureHeaders();

    AudioResourceManager audioManager;
    audioManager.buildAudioHeaders();

    GameState game_state{
        .activity_state = GameActivityState::MENU,
    };

    Game game(game_state, audioManager, textureManager);

    Logger& logger = Logger::getInstance();

    bool exitTriggered = false;
    logger.log(LogLevel::INFO, "Starting game...");

    while (!WindowShouldClose() && !exitTriggered) {
        BeginDrawing();
        ClearBackground(GetColor(0x052c46ff));

        switch (game_state.activity_state) {
            case GameActivityState::MENU:
                game.draw_menu();
            break;

            case GameActivityState::PLAYING:
                game.update();
                game.draw();
            break;

            case GameActivityState::GAME_OVER:
                game.draw_game_over();
            break;

            case GameActivityState::SETTINGS:
                // todo
            break;

            case GameActivityState::PAUSED:
                // todo
            break;

            case GameActivityState::LOADING:
                // todo
            break;

            case GameActivityState::EXIT:
                exitTriggered = true;
            break;

            default:
                DrawText("Unknown State", Config::WindowWidth / 2 - 50, Config::WindowHeight / 2, 20, RED);
            break;
        }

        EndDrawing();
    }

    CloseWindow();

    logger.log(LogLevel::INFO, "Game ended.");

    return 0;
}

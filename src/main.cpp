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

    logger.log(LogLevel::DEBUG, "Default Speed: " + std::to_string(GlobalVariables::defaultSpeed));
    logger.log(LogLevel::DEBUG, "Default Position: " + std::to_string(GlobalVariables::defaultPosition.x) + ", " + std::to_string(GlobalVariables::defaultPosition.y));
    logger.log(LogLevel::DEBUG, "Window Width: " + std::to_string(Config::WindowWidth));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x052c46ff));

        switch (game_state.activity_state) {
            case GameActivityState::MENU:
                game.draw_menu();
                if (IsKeyPressed(KEY_ENTER)) {
                    game_state.activity_state = GameActivityState::PLAYING;
                    game.reset_game(); // Ensure game state is initialized when starting
                }
            break;

            case GameActivityState::PLAYING:
                game.update();
                game.draw();
            break;

            case GameActivityState::GAME_OVER:
                game.draw_game_over();
                if (IsKeyPressed(KEY_ENTER)) {
                    game_state.activity_state = GameActivityState::MENU;
                    game.reset_game();
                }
            break;

            default:
                DrawText("Unknown State", Config::WindowWidth / 2 - 50, Config::WindowHeight / 2, 20, RED);
            break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

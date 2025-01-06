//
// Created by codingwithjamal on 1/1/2025.
//

#include "main.h"

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
        .pipePassed = false,
        .score = 0,
        .playerSpeed = GlobalVariables::defaultSpeed,
        .playerPosition = GlobalVariables::defaultPosition,
    };

    Game game(game_state, audioManager, textureManager);

    std::cout << "Default Speed: " << GlobalVariables::defaultSpeed << std::endl;
    std::cout << "Default Position: " << GlobalVariables::defaultPosition.x << ", " << GlobalVariables::defaultPosition.y << std::endl;
    std::cout << "Window Width: " << Config::WindowWidth << ", Window Height: " << Config::WindowHeight << std::endl;


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

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
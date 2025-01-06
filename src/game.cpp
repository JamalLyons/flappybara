//
// Created by codingwithjamal on 1/3/2025.
//

#include "game.h"
#include <cstdlib> // For rand() and srand()
#include <random> // For std::mt19937 and std::uniform_int_distribution

void run_playing(GameState &game_state, AudioResourceManager &audioManager) {
    game_state.playerSpeed += GlobalVariables::gravity * GetFrameTime();
    game_state.playerPosition.y += game_state.playerSpeed * GetFrameTime();

    if (IsKeyPressed(KEY_SPACE)) {
        game_state.playerSpeed = GlobalVariables::jumpHeight;
        audioManager.playAudio("spring-effect");
    }

    if (game_state.playerPosition.y > Config::WindowHeight || game_state.playerPosition.y < 0 || game_state.playerPosition.x > Config::WindowWidth) {
        game_state.activity_state = GameActivityState::GAME_OVER;
    }

    // Seed random for pipe gap positions
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(0, RAND_MAX);
    srand(dist(mt));

    // Move pipes
    GlobalVariables::pipes[0].x -= GlobalVariables::pipeSpeed  * GetFrameTime();
    GlobalVariables::pipes[1].x -= GlobalVariables::pipeSpeed  * GetFrameTime();

    // Reset pipes when off-screen
    if (GlobalVariables::pipes[0].x + GlobalVariables::pipeWidth < 0) {
        GlobalVariables::pipes[0].x = Config::WindowWidth;
        GlobalVariables::pipes[1].x = Config::WindowWidth;
        const auto randomHeight = static_cast<float>(GetRandomValue(50, Config::WindowHeight - GlobalVariables::pipeGap - 50));
        GlobalVariables::pipes[0].height = randomHeight;
        GlobalVariables::pipes[1].y = randomHeight + GlobalVariables::pipeGap;
        GlobalVariables::pipes[1].height = Config::WindowHeight - GlobalVariables::pipes[1].y;
        game_state.pipePassed = false;
    }

    // Check if player has passed a pipe
    if (!game_state.pipePassed && GlobalVariables::pipes[0].x + GlobalVariables::pipeWidth < game_state.playerPosition.x) {
        game_state.score++;
        game_state.pipePassed = true; // Prevents multiple increments for the same pipe
        audioManager.playAudio("score");
    }

    // Collision detection
    if (CheckCollisionCircleRec(game_state.playerPosition, 20, GlobalVariables::pipes[0]) ||
        CheckCollisionCircleRec(game_state.playerPosition, 20, GlobalVariables::pipes[1]) ||
        game_state.playerPosition.y > Config::WindowHeight ||
        game_state.playerPosition.y < 0) {

        audioManager.playAudio("game-over");
        game_state.score = 0;
        game_state.activity_state = GameActivityState::GAME_OVER;
    }
}

void draw_playing(GameState &game_state) {
    DrawCircleV(game_state.playerPosition, 20, BLUE);

    DrawRectangleRec(GlobalVariables::pipes[0], GREEN);
    DrawRectangleRec(GlobalVariables::pipes[1], GREEN);

    // Draw score
    DrawText(TextFormat("Score: %d", game_state.score), 10, 10, 20, WHITE);
}

void reset_game(GameState &game_state) {
    game_state.playerSpeed = GlobalVariables::defaultSpeed;
    game_state.playerPosition = GlobalVariables::defaultPosition;

    // Reset pipes when game over
    GlobalVariables::pipes[0] =
        { Config::WindowWidth, 0, GlobalVariables::pipeWidth, 200 },
    GlobalVariables::pipes[1] =
        { Config::WindowWidth, 200 + GlobalVariables::pipeGap, GlobalVariables::pipeWidth, Config::WindowHeight - 200 - GlobalVariables::pipeGap };
}
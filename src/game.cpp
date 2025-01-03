//
// Created by codingwithjamal on 1/3/2025.
//

#include "game.h"

void run_playing(GameState &game_state, AudioResourceManager &audioManager) {
    game_state.playerSpeed += GlobalVariables::gravity * GetFrameTime();
    game_state.playerPosition.y += game_state.playerSpeed * GetFrameTime();

    if (IsKeyPressed(KEY_SPACE)) {
        game_state.playerSpeed = GlobalVariables::jumpHeight;

        constexpr Wave wave{
            .frameCount = SPRING_AUDIO_FRAME_COUNT,
            .sampleRate = SPRING_AUDIO_SAMPLE_RATE,
            .sampleSize = SPRING_AUDIO_SAMPLE_SIZE,
            .channels = SPRING_AUDIO_CHANNELS,
            .data = SPRING_AUDIO_DATA
        };
        audioManager.playRawAudio("spring-effect", wave);
    }

    if (game_state.playerPosition.y > Config::WindowHeight || game_state.playerPosition.y < 0 || game_state.playerPosition.x > Config::WindowWidth) {
        game_state.activity_state = GameActivityState::GAME_OVER;
    }
}

void draw_playing(const GameState &game_state) {
    DrawCircleV(game_state.playerPosition, 20, BLUE);
}

void reset_game(GameState &game_state) {
    game_state.playerSpeed = GlobalVariables::defaultSpeed;
    game_state.playerPosition = GlobalVariables::defaultPosition;
}
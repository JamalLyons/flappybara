//
// Created by codingwithjamal on 1/1/2025.
//

#ifndef MAIN_H
#define MAIN_H

#include "AudioResourceManager.h"
#include "constants.h"
#include "raylib.h"

struct GameState {
    GameActivityState activity_state;   // The current state of the game (playing, paused, etc.)
    bool pipePassed;                    // If the player has passed a pipe
    int score;                          // The current score of the player
    float playerSpeed;                  // The speed of the player in pixels per second
    Vector2 playerPosition;             // The position of the player in pixels
};

void update(GameState& game_state, AudioResourceManager& audioManager);
void draw(const GameState& game_state);

#endif //MAIN_H

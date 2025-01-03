//
// Created by codingwithjamal on 1/1/2025.
//

#ifndef MAIN_H
#define MAIN_H

#include "AudioResourceManager.h"
#include "constants.h"

struct GameState {
    GameActivityState activity_state;

    int score;
    float playerSpeed;
    Vector2 playerPosition;
};

void update(GameState& game_state, AudioResourceManager& audioManager);
void draw(const GameState& game_state);

#endif //MAIN_H

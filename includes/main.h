//
// Created by codingwithjamal on 1/1/2025.
//

#ifndef MAIN_H
#define MAIN_H

#include "constants.h"

struct GameState {
    GameActivityState activity_state;
};

void update(GameState& game_state);
void draw(const GameState& game_state);

#endif //MAIN_H

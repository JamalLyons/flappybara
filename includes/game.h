//
// Created by codingwithjamal on 1/3/2025.
//

#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "main.h"
#include "raylib.h"
#include "AudioResourceManager.h"

#include "../resources/audio/headers/spring_audio.h"

void run_playing(GameState &game_state, AudioResourceManager &audioManager);
void draw_playing(const GameState &game_state);
void reset_game(GameState &game_state);

#endif //GAME_H

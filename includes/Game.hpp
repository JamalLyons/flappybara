//
// Created by codingwithjamal on 1/3/2025.
//

#pragma once

#include "AudioResourceManager.hpp"
#include "TextureResourceManager.hpp"
#include "constants.hpp"

enum class GameActivityState {
    MENU,
    PLAYING,
    PAUSED,
    LOADING,
    SETTINGS,
    GAME_OVER,
    EXIT
};

struct GameState {
    GameActivityState activity_state;   // The current state of the game (playing, paused, etc.)
};

class Game {
public:
    Game(GameState &game_state, AudioResourceManager &audioManager, TextureResourceManager &textureManager);
    ~Game();

    void update();

    void draw();

    void draw_menu();

    void draw_game_over();

    void reset_game();

private:
    GameState &game_state;
    AudioResourceManager &audioManager;
    TextureResourceManager &textureManager;

    Rectangle m_pipes[2] = {
        { Config::WindowWidth, 0, m_pipeWidth, 200 },
        { Config::WindowWidth, 200 + m_pipeGap, m_pipeWidth, Config::WindowHeight - 200 - m_pipeGap },
    };

    float m_playerWidth = 70.0f;
    float m_playerHeight = 70.0f;
    float m_playerSpeed;                  // The speed of the player in pixels per second
    Vector2 m_playerPosition;             // The position of the player in pixels
    bool m_pipePassed;                    // If the player has passed a pipe
    int m_score;                          // The current score of the player
    int m_gameOverScore;                  // The score at which the game is over

    float m_pipeWidth = 80.0f;
    float m_pipeGap = 150.f;
    float m_pipeSpeed = 200.f;

    const float m_gravity = 400.0f;      // pixels per second ^ 2
    const float m_jumpHeight = -250.0f;
};
//
// Created by codingwithjamal on 1/3/2025.
//

#include "game.h"
#include <random>

Game::Game(GameState &game_state, AudioResourceManager &audioManager, TextureResourceManager &textureManager)
    : game_state(game_state), audioManager(audioManager), textureManager(textureManager) {}

Game::~Game() = default;

void Game::update() {
    // Apply gravity to player
    this->game_state.playerSpeed += this->m_gravity * GetFrameTime();
    this->game_state.playerPosition.y += this->game_state.playerSpeed * GetFrameTime();

    // Jump if space is pressed
    if (IsKeyPressed(KEY_SPACE)) {
        this->game_state.playerSpeed = m_jumpHeight;
        this->audioManager.playAudio("spring-effect");
    }

    // Check if player has hit world boundaries
    if (this->game_state.playerPosition.y > Config::WindowHeight || this->game_state.playerPosition.y < 0 || this->game_state.playerPosition.x > Config::WindowWidth) {
        this->game_state.activity_state = GameActivityState::GAME_OVER;
        this->audioManager.playAudio("game-over");
        return;
    }

    // Move pipes
    this->m_pipes[0].x -= this->m_pipeSpeed  * GetFrameTime();
    this->m_pipes[1].x -= this->m_pipeSpeed  * GetFrameTime();

    // Reset pipes when off-screen
    if (m_pipes[0].x + this->m_pipeWidth < 0) {
        // Seed random for pipe gap positions
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution dist(50.0f, Config::WindowHeight - this->m_pipeGap - 50.0f);
        const auto randomHeight = dist(mt);

        this->m_pipes[0].x = Config::WindowWidth;
        this->m_pipes[1].x = Config::WindowWidth;
        this->m_pipes[0].height = randomHeight;
        this->m_pipes[1].y = randomHeight + this->m_pipeGap;
        this->m_pipes[1].height = Config::WindowHeight - this->m_pipes[1].y;
        this->game_state.pipePassed = false;
    }

    // Check if player has passed a pipe
    if (!this->game_state.pipePassed && this->m_pipes[0].x + this->m_pipeWidth < this->game_state.playerPosition.x) {
        this->game_state.score++;
        this->game_state.pipePassed = true; // Prevents multiple increments for the same pipe
        this->audioManager.playAudio("score");
    }

    // Collision detection
    if (CheckCollisionCircleRec(this->game_state.playerPosition, 20, this->m_pipes[0]) ||
        CheckCollisionCircleRec(this->game_state.playerPosition, 20, this->m_pipes[1]) ||
        this->game_state.playerPosition.y > Config::WindowHeight ||
        this->game_state.playerPosition.y < 0) {

        this->audioManager.playAudio("game-over");
        this->game_state.score = 0;
        this->game_state.activity_state = GameActivityState::GAME_OVER;
    }
}

void Game::reset_game() {
    this->game_state.playerSpeed = GlobalVariables::defaultSpeed;
    this->game_state.playerPosition = GlobalVariables::defaultPosition;
    this->game_state.pipePassed = false;
    this->game_state.score = 0;

    // Reset pipes when game over
    this->m_pipes[0] = { Config::WindowWidth, 0, m_pipeWidth, 200 };
    this->m_pipes[1] =
        { Config::WindowWidth, 200 + m_pipeGap, m_pipeWidth, Config::WindowHeight - 200 - m_pipeGap };
}

void Game::draw() {
    Texture2D background = this->textureManager.getTexture("background-day");

    DrawRectangleRec(m_pipes[0], GREEN);
    DrawRectangleRec(m_pipes[1], GREEN);

    DrawCircleV(this->game_state.playerPosition, 20, BLUE);

    DrawText(TextFormat("Player Y: %.2f", this->game_state.playerPosition.y), 10, 30, 20, WHITE);
    DrawText(TextFormat("Player Speed: %.2f", this->game_state.playerSpeed), 10, 50, 20, WHITE);
    DrawText(TextFormat("Pipe 1 X: %.2f, Height: %.2f", m_pipes[0].x, m_pipes[0].height), 10, 70, 20, WHITE);
    DrawText(TextFormat("Pipe 2 X: %.2f, Y: %.2f, Height: %.2f", m_pipes[1].x, m_pipes[1].y, m_pipes[1].height), 10, 90, 20, WHITE);
    DrawText(TextFormat("Score: %d", this->game_state.score), 10, 10, 20, WHITE);

    DrawTexture(background, 0, 0, WHITE);
}

void Game::draw_menu() {
    DrawText("Start Menu - Press ENTER to Play", Config::WindowWidth / 2 - 200, Config::WindowHeight / 2, 20, WHITE);
}

void Game::draw_game_over() {
    DrawText("Game Over - Press ENTER to Restart", Config::WindowWidth / 2 - 150, Config::WindowHeight / 2, 20, WHITE);
}
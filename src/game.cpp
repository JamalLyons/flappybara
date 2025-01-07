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

    // Floor collision
    const float floorHeight = static_cast<float>(GetScreenHeight()) * 0.1f; // 10% of screen height
    const float floorY = static_cast<float>(GetScreenHeight()) - floorHeight;

    if (this->game_state.playerPosition.y + 20 > floorY) { // Player radius is 20
        this->game_state.activity_state = GameActivityState::GAME_OVER;
        this->audioManager.playAudio("game-over");
        return;
    }

    // Check if player has hit world boundaries
    if (this->game_state.playerPosition.y < 0 || this->game_state.playerPosition.x > Config::WindowWidth) {
        this->game_state.activity_state = GameActivityState::GAME_OVER;
        this->audioManager.playAudio("game-over");
        return;
    }

    // Move pipes
    this->m_pipes[0].x -= this->m_pipeSpeed * GetFrameTime();
    this->m_pipes[1].x -= this->m_pipeSpeed * GetFrameTime();

    // Reset pipes when off-screen
    if (m_pipes[0].x + this->m_pipeWidth < 0) {
        // Seed random for pipe gap positions
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution dist(50.0f, floorY - this->m_pipeGap - 50.0f);
        const auto randomHeight = dist(mt);

        this->m_pipes[0].x = Config::WindowWidth;
        this->m_pipes[1].x = Config::WindowWidth;
        this->m_pipes[0].height = randomHeight;
        this->m_pipes[1].y = randomHeight + this->m_pipeGap;
        this->m_pipes[1].height = floorY - this->m_pipes[1].y;
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
        CheckCollisionCircleRec(this->game_state.playerPosition, 20, this->m_pipes[1])) {
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
    const Texture2D background = this->textureManager.getTexture("background-day");
    const Texture2D pipe = this->textureManager.getTexture("pipe-green");
    const Texture2D floor = this->textureManager.getTexture("floor");
    const Texture2D player = this->textureManager.getTexture("player");

    // Define the source rectangle (full texture)
    const Rectangle source = { 0.0f, 0.0f, static_cast<float>(background.width), static_cast<float>(background.height) };

    // Define the destination rectangle (screen dimensions)
    const Rectangle dest = { 0.0f, 0.0f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    // Origin point for rotation (not used here, so set to (0,0))
    constexpr Vector2 origin = { 0.0f, 0.0f };

    // Draw the background texture scaled to fit the screen
    DrawTexturePro(background, source, dest, origin, 0.0f, WHITE);

    // Draw the pipes
    DrawTexturePro(pipe, source, m_pipes[0], origin, 0.0f, WHITE);
    DrawTexturePro(pipe, source, m_pipes[1], origin, 0.0f, WHITE);

    // Adjust the scale factor to fit the height of the floor
    const float floorScale = static_cast<float>(GetScreenHeight()) * 0.1f / floor.height; // Floor height 10% of screen height
    const float floorHeight = floor.height * floorScale;

    // Calculate the number of tiles needed to fill the screen width
    const int tiles = static_cast<int>(std::ceil(static_cast<float>(GetScreenWidth()) / (floor.width * floorScale)));

    // Position at the bottom of the screen
    const float floorY = static_cast<float>(GetScreenHeight()) - floorHeight;

    // Draw the floor texture tiled across the bottom
    for (int i = 0; i < tiles; ++i) {
        const float floorX = i * floor.width * floorScale; // X position for each tile
        DrawTextureEx(floor, { floorX, floorY }, 0.0f, floorScale, WHITE);
    }

    // Define the source rectangle for the player (full player texture)
    const Rectangle playerSource = { 0.0f, 0.0f, static_cast<float>(player.width), static_cast<float>(player.height) };

    // Define the destination rectangle for the player (position and size on the screen)
    const Rectangle playerDest = {
        this->game_state.playerPosition.x,
        this->game_state.playerPosition.y,
        70.0f, // Width of the player on the screen
        70.0f  // Height of the player on the screen
    };

    // Draw the player texture
    DrawTexturePro(player, playerSource, playerDest, origin, 0.0f, WHITE);


    DrawText(TextFormat("Player Y: %.2f", this->game_state.playerPosition.y), 10, 30, 20, WHITE);
    DrawText(TextFormat("Player Speed: %.2f", this->game_state.playerSpeed), 10, 50, 20, WHITE);
    DrawText(TextFormat("Pipe 1 X: %.2f, Height: %.2f", m_pipes[0].x, m_pipes[0].height), 10, 70, 20, WHITE);
    DrawText(TextFormat("Pipe 2 X: %.2f, Y: %.2f, Height: %.2f", m_pipes[1].x, m_pipes[1].y, m_pipes[1].height), 10, 90, 20, WHITE);
    DrawText(TextFormat("Score: %d", this->game_state.score), 10, 10, 20, WHITE);
}

void Game::draw_menu() {
    DrawText("Start Menu - Press ENTER to Play", Config::WindowWidth / 2 - 200, Config::WindowHeight / 2, 20, WHITE);
}

void Game::draw_game_over() {
    DrawText("Game Over - Press ENTER to Restart", Config::WindowWidth / 2 - 150, Config::WindowHeight / 2, 20, WHITE);
}
//
// Created by codingwithjamal on 1/3/2025.
//

#include "Game.hpp"
#include <random>

Game::Game(GameState &game_state, AudioResourceManager &audioManager, TextureResourceManager &textureManager)
    : game_state(game_state), audioManager(audioManager), textureManager(textureManager) {

    Logger& logger = Logger::getInstance();

    this->m_playerPosition = GlobalVariables::defaultPosition;
    this->m_playerSpeed = GlobalVariables::defaultSpeed;
    this->m_score = 0;
    this->m_pipePassed = false;

    logger.log(LogLevel::INFO, "Game initialized with default player position and speed.");
}

Game::~Game() = default;

void Game::update() {
    Logger& logger = Logger::getInstance();

    // Apply gravity to player
    this->m_playerSpeed += this->m_gravity * GetFrameTime();
    this->m_playerPosition.y += this->m_playerSpeed * GetFrameTime();

    // logger.log(LogLevel::DEBUG, "Player position updated: Y = " + std::to_string(this->m_playerPosition.y));
    // logger.log(LogLevel::DEBUG, "Player speed updated: Speed = " + std::to_string(this->m_playerSpeed));

    // Jump if space is pressed
    if (IsKeyPressed(KEY_SPACE)) {
        this->m_playerSpeed = m_jumpHeight;
        this->audioManager.playAudio("spring-effect");
        logger.log(LogLevel::INFO, "Player jumped. Current speed: " + std::to_string(this->m_playerSpeed));
    }

    // Floor collision
    const float floorHeight = static_cast<float>(GetScreenHeight()) * 0.1f; // 10% of screen height
    const float floorY = static_cast<float>(GetScreenHeight()) - floorHeight;

    if (this->m_playerPosition.y + this->m_playerHeight >= floorY) {
        this->game_state.activity_state = GameActivityState::GAME_OVER;
        this->audioManager.playAudio("game-over");
        logger.log(LogLevel::INFO, "Player collided with the floor. Game over.");
        return;
    }

    // Check if player has hit world boundaries
    if (this->m_playerPosition.y < 0 || this->m_playerPosition.x > Config::WindowWidth) {
        this->game_state.activity_state = GameActivityState::GAME_OVER;
        this->audioManager.playAudio("game-over");
        logger.log(LogLevel::INFO, "Player hit world boundaries. Game over.");
        return;
    }

    // Move pipes
    this->m_pipes[0].x -= this->m_pipeSpeed * GetFrameTime();
    this->m_pipes[1].x -= this->m_pipeSpeed * GetFrameTime();

    // logger.log(LogLevel::DEBUG, "Pipe positions updated: Pipe1 X = " + std::to_string(this->m_pipes[0].x) + ", Pipe2 X = " + std::to_string(this->m_pipes[1].x));

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
        this->m_pipePassed = false;

        logger.log(LogLevel::INFO, "Pipes reset. New heights: Pipe1 Height = " + std::to_string(this->m_pipes[0].height) + ", Pipe2 Y = " + std::to_string(this->m_pipes[1].y));
    }

    // Check if player has passed a pipe
    if (!this->m_pipePassed && this->m_pipes[0].x + this->m_pipeWidth < this->m_playerPosition.x) {
        this->m_score++;
        this->m_pipePassed = true; // Prevents multiple increments for the same pipe
        this->audioManager.playAudio("score");

        logger.log(LogLevel::INFO, "Player passed a pipe. Score updated: " + std::to_string(this->m_score));
    }

    // Define player rectangle
    const Rectangle playerRect = {
        this->m_playerPosition.x,
        this->m_playerPosition.y,
        this->m_playerWidth,
        this->m_playerHeight
    };

    // Collision detection using CheckCollisionRecs
    if (CheckCollisionRecs(playerRect, this->m_pipes[0]) || CheckCollisionRecs(playerRect, this->m_pipes[1])) {
        this->audioManager.playAudio("game-over");
        this->m_score = 0;
        this->game_state.activity_state = GameActivityState::GAME_OVER;

        logger.log(LogLevel::INFO, "Collision detected with pipe. Game over.");
    }
}

void Game::reset_game() {
    Logger& logger = Logger::getInstance();

    this->m_playerSpeed = GlobalVariables::defaultSpeed;
    this->m_playerPosition = GlobalVariables::defaultPosition;
    this->m_pipePassed = false;
    this->m_score = 0;

    this->m_pipes[0] = { Config::WindowWidth, 0, m_pipeWidth, 200 };
    this->m_pipes[1] = { Config::WindowWidth, 200 + m_pipeGap, m_pipeWidth, Config::WindowHeight - 200 - m_pipeGap };

    logger.log(LogLevel::INFO, "Game reset to initial state.");
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
        this->m_playerPosition.x,
        this->m_playerPosition.y,
        70.0f, // Width of the player on the screen
        70.0f  // Height of the player on the screen
    };

    // Draw the player texture
    DrawTexturePro(player, playerSource, playerDest, origin, 0.0f, WHITE);


    DrawText(TextFormat("Player Y: %.2f", this->m_playerPosition.y), 10, 30, 20, WHITE);
    DrawText(TextFormat("Player Speed: %.2f", this->m_playerSpeed), 10, 50, 20, WHITE);
    DrawText(TextFormat("Pipe 1 X: %.2f, Height: %.2f", m_pipes[0].x, m_pipes[0].height), 10, 70, 20, WHITE);
    DrawText(TextFormat("Pipe 2 X: %.2f, Y: %.2f, Height: %.2f", m_pipes[1].x, m_pipes[1].y, m_pipes[1].height), 10, 90, 20, WHITE);
    DrawText(TextFormat("Score: %d", this->m_score), 10, 10, 20, WHITE);
}

void Game::draw_menu() {
    DrawText("Start Menu - Press ENTER to Play", Config::WindowWidth / 2 - 200, Config::WindowHeight / 2, 20, WHITE);
}

void Game::draw_game_over() {
    DrawText("Game Over - Press ENTER to Restart", Config::WindowWidth / 2 - 150, Config::WindowHeight / 2, 20, WHITE);
}
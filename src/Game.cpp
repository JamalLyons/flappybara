//
// Created by codingwithjamal on 1/3/2025.
//

#include "Game.hpp"
#include <random>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Game::Game(GameState &game_state, AudioResourceManager &audioManager, TextureResourceManager &textureManager)
    : game_state(game_state), audioManager(audioManager), textureManager(textureManager) {

    Logger& logger = Logger::getInstance();

    m_playerPosition = GlobalVariables::defaultPosition;
    m_playerSpeed = GlobalVariables::defaultSpeed;
    m_score = 0;
    m_gameOverScore = 0;
    m_pipePassed = false;

    logger.log(LogLevel::INFO, "Game initialized with default player position and speed.");
}

Game::~Game() = default;

void Game::update() {
    Logger& logger = Logger::getInstance();

    // Apply gravity to player
    m_playerSpeed += m_gravity * GetFrameTime();
    m_playerPosition.y += m_playerSpeed * GetFrameTime();

    // logger.log(LogLevel::DEBUG, "Player position updated: Y = " + std::to_string(m_playerPosition.y));
    // logger.log(LogLevel::DEBUG, "Player speed updated: Speed = " + std::to_string(m_playerSpeed));

    // Jump if space is pressed
    if (IsKeyPressed(KEY_SPACE)) {
        m_playerSpeed = m_jumpHeight;
        // audioManager.playAudio("spring-effect"); // its kinda annoying lol
        logger.log(LogLevel::INFO, "Player jumped. Current speed: " + std::to_string(m_playerSpeed));
    }

    // Floor collision
    const float floorHeight = static_cast<float>(GetScreenHeight()) * 0.1f; // 10% of screen height
    const float floorY = static_cast<float>(GetScreenHeight()) - floorHeight;

    if (m_playerPosition.y + m_playerHeight >= floorY) {
        game_state.activity_state = GameActivityState::GAME_OVER;
        audioManager.playAudio("game-over");
        logger.log(LogLevel::INFO, "Player collided with the floor. Game over.");
        return;
    }

    // Check if player has hit world boundaries
    if (m_playerPosition.y < 0 || m_playerPosition.x > Config::WindowWidth) {
        game_state.activity_state = GameActivityState::GAME_OVER;
        audioManager.playAudio("game-over");
        logger.log(LogLevel::INFO, "Player hit world boundaries. Game over.");
        return;
    }

    // Move pipes
    m_pipes[0].x -= m_pipeSpeed * GetFrameTime();
    m_pipes[1].x -= m_pipeSpeed * GetFrameTime();

    // logger.log(LogLevel::DEBUG, "Pipe positions updated: Pipe1 X = " + std::to_string(m_pipes[0].x) + ", Pipe2 X = " + std::to_string(m_pipes[1].x));

    // Reset pipes when off-screen
    if (m_pipes[0].x + m_pipeWidth < 0) {
        // Seed random for pipe gap positions
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution dist(50.0f, floorY - m_pipeGap - 50.0f);
        const auto randomHeight = dist(mt);

        m_pipes[0].x = Config::WindowWidth;
        m_pipes[1].x = Config::WindowWidth;
        m_pipes[0].height = randomHeight;
        m_pipes[1].y = randomHeight + m_pipeGap;
        m_pipes[1].height = floorY - m_pipes[1].y;
        m_pipePassed = false;

        logger.log(LogLevel::INFO, "Pipes reset. New heights: Pipe1 Height = " + std::to_string(m_pipes[0].height) + ", Pipe2 Y = " + std::to_string(m_pipes[1].y));
    }

    // Check if player has passed a pipe
    if (!m_pipePassed && m_pipes[0].x + m_pipeWidth < m_playerPosition.x) {
        m_score++;
        m_pipePassed = true; // Prevents multiple increments for the same pipe
        audioManager.playAudio("score");

        logger.log(LogLevel::INFO, "Player passed a pipe. Score updated: " + std::to_string(m_score));
    }

    // Define player rectangle
    const Rectangle playerRect = {
        m_playerPosition.x,
        m_playerPosition.y,
        m_playerWidth,
        m_playerHeight
    };

    // Collision detection using CheckCollisionRecs
    if (CheckCollisionRecs(playerRect, m_pipes[0]) || CheckCollisionRecs(playerRect, m_pipes[1])) {
        audioManager.playAudio("game-over");
        game_state.activity_state = GameActivityState::GAME_OVER;
        m_gameOverScore = m_score;

        logger.log(LogLevel::INFO, "Collision detected with pipe. Game over.");
    }
}

void Game::reset_game() {
    Logger& logger = Logger::getInstance();

    m_playerSpeed = GlobalVariables::defaultSpeed;
    m_playerPosition = GlobalVariables::defaultPosition;
    m_pipePassed = false;
    m_score = 0;
    m_gameOverScore = 0;

    m_pipes[0] = { Config::WindowWidth, 0, m_pipeWidth, 200 };
    m_pipes[1] = { Config::WindowWidth, 200 + m_pipeGap, m_pipeWidth, Config::WindowHeight - 200 - m_pipeGap };

    logger.log(LogLevel::INFO, "Game reset to initial state.");
}

void Game::draw() {
    const Texture2D background = textureManager.getTexture("background-day");
    const Texture2D pipe = textureManager.getTexture("pipe-green");
    const Texture2D floor = textureManager.getTexture("floor");
    const Texture2D player = textureManager.getTexture("player");

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
    const float floorScale = static_cast<float>(GetScreenHeight()) * 0.1f / static_cast<float>(floor.height); // Floor height 10% of screen height
    const float floorHeight = static_cast<float>(floor.height) * floorScale;

    // Calculate the number of tiles needed to fill the screen width
    const int tiles = static_cast<int>(std::ceil(static_cast<float>(GetScreenWidth()) / (static_cast<float>(floor.width) * floorScale)));

    // Position at the bottom of the screen
    const float floorY = static_cast<float>(GetScreenHeight()) - floorHeight;

    // Draw the floor texture tiled across the bottom
    for (int i = 0; i < tiles; ++i) {
        const float floorX = static_cast<float>(i) * static_cast<float>(floor.width) * floorScale; // X position for each tile
        DrawTextureEx(floor, { floorX, floorY }, 0.0f, floorScale, WHITE);
    }

    // Define the source rectangle for the player (full player texture)
    const Rectangle playerSource = { 0.0f, 0.0f, static_cast<float>(player.width), static_cast<float>(player.height) };

    // Define the destination rectangle for the player (position and size on the screen)
    const Rectangle playerDest = {
        m_playerPosition.x,
        m_playerPosition.y,
        70.0f, // Width of the player on the screen
        70.0f  // Height of the player on the screen
    };

    // Draw the player texture
    DrawTexturePro(player, playerSource, playerDest, origin, 0.0f, WHITE);


    DrawText(TextFormat("Player Y: %.2f", m_playerPosition.y), 10, 30, 20, WHITE);
    DrawText(TextFormat("Player Speed: %.2f", m_playerSpeed), 10, 50, 20, WHITE);
    DrawText(TextFormat("Pipe 1 X: %.2f, Height: %.2f", m_pipes[0].x, m_pipes[0].height), 10, 70, 20, WHITE);
    DrawText(TextFormat("Pipe 2 X: %.2f, Y: %.2f, Height: %.2f", m_pipes[1].x, m_pipes[1].y, m_pipes[1].height), 10, 90, 20, WHITE);
    DrawText(TextFormat("Score: %d", m_score), 10, 10, 20, WHITE);
}

void Game::draw_menu() {
    DrawText("FlappyBara", Config::WindowWidth / 2 - 100, Config::WindowHeight / 4 - 100, 40, WHITE);

    // Play Button
    constexpr Rectangle playButton = { static_cast<float>(Config::WindowWidth) / 2.0f - 75.0f, static_cast<float>(Config::WindowHeight) / 2.0f - 50.0f, 150.0f, 50.0f };
    if (GuiButton(playButton, "Play")) {
        game_state.activity_state = GameActivityState::PLAYING;
        reset_game(); // Ensure game state is initialized when starting
    }

    constexpr Rectangle settingsButton = { playButton.x, playButton.y + 80.0f, playButton.width, playButton.height };
    if (GuiButton(settingsButton, "Settings")) {
        game_state.activity_state = GameActivityState::SETTINGS; // Open settings
    }

    constexpr Rectangle exitButton = { playButton.x, playButton.y + 160.0f, playButton.width, playButton.height };
    if (GuiButton(exitButton, "Exit")) {
        game_state.activity_state = GameActivityState::EXIT;
    }
}

void Game::draw_game_over() {
    // Game Over Text
    DrawText("Game Over", Config::WindowWidth / 2 - 100, Config::WindowHeight / 2 - 60, 40, WHITE);

    // The player's Score
    DrawText(TextFormat("Your Score: %d", m_gameOverScore), Config::WindowWidth / 2 - 100, Config::WindowHeight / 2, 25, WHITE);

    // Back to Menu Button
    constexpr float buttonWidth = 150.0f;
    constexpr float buttonHeight = 50.0f;
    const Rectangle menuButton = { static_cast<float>(Config::WindowWidth) / 2.0f - buttonWidth / 2.0f, static_cast<float>(Config::WindowHeight) / 2.0f + 60.0f, buttonWidth, buttonHeight };

    if (GuiButton(menuButton, "Back")) {
        reset_game();
        game_state.activity_state = GameActivityState::MENU;
    }
}
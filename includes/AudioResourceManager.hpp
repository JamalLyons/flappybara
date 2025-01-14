//
// Created by codingwithjamal on 1/2/2025.
//

#pragma once

#include <string>
#include <unordered_map>
#include <raylib.h>
#include "Logger.hpp"

class AudioResourceManager {
public:
    // Constructor: Initializes the audio device
    AudioResourceManager();

    // Destructor: Cleans up all loaded resources and closes the audio device
    ~AudioResourceManager();

    // Play a sound resource
    void playAudio(const std::string &key);

    // Play a raw sound resource
    // Used mainly for playing audio header files
    void playRawAudio(const std::string &key, const Wave &wave);

    // Stop a sound resource
    void stopAudio(const std::string &key);

    // Unload a specific sound resource
    void unloadAudio(const std::string &key);

    // Load predefined sound resources
    void loadAudioResources();

    // Unload all loaded sound resources
    void unloadAllAudio();

    // A utility function to build all audio headers from wave files
    // This is used so sound resources can be embedded in the final executable without having to load them at runtime.
    // The function can be enabled in the constants.h file.
    //
    // This function is meant to be run in development only and should not be enabled in release builds.
    void buildAudioHeaders();

    Music getBackgroundMusicRef() const;

    void playBackgroundMusic();

private:
    // Created in loadAudioResources() method
    Music background_game_music;

    // Audio sound cache
    std::unordered_map<std::string, Sound> audioResources;
    std::unordered_map<std::string, std::string> predefinedAudioPaths = {
        // Sound effects
        {"spring-effect", "../resources/audio/spring.wav"},
        {"game-over", "../resources/audio/game_over.wav"},
        {"level-complete", "../resources/audio/level_complete.wav"},
        {"score", "../resources/audio/score.wav"},
        {"theme-song", "../resources/audio/capybara_song.wav"},
    };
};
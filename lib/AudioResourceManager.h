//
// Created by codingwithjamal on 1/2/2025.
//

#ifndef AUDIORESOURCEMANAGER_H
#define AUDIORESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include "raylib.h"

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

private:
    // Audio sound cache
    std::unordered_map<std::string, Sound> audioResources;
    std::unordered_map<std::string, std::string> predefinedAudioPaths = {
        // Sound effects
        {"spring-effect", "../resources/audio/spring.wav"},
        // Background music
        // {"menu-background-music", "../resources/audio/background-music.wav"}
    };
};

#endif //AUDIORESOURCEMANAGER_H

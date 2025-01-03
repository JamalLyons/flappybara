#include "AudioResourceManager.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

#include "../includes/constants.h"

// Audio headers
#include "../resources/audio/headers/spring_audio.h"

AudioResourceManager::AudioResourceManager() {
    InitAudioDevice();
    loadAudioResources();
}

AudioResourceManager::~AudioResourceManager() {
    unloadAllAudio();
    CloseAudioDevice();
}

void AudioResourceManager::loadAudioResources() {
    constexpr Wave wave{
        .frameCount = SPRING_AUDIO_FRAME_COUNT,
        .sampleRate = SPRING_AUDIO_SAMPLE_RATE,
        .sampleSize = SPRING_AUDIO_SAMPLE_SIZE,
        .channels = SPRING_AUDIO_CHANNELS,
        .data = SPRING_AUDIO_DATA
    };

    audioResources["spring-effect"] = LoadSoundFromWave(wave);
}


void AudioResourceManager::playAudio(const std::string &key) {
    if (audioResources.contains(key)) {
        if (const Sound &sound = audioResources[key]; sound.stream.buffer != nullptr) {
            std::cout << "Playing audio: " << key << std::endl;
            PlaySound(sound);
        } else {
            std::cerr << "Error: Audio '" << key << "' is not valid!" << std::endl;
        }
    } else {
        std::cerr << "Error: Audio key '" << key << "' not found!" << std::endl;
    }
}

void AudioResourceManager::stopAudio(const std::string &key) {
    if (audioResources.contains(key)) {
        if (const Sound &sound = audioResources[key]; sound.stream.buffer != nullptr) {
            std::cout << "Stopping audio: " << key << std::endl;
            StopSound(sound);
        } else {
            std::cerr << "Error: Audio '" << key << "' is not valid!" << std::endl;
        }
    } else {
        std::cerr << "Error: Audio key '" << key << "' not found!" << std::endl;
    }
}


void AudioResourceManager::unloadAudio(const std::string &key) {
    if (audioResources.contains(key)) {
        UnloadSound(audioResources[key]);
        audioResources.erase(key);
    } else {
        std::cerr << "Error: Audio key '" << key << "' not found!" << std::endl;
    }
}

void AudioResourceManager::unloadAllAudio() {
    for (auto &[key, sound] : audioResources) {
        UnloadSound(sound);
    }
    audioResources.clear();
}

void AudioResourceManager::buildAudioHeaders() {
    if constexpr (!Config::buildAudioHeaders) {
        std::cout << "Audio headers building disabled." << std::endl;
        return;
    }

    std::cout << "Building audio headers..." << std::endl;

    const std::string outputDir = "../resources/audio/headers/";
    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directories(outputDir);
        std::cout << "Created output directory: " << outputDir << std::endl;
    }

    for (const auto &[key, path] : predefinedAudioPaths) {
        // Load the wave file
        const Wave wave = LoadWave(path.c_str());
        if (!wave.data) {
            std::cerr << "Error: Failed to load wave data: " << path << std::endl;
            continue;
        }

        // Generate a sanitized output header file name
        const std::string filename = path.substr(path.find_last_of("/\\") + 1);
        const std::string sanitizedFilename = filename.substr(0, filename.find_last_of('.')) + "_audio.h";
        const std::string outputPath = outputDir + sanitizedFilename;

        std::cout << "Building header: " << outputPath << std::endl;

        // Export the wave as a C header
        if (!ExportWaveAsCode(wave, outputPath.c_str())) {
            TraceLog(LOG_ERROR, "Failed to export wave as C header file: %s", outputPath.c_str());
        } else {
            TraceLog(LOG_INFO, "Wave exported as C header file successfully: %s", outputPath.c_str());
        }

        // Unload the wave to free memory
        UnloadWave(wave);
    }

    std::cout << "Finished building audio headers." << std::endl;
}

void AudioResourceManager::playRawAudio(const std::string &key, const Wave &wave) {
    if (!audioResources.contains(key)) {
        std::cout << "Caching loaded audio file: " << key << "" << std::endl;
        audioResources[key] = LoadSoundFromWave(wave);
    }

    PlaySound(audioResources[key]);
}

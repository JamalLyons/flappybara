//
// Created by codingwithjamal on 1/3/2025.
//


#include <iostream>
#include <filesystem>

#include "AudioResourceManager.hpp"
#include "constants.hpp"

// Audio headers
#include "../resources/audio/headers/spring_audio.h"
#include "../resources/audio/headers/game_over_audio.h"
#include "../resources/audio/headers/level_complete_audio.h"
#include "../resources/audio/headers/score_audio.h"
#include "../resources/audio/headers/capybara_song_audio.h"

AudioResourceManager::AudioResourceManager() {
    InitAudioDevice();
    loadAudioResources();
}

AudioResourceManager::~AudioResourceManager() {
    UnloadMusicStream(background_game_music);
    unloadAllAudio();
    CloseAudioDevice();
}

void AudioResourceManager::loadAudioResources() {
    Logger& logger = Logger::getInstance();
    logger.log(LogLevel::INFO, "Loading audio resources.");

    constexpr Wave spring_wave{
        .frameCount = SPRING_AUDIO_FRAME_COUNT,
        .sampleRate = SPRING_AUDIO_SAMPLE_RATE,
        .sampleSize = SPRING_AUDIO_SAMPLE_SIZE,
        .channels = SPRING_AUDIO_CHANNELS,
        .data = SPRING_AUDIO_DATA
    };

    constexpr Wave game_over_wave{
        .frameCount = GAME_OVER_AUDIO_FRAME_COUNT,
        .sampleRate = GAME_OVER_AUDIO_SAMPLE_RATE,
        .sampleSize = GAME_OVER_AUDIO_SAMPLE_SIZE,
        .channels = GAME_OVER_AUDIO_CHANNELS,
        .data = GAME_OVER_AUDIO_DATA
    };

    constexpr Wave level_complete_wave{
        .frameCount = LEVEL_COMPLETE_AUDIO_FRAME_COUNT,
        .sampleRate = LEVEL_COMPLETE_AUDIO_SAMPLE_RATE,
        .sampleSize = LEVEL_COMPLETE_AUDIO_SAMPLE_SIZE,
        .channels = LEVEL_COMPLETE_AUDIO_CHANNELS,
        .data = LEVEL_COMPLETE_AUDIO_DATA
    };

    constexpr Wave score_wave{
        .frameCount = SCORE_AUDIO_FRAME_COUNT,
        .sampleRate = SCORE_AUDIO_SAMPLE_RATE,
        .sampleSize = SCORE_AUDIO_SAMPLE_SIZE,
        .channels = SCORE_AUDIO_CHANNELS,
        .data = SCORE_AUDIO_DATA
    };

    audioResources["spring-effect"] = LoadSoundFromWave(spring_wave);
    audioResources["game-over"] = LoadSoundFromWave(game_over_wave);
    audioResources["level-complete"] = LoadSoundFromWave(level_complete_wave);
    audioResources["score"] = LoadSoundFromWave(score_wave);

    // background_game_music = LoadMusicStreamFromMemory(".wav", CAPYBARA_SONG_AUDIO_DATA, 2763776);
    background_game_music = LoadMusicStream("../resources/audio/capybara_song.wav");

    logger.log(LogLevel::INFO, "Audio resources loaded successfully.");
}


void AudioResourceManager::playAudio(const std::string &key) {
    Logger& logger = Logger::getInstance();

    if (Config::disableAudio) {
        logger.log(LogLevel::WARNING, "Audio disabled in config.");
        return;
    }

    if (audioResources.contains(key)) {
        if (const Sound &sound = audioResources[key]; sound.stream.buffer != nullptr) {
            logger.log(LogLevel::INFO, "Playing audio: " + key);
            PlaySound(sound);
        } else {
            logger.log(LogLevel::ERROR, "Error: Audio '" + key + "' is not valid!");
        }
    } else {
        logger.log(LogLevel::ERROR, "Error: Audio key '" + key + "' not found!");
    }
}

void AudioResourceManager::stopAudio(const std::string &key) {
    Logger& logger = Logger::getInstance();

    if (audioResources.contains(key)) {
        if (const Sound &sound = audioResources[key]; sound.stream.buffer != nullptr) {
            logger.log(LogLevel::INFO, "Stopping audio: " + key);
            StopSound(sound);
        } else {
            logger.log(LogLevel::ERROR, "Error: Audio '" + key + "' is not valid!");
        }
    } else {
        logger.log(LogLevel::ERROR, "Error: Audio key '" + key + "' not found!");
    }
}


void AudioResourceManager::unloadAudio(const std::string &key) {
    Logger& logger = Logger::getInstance();

    if (audioResources.contains(key)) {
        logger.log(LogLevel::INFO, "Unloading audio: " + key);
        UnloadSound(audioResources[key]);
        audioResources.erase(key);
    } else {
        logger.log(LogLevel::ERROR, "Error: Audio key '" + key + "' not found!");
    }
}

void AudioResourceManager::unloadAllAudio() {
    Logger& logger = Logger::getInstance();
    logger.log(LogLevel::INFO, "Unloading all audio resources.");

    for (const auto &[_, sound] : audioResources) {
        UnloadSound(sound);
    }
    audioResources.clear();

    logger.log(LogLevel::INFO, "All audio resources unloaded.");
}

void AudioResourceManager::buildAudioHeaders() {
    Logger& logger = Logger::getInstance();

    if constexpr (!Config::buildAudioHeaders) {
        logger.log(LogLevel::INFO, "Audio headers building disabled.");
        return;
    }

    logger.log(LogLevel::INFO, "Building audio headers.");

    const std::string outputDir = "../resources/audio/headers/";
    if (!std::filesystem::exists(outputDir)) {
        std::filesystem::create_directories(outputDir);
        logger.log(LogLevel::INFO, "Created output directory: " + outputDir);
    }

    for (const auto &[key, path] : predefinedAudioPaths) {
        const Wave wave = LoadWave(path.c_str());
        if (!wave.data) {
            logger.log(LogLevel::ERROR, "Error: Failed to load wave data: " + path);
            continue;
        }

        const std::string filename = path.substr(path.find_last_of("/\\") + 1);
        const std::string sanitizedFilename = filename.substr(0, filename.find_last_of('.')) + "_audio.h";
        const std::string outputPath = outputDir + sanitizedFilename;

        logger.log(LogLevel::INFO, "Building header: " + outputPath);

        if (!ExportWaveAsCode(wave, outputPath.c_str())) {
            logger.log(LogLevel::ERROR, "Failed to export wave as C header file: " + outputPath);
        } else {
            logger.log(LogLevel::INFO, "Wave exported as C header file successfully: " + outputPath);
        }

        UnloadWave(wave);
    }

    logger.log(LogLevel::INFO, "Finished building audio headers.");
}

void AudioResourceManager::playRawAudio(const std::string &key, const Wave &wave) {
    Logger& logger = Logger::getInstance();

    if (!audioResources.contains(key)) {
        logger.log(LogLevel::INFO, "Caching loaded audio file: " + key);
        audioResources[key] = LoadSoundFromWave(wave);
    }

    logger.log(LogLevel::INFO, "Playing raw audio: " + key);
    PlaySound(audioResources[key]);
}

void AudioResourceManager::playBackgroundMusic() {
    PlayMusicStream(background_game_music);
}

Music AudioResourceManager::getBackgroundMusicRef() const {
    return background_game_music;
}


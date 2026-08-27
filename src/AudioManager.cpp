#define MINIAUDIO_IMPLEMENTATION
#include "src/AudioManager.hpp"
#include <iostream>

AudioManager::AudioManager() : m_is_initialized(false) {}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    if (m_is_initialized) return true;

    // Initialize the high-level engine and the default playback device
    ma_result result = ma_engine_init(NULL, &m_engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize miniaudio engine. Error code: " << result << std::endl;
        return false;
    }

    m_is_initialized = true;
    return true;
}

void AudioManager::shutdown() {
    if (m_is_initialized) {
        ma_engine_uninit(&m_engine);
        m_is_initialized = false;
    }
}

bool AudioManager::play_sound(const std::string& filepath, bool loop) {
    if (!m_is_initialized) {
        std::cerr << "AudioManager must be initialized before playing sounds." << std::endl;
        return false;
    }

    if (loop) {
        ma_result result = ma_engine_play_sound(&m_engine, filepath.c_str(), nullptr);

        if (result != MA_SUCCESS) {
            std::cerr << "Failed to play sound: " << filepath << " | Error code: " << result << std::endl;
            return false;
        }
        
        return true;
    }
    
    ma_result result = ma_engine_play_sound(&m_engine, filepath.c_str(), nullptr);

    if (result != MA_SUCCESS)
    {
        std::cerr <<"Failed to play sound: " << filepath << " | Error code: " << result << std::endl;
        return false;
    }

    return true;
}

void AudioManager::set_master_volume(float volume) {
    if (m_is_initialized) {
        ma_engine_set_volume(&m_engine, volume);
    }
}


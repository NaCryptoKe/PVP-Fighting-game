#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include "miniaudio/miniaudio.h"
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Prevent copying to avoid multiple engine initializations
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Core Controls
    bool initialize();
    void shutdown();

    // Sound Operations
    bool play_sound(const std::string& filepath, bool loop = false);
    void set_master_volume(float volume); // Volume range: 0.0f to 1.0f+

private:
    ma_engine m_engine;
    bool m_is_initialized;
};

#endif // AUDIO_MANAGER_HPP
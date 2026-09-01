#include "src/AudioManager.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    AudioManager audio;

    if (!audio.initialize()) {
        return -1;
    }

    std::cout << "Playing background music..." << std::endl;
    // Pass a valid path to an MP3, WAV, or FLAC file
    audio.play_sound("assets/audio/background/background_music.mp3", true); 

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Playing explosion sound effect..." << std::endl;
    audio.play_sound("assets/audio/sfx/explosion.mp3", false);

    // Keep the main thread alive for 35 seconds while audio plays in the background
    std::this_thread::sleep_for(std::chrono::seconds(30));

    std::cout << "Lowering master volume to 20%..." << std::endl;
    audio.set_master_volume(0.2f);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Shutting down audio..." << std::endl;
    return 0; // Destructor handles cleanup automatically
}

// run with: g++ -std=c++17 -Wall -Wextra -Iinclude src/AudioManager.cpp test/audio_manager_test.cpp -o audio_manager_test && ./audio_manager_test
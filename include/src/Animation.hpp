#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "GL/glut.h"
#include <vector>

#include "src/Texture.hpp"

class Animation 
{
private:
    // A vector containing all the textures that make the animation
    std::vector<TextureData> frames;

    float frameDuration; // seconds per frame
    float elapsedTime;   // time accumulated on current frame
                         // might defer from the games update loop

    int currentFrame;
    
    bool looping;
    bool finished;

public:
    Animation();

    // Loads frames named like "00.png", "01.png", ... "NN.png" from a folder.
    // e.g. loadFromFiles("assets/characters/ryu/jump-forward/", 6, 0.08f)
    // loads jump-forward/00.png through 05.png at ~12.5 FPS playback.
    bool loadFromFiles(
        const char* folderPath, 
        int frameCount, float duration, 
        bool loop = true
    );

    // Advances playback. Call once per frame with the time elapsed since last call.
    void update(float deltaTime);
    // Delta time is useful becuase we don't want spikes for frames in differing computers

    // Restarts playback from frame 0.
    void reset();       // essential because we don't want it to continue from previous or other frames

    void setLooping(bool loop);
    bool isFinished() const;

    TextureData getCurrentTexture() const;  // id, height, width of current texture
    int getCurrentFrameIndex() const;       // index of frame in current frames vector
    int getFrameCount() const;              // size of frames
};

#endif // ANIMATION_HPP
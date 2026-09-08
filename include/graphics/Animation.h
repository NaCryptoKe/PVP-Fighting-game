#ifndef ANIMATION_H
#define ANIMATION_H

#include "GL/glut.h"
#include <vector>

#include "graphics/Texture.h"

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

    // Loads `frameCount` frames named like "NN.png" from a folder, starting
    // at `startFrame`. Most sprite folders are exported as 01.png..NN.png
    // (startFrame = 1), while some begin at 00.png (startFrame = 0).
    // e.g. loadFromFiles("assets/characters/ryu/idle/", 4, 0.15f)
    //      loads idle/00.png through 03.png at ~6.7 FPS playback.
    bool loadFromFiles(
        const char* folderPath, 
        int frameCount, float duration, 
        bool loop = true,
        int startFrame = 0
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

#endif // ANIMATION_H

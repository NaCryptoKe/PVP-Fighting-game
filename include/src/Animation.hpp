#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <GL/glut.h>
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
        bool loop = false
    );

    // Advances playback. Call once per frame with the time elapsed since last call.
    void update(float deltaTime);

    // Restarts playback from frame 0.
    void reset();

    void setLooping(bool loop);
    bool isFinished() const;

    TextureData getCurrentTexture() const;
    int getCurrentFrameIndex() const;
    int getFrameCount() const;
};

#endif // ANIMATION_HPP
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <GL/glut.h>
#include <vector>

class Animation 
{
private:
    std::vector<GLuint> frames;

    float frameDuration; // seconds per frame
    float elapsedTime;   // time accumulated on current frame
    int currentFrame;
    bool looping;
    bool finished;

public:
    Animation();

    // Loads frames named like "00.png", "01.png", ... "NN.png" from a folder.
    // e.g. loadFromFiles("assets/characters/ryu/jump-forward/", 6, 0.08f)
    // loads jump-forward/00.png through 05.png at ~12.5 FPS playback.
    bool loadFromFiles(const char* folderPath, int frameCount, float duration, bool loop = false);

    // Advances playback. Call once per frame with the time elapsed since last call.
    void update(float deltaTime);

    // Restarts playback from frame 0.
    void reset();

    void setLooping(bool loop);
    bool isFinished() const;

    GLuint getCurrentTexture() const;
    int getCurrentFrameIndex() const;
    int getFrameCount() const;
};

#endif // ANIMATION_HPP
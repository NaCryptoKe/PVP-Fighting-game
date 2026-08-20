#include "src/Animation.hpp"
#include <cstdio>

Animation::Animation()
    : frameDuration(0.1f), elapsedTime(0.0f), currentFrame(0),
      looping(true), finished(false) {}

bool Animation::loadFromFiles(
    const char* folderPath, 
    int frameCount, float duration, 
    bool loop
)
{
    frameDuration = duration; // how long each frame should take
    looping = loop;
    elapsedTime = 0.0f;
    currentFrame = 0;
    finished = false;

    frames.clear();
    frames.reserve(frameCount);

    for (int i = 0; i < frameCount; ++i) 
    {
        char path[256];
        snprintf(path, sizeof(path), "%s%02d.png", folderPath, i);  // basically delegating finding png
        // to naming format of 0x.png

        TextureData tex = loadTexture(path);
        if (tex.id == 0) 
        {
            printf("Animation: failed to load frame %s\n", path);
            return false;
        }

        frames.push_back(tex);
    }

    return !frames.empty(); // If frames is empty it means it didn't load
}

void Animation::update(float deltaTime) 
{
    if (frames.empty() || finished) 
    {
        return;
    }

    elapsedTime += deltaTime;

    while (elapsedTime >= frameDuration) // to calculate a frames duration has passed or not
    {
        elapsedTime -= frameDuration;
        currentFrame++;

        if (currentFrame >= (int)frames.size()) // diagnosing if it is looping or not
        {
            if (looping) 
            {
                currentFrame = 0;
            } 
            else 
            {
                currentFrame = (int)frames.size() - 1;
                finished = true;
                break;
            }
        }
    }
}

void Animation::reset() 
{
    currentFrame = 0;
    elapsedTime = 0.0f;
    finished = false;
}

void Animation::setLooping(bool loop) 
{
    looping = loop;
}

bool Animation::isFinished() const 
{
    return finished;
}

TextureData Animation::getCurrentTexture() const 
{
    if (frames.empty()) 
    {
        return { 0, 0, 0 };
    }
    return frames[currentFrame];
}

int Animation::getCurrentFrameIndex() const 
{
    return currentFrame;
}

int Animation::getFrameCount() const 
{
    return (int)frames.size();
}

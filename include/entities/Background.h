#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "entities/Camera.h"
#include "graphics/Texture.h"

// One tiling layer of the parallax background
struct BackgroundLayer
{
    TextureData texture;      // texture.id == 0 -> layer not loaded, skipped
    float tileWidth;          // one tile's size in world units
    float tileHeight;
    float parallaxFactor;     // 0 = glued to the camera, 1 = glued to the world
    float bottomY;            // world y of the layer's bottom edge
};

// Three-layer parallax background (sky, mountains, playfield wall).
// Rendered in WORLD space after the camera transform: each layer scrolls at
// its own rate by offsetting it with camera.centerX * (1 - parallaxFactor),
// and is tiled horizontally to cover the full visible width at any zoom.
class Background
{
public:
    // Loads the three layer textures (graceful: a failed layer is skipped)
    bool load();

    // Draws the layers back to front; call after Camera::apply
    void render(const Camera& camera) const;

private:
    void drawLayer(const BackgroundLayer& layer, const Camera& camera) const;

    BackgroundLayer layers[3];   // sky, mountains, wall (back to front)
};

#endif // BACKGROUND_H

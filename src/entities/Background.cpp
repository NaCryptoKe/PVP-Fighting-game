#include "entities/Background.h"

#include "GL/glut.h"
#include "render/Renderer.h"

#include <cmath>

namespace
{
// Tuning knobs — parallax rates, world heights and vertical anchors.
// The sky is oversized so it still covers the screen at minimum zoom:
// zoom 0.5 reveals a 3840x2160 world window (roughly y -930 .. +1335).
constexpr float SKY_FACTOR         = 0.15f;
constexpr float SKY_HEIGHT         = 2600.0f;
constexpr float SKY_BOTTOM_Y       = -1000.0f;

constexpr float MOUNTAINS_FACTOR   = 0.5f;
constexpr float MOUNTAINS_HEIGHT   = 700.0f;
constexpr float MOUNTAINS_BOTTOM_Y = 120.0f;   // the ground line

constexpr float WALL_FACTOR        = 1.0f;     // playfield: glued to the world
constexpr float WALL_HEIGHT        = 480.0f;
constexpr float WALL_BOTTOM_Y      = 120.0f;   // the fighters' feet

// Matches the fixed 1920-wide orthographic projection
constexpr float SCREEN_WIDTH = 1920.0f;

// Loads one layer texture and switches it to LINEAR filtering — scenic art
// looks better smooth, while the pixel-art fighters keep NEAREST.
TextureData loadLayerTexture(const char* path)
{
    TextureData texture = loadTexture(path);
    if (texture.id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    return texture;
}

// Builds one layer; the world-space tile width follows the texture's aspect
// ratio so the art is never stretched.
BackgroundLayer makeLayer(const TextureData& texture, float height,
                          float parallaxFactor, float bottomY)
{
    BackgroundLayer layer;
    layer.texture        = texture;
    layer.tileHeight     = height;
    layer.tileWidth      = (texture.height > 0)
        ? height * (static_cast<float>(texture.width) / static_cast<float>(texture.height))
        : height;
    layer.parallaxFactor = parallaxFactor;
    layer.bottomY        = bottomY;
    return layer;
}
}

bool Background::load()
{
    layers[0] = makeLayer(
        loadLayerTexture("assets/background/layer1_sky.png"),
        SKY_HEIGHT, SKY_FACTOR, SKY_BOTTOM_Y);

    layers[1] = makeLayer(
        loadLayerTexture("assets/background/layer2_mountains.png"),
        MOUNTAINS_HEIGHT, MOUNTAINS_FACTOR, MOUNTAINS_BOTTOM_Y);

    layers[2] = makeLayer(
        loadLayerTexture("assets/background/layer3_wall_playfield.png"),
        WALL_HEIGHT, WALL_FACTOR, WALL_BOTTOM_Y);

    return layers[0].texture.id != 0 || layers[1].texture.id != 0 || layers[2].texture.id != 0;
}

void Background::render(const Camera& camera) const
{
    // Back to front: sky, then mountains, then the playfield wall
    for (const BackgroundLayer& layer : layers)
    {
        if (layer.texture.id != 0)
            drawLayer(layer, camera);
    }
}

// Draws one layer across the visible width with the parallax offset applied.
//
// originX is where the layer's tiling starts: it moves with the camera at a
// REDUCED rate (camera.centerX * (1 - factor)) — that is what makes distant
// layers lag behind the fighters. Tiles are then placed starting from the
// left edge of the visible window until it is fully covered.
void Background::drawLayer(const BackgroundLayer& layer, const Camera& camera) const
{
    const float halfWidth    = (SCREEN_WIDTH * 0.5f) / camera.zoom;
    const float visibleLeft  = camera.centerX - halfWidth;
    const float visibleRight = camera.centerX + halfWidth;

    const float originX = camera.centerX * (1.0f - layer.parallaxFactor);

    int tileIndex = static_cast<int>(std::floor((visibleLeft - originX) / layer.tileWidth));
    const float tilesEnd = visibleRight + layer.tileWidth;   // one extra tile of safety

    for (; originX + tileIndex * layer.tileWidth < tilesEnd; ++tileIndex)
    {
        const float tileX = originX + tileIndex * layer.tileWidth;

        cg::Transform transform;
        // The unit quad is centered on its origin -> position = tile center
        transform.position = cg::Vec3(tileX + layer.tileWidth * 0.5f,
                                      layer.bottomY + layer.tileHeight * 0.5f,
                                      0.0f);
        transform.scale = cg::Vec3(layer.tileWidth, layer.tileHeight, 1.0f);

        Renderer::drawTexturedQuad(layer.texture.id, transform);
    }
}

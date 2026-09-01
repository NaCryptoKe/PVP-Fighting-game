#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera
{
private:
    // ------------------------------------------------------------
    // Shake
    // ------------------------------------------------------------
    float shakeIntensity;
    float shakeDuration;
    float shakeTimer;
    float duration;

    float offsetX;
    float offsetY;

    // ------------------------------------------------------------
    // Framing (pan + zoom)
    // ------------------------------------------------------------
    // "Zoom" widens (zooms out, < 1.0) when the players are far
    // apart and narrows (zooms in, > 1.0) when they're close
    // together, the way MK/SF-style 2D fighter cameras behave -
    // and pans to stay centered on the midpoint between them,
    // clamped so it never shows past the stage's edges.

    float stageLeft;
    float stageRight;

    float viewportWidth;
    float viewportHeight;

    float centerX; // current, smoothed toward target each frame
    float targetCenterX;
    float zoom;    // current, smoothed toward target each frame
    float targetZoom;

    static constexpr float MIN_ZOOM = 0.72f;                // widest  - players far apart
    static constexpr float MAX_ZOOM = 1.15f;                // closest - players close together
    static constexpr float NEAR_DISTANCE = 220.0f;      // at/below this gap -> MAX_ZOOM
    static constexpr float FAR_DISTANCE = 1100.0f;       // at/above this gap -> MIN_ZOOM
    static constexpr float FOLLOW_SPEED = 4.0f;         // higher = snappier camera

public:
    Camera();

    // --------------------------------------------------------
    // Shake
    // --------------------------------------------------------
    void shake(float intensity, float duration);

    // Advances shake decay only. Call once per frame regardless
    // of whether follow() is also used.
    void update(float deltaTime);

    float getOffsetX() const;
    float getOffsetY() const;

    // --------------------------------------------------------
    // Framing
    // --------------------------------------------------------

    // Sets the world-space left/right edges the camera won't pan
    // past, and re-centers the camera on them immediately (so the
    // very first frame doesn't animate in from an arbitrary start
    // position). Call once during setup, before the first follow().
    void setStageBounds(float left, float right);

    // Size, in world units, of the view at zoom 1.0 - i.e. the
    // virtual canvas passed to glOrtho/gluOrtho2D. Needed to know
    // how much world-space is visible at a given zoom level, both
    // for the pan/zoom framing calculation and for clamping the
    // pan to the stage bounds.
    void setViewportSize(float width, float height);

    // Recomputes target pan position and zoom level from both
    // players' X positions, and smoothly moves the camera toward
    // that target. Call once per frame, alongside update().
    void follow(float player1X, float player2X, float deltaTime);

    float getCenterX() const; // world X the camera is centered on
    float getZoom() const;    // scale multiplier - see MIN_ZOOM/MAX_ZOOM
};

#endif // CAMERA_HPP
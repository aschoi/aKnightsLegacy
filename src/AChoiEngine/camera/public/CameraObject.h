#pragma once

// struct for a 2D camera that centers on the player
// everything is in pixels.
struct ACE_Camera2D_Center {
    float x_px = 0.0f;   // top-left in WORLD space
    float y_px = 0.0f;
    float viewWidth_pixels = 0.0f;   // viewport size (screen size)  
    float viewHeight_pixels = 0.0f;

    // World bounds
    float worldWidth_pixels = 0.0f;
    float worldHeight_pixels = 0.0f;

    float screenWidth_px = 0.0f;
    float screenHeight_px = 0.0f;

    float zoom = 1;
};


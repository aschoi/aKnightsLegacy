#pragma once
#include <SDL3/SDL.h>
#include <algorithm>

// struct for a 2D camera that centers on the player
// everything is in pixels.
struct ACE_Camera2D {
    float x = 0.0f;   // top-left in WORLD space
    float y = 0.0f;
    float w = 0.0f;   // viewport size (screen size)  
    float h = 0.0f;

    // World bounds (optional, for clamping)
    float worldW = 0.0f;
    float worldH = 0.0f;

    void ACE_SetViewport(float viewW, float viewH) { 
        w = viewW; 
        h = viewH; 
    }

    void ACE_SetWorldBounds(float ww, float wh) {
        worldW = ww; 
        worldH = wh; 
    }

    void ACE_Follow(float targetX, float targetY) {
        // center camera on target
        x = targetX - w * 0.5f;
        y = targetY - h * 0.5f;

        // clamp to world bounds if provided
        if (worldW > 0.0f && worldH > 0.0f) {
            x = std::clamp(x, 0.0f, std::max(0.0f, worldW - w));
            y = std::clamp(y, 0.0f, std::max(0.0f, worldH - h));
        }
    }

    // Convert world -> screen
    SDL_FRect ACE_WorldToScreen(const SDL_FRect& worldRect) const {
        SDL_FRect out = worldRect;
        out.x -= x;
        out.y -= y;
        return out;
    }

    SDL_FPoint ACE_WorldToScreenPoint(float wx, float wy) const {
        return SDL_FPoint{ wx - x, wy - y };
    }
};


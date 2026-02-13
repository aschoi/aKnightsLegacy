#include <SDL3/SDL.h>
#include <algorithm>
#include "AChoiEngine/camera/public/CameraSys.h"
#include "AChoiEngine/camera/public/CameraObject.h"
#include "AChoiEngine/gameObject/public/GameObject.h"


void ACE_Cam_SetViewport(ACE_Camera2D_Center& cam, float viewWidth, float viewHeight) {
    cam.viewWidth_pixels = viewWidth;
    cam.viewHeight_pixels = viewHeight;
}

void ACE_Cam_SetWorldBounds(ACE_Camera2D_Center& cam, float worldWidth, float worldHeight) {
    cam.worldWidth_pixels = worldWidth;
    cam.worldHeight_pixels = worldHeight;
}

void ACE_Cam_FollowCenter(ACE_Camera2D_Center& cam, float targetX, float targetY) {
    // center camera on target
    cam.x_px = targetX - cam.viewWidth_pixels * 0.5f;
    cam.y_px = targetY - cam.viewHeight_pixels * 0.5f;

    // clamp to world bounds if provided
    if (cam.worldWidth_pixels > 0.0f && cam.worldHeight_pixels > 0.0f) {
        cam.x_px = std::clamp(cam.x_px, 0.0f, std::max(0.0f, cam.worldWidth_pixels - cam.viewWidth_pixels));
        cam.y_px = std::clamp(cam.y_px, 0.0f, std::max(0.0f, cam.worldHeight_pixels - cam.viewHeight_pixels));
    }
}

void ACE_Cam_FollowCenter(ACE_Camera2D_Center& cam, ACE_GameObject& gameObj) {
    // center camera on target
    cam.x_px = gameObj.get_x_px() - cam.viewWidth_pixels * 0.5f;
    cam.y_px = gameObj.get_y_px() - cam.viewHeight_pixels * 0.5f;

    // clamp to world bounds if provided
    if (cam.worldWidth_pixels > 0.0f && cam.worldHeight_pixels > 0.0f) {
        cam.x_px = std::clamp(cam.x_px, 0.0f, std::max(0.0f, cam.worldWidth_pixels - cam.viewWidth_pixels));
        cam.y_px = std::clamp(cam.y_px, 0.0f, std::max(0.0f, cam.worldHeight_pixels - cam.viewHeight_pixels));
    }
}

// Convert world -> screen
SDL_FRect ACE_Cam_WorldToScreen(ACE_Camera2D_Center& cam, const SDL_FRect& worldRect) {

    SDL_FRect out = worldRect;

    const float sx = (cam.viewWidth_pixels > 0.0f) ? (cam.screenWidth_px / cam.viewWidth_pixels) : 1.0f;
    const float sy = (cam.viewHeight_pixels > 0.0f) ? (cam.screenHeight_px / cam.viewHeight_pixels) : 1.0f;

    /*out.x = (out.x - cam.x_px) * cam.zoom;
    out.y = (out.y - cam.y_px) * cam.zoom;
    out.w = out.w * cam.zoom;
    out.h = out.h * cam.zoom;*/
    out.x = (worldRect.x - cam.x_px) * sx;
    out.y = (worldRect.y - cam.y_px) * sy;
    out.w = worldRect.w * sx;
    out.h = worldRect.h * sy;

    return out;
}

SDL_FPoint ACE_Cam_WorldToScreenPoint(ACE_Camera2D_Center& cam, float wx, float wy) {
    const float sx = (cam.viewWidth_pixels > 0.0f) ? (cam.screenWidth_px / cam.viewWidth_pixels) : 1.0f;
    const float sy = (cam.viewHeight_pixels > 0.0f) ? (cam.screenHeight_px / cam.viewHeight_pixels) : 1.0f;

    return SDL_FPoint{ (wx - cam.x_px) * sx, (wy - cam.y_px) * sy };

    //return SDL_FPoint{ (wx - cam.x_px) * cam.zoom, (wy - cam.y_px) * cam.zoom };
}


void ACE_Cam_SetScreenSize(ACE_Camera2D_Center& cam, float screenW, float screenH) {
    cam.screenWidth_px = screenW;
    cam.screenHeight_px = screenH;
}

void ACE_Cam_Zoom(ACE_Camera2D_Center& cam, float zoomFactor) {
    // zoomFactor > 1 => zoom in, zoomFactor < 1 => zoom out
    // because we store "view size", we invert it:
    // zoom in => smaller view
    const float newViewW = cam.viewWidth_pixels / zoomFactor;
    const float newViewH = cam.viewHeight_pixels / zoomFactor;

    // keep current center stable
    const float centerX = cam.x_px + cam.viewWidth_pixels * 0.5f;
    const float centerY = cam.y_px + cam.viewHeight_pixels * 0.5f;

    cam.viewWidth_pixels = newViewW;
    cam.viewHeight_pixels = newViewH;

    cam.x_px = centerX - cam.viewWidth_pixels * 0.5f;
    cam.y_px = centerY - cam.viewHeight_pixels * 0.5f;

    // clamp to world bounds
    if (cam.worldWidth_pixels > 0.0f && cam.worldHeight_pixels > 0.0f) {
        cam.x_px = std::clamp(cam.x_px, 0.0f, std::max(0.0f, cam.worldWidth_pixels - cam.viewWidth_pixels));
        cam.y_px = std::clamp(cam.y_px, 0.0f, std::max(0.0f, cam.worldHeight_pixels - cam.viewHeight_pixels));
    }
}

#pragma once
#include <SDL3/SDL.h>
#include <algorithm>

class ACE_Camera2D_Center;
class ACE_GameObject;

void ACE_Cam_SetViewport(ACE_Camera2D_Center& cam, float viewWidth, float viewHeight);
void ACE_Cam_SetWorldBounds(ACE_Camera2D_Center& cam, float worldWidth, float worldHeight);
void ACE_Cam_FollowCenter(ACE_Camera2D_Center& cam, float targetX, float targetY);
void ACE_Cam_FollowCenter(ACE_Camera2D_Center& cam, ACE_GameObject& gameObj);
void ACE_Cam_SetZoom(ACE_Camera2D_Center& cam, float new_zoom);

SDL_FRect ACE_Cam_WorldToScreen(ACE_Camera2D_Center& cam, const SDL_FRect& worldRect);
SDL_FPoint ACE_Cam_WorldToScreenPoint(ACE_Camera2D_Center& cam, float wx, float wy);

void ACE_Cam_SetScreenSize(ACE_Camera2D_Center& cam, float screenW, float screenH);
void ACE_Cam_Zoom(ACE_Camera2D_Center& cam, float zoomFactor);
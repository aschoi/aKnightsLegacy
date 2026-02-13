#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <string>
#include "AChoiEngine/input/public/Keyboard.h"

class ACE_Camera2D;
class ACE_Camera2D_Center;

struct ACE_HudLine {
	SDL_Texture* tex = nullptr;
	SDL_FRect dst{};
	float w_pixels = 0;
	float h_pixels = 0;
	float y_px = 0.0f;
};


class ACE_HudInterface {

public:
	virtual ~ACE_HudInterface() = default;

	virtual bool ACE_Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, ACE_Camera2D& cam) = 0;
	virtual void ACE_Shutdown() = 0;

	virtual void ACE_HandleEvent(Keys key, SDL_Renderer* r) = 0;

	virtual void ACE_UpdateFixed(double dt, const bool* keys) = 0;
	virtual void ACE_UpdateFrame(uint64_t now_ms) = 0;

	virtual void ACE_Render(SDL_Renderer* r, ACE_Camera2D& cam) = 0;
	virtual void ACE_Render(SDL_Renderer* r, ACE_Camera2D_Center& cam) = 0;


	virtual void ACE_SetLineText(int i, const std::string& s) = 0;


protected:


private:

};



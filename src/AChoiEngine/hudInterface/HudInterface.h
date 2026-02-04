#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <string>
#include "AChoiEngine/input/public/Keyboard.h"

class Camera2D;

struct HudLine {
	SDL_Texture* tex = nullptr;
	SDL_FRect dst{};
	float w_pixels = 0;
	float h_pixels = 0;
	float y_px = 0.0f;
};


class HudInterface {

public:
	virtual ~HudInterface() = default;

	virtual bool Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, Camera2D& cam) = 0;
	virtual void Shutdown() = 0;

	virtual void HandleEvent(Keys key, SDL_Renderer* r) = 0;

	virtual void UpdateFixed(double dt, const bool* keys) = 0;
	virtual void UpdateFrame(uint64_t now_ms) = 0;

	virtual void Render(SDL_Renderer* r, Camera2D& cam) = 0;

	virtual void SetLineText(int i, const std::string& s) = 0;


protected:


private:

};



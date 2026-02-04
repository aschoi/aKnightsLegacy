#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include "AChoiEngine/input/public/Keyboard.h"

class SDL_Renderer;

enum class GameState {StartMenu, MainGameplay, Credits, Pause, CustomStateOne, CustomStateTwo, CustomStateThree};

class GameStateInterface {

public:
	virtual ~GameStateInterface() = default;

	virtual bool Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) = 0;

	virtual void Shutdown() = 0;

	virtual void HandleEvent(Keys key) = 0;

	virtual void UpdateFixed(double dt, const bool* keys) = 0;
	virtual void UpdateFrame(uint64_t now_ms) = 0;

	virtual void Render(SDL_Renderer* r) = 0;

protected:

private:

};
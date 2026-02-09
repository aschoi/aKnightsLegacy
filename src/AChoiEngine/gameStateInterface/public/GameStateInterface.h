#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include "AChoiEngine/input/public/Keyboard.h"

class SDL_Renderer;

enum class GameState {StartMenu, MainGameplay, Credits, Pause, CustomStateOne, CustomStateTwo, CustomStateThree};

class ACE_GameStateInterface {

public:
	virtual ~ACE_GameStateInterface() = default;

	virtual bool ACE_Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) = 0;

	virtual void ACE_Shutdown() = 0;

	virtual void ACE_HandleEvent(Keys key) = 0;

	virtual void ACE_UpdateFixed(double dt, const bool* keys) = 0;
	virtual void ACE_UpdateFrame(uint64_t now_ms) = 0;

	virtual void ACE_Render(SDL_Renderer* r) = 0;

protected:

private:

};
#pragma once
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

struct appData {
	SDL_Renderer* appRenderer_ = nullptr;
	MIX_Mixer* appMixer_ = nullptr;
	float appW_ = 0;
	float appH_ = 0;
};

struct fontData {
	TTF_Font* boldFont = nullptr;
};


#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>

struct AnimSprite {
	SDL_Texture* tex = nullptr;
	std::vector<SDL_FRect> frames;
	float frameW = 0.0f;
	float frameH = 0.0f;
	int frameMs = 100;
	uint64_t startMs = 0;
	bool playing = false;
	bool loop = false;
	int iCurr = 0;
	float scale = 1.0f;
};

struct AnimGif {  // for already animated gifs
	std::vector<SDL_Texture*> frames;
	float w = 0;
	float h = 0;
	int frame_ms = 100;  // How long a frame lasts in ms
	uint64_t start_ms = 0;
	bool playing = false;
	bool loop = false;
	float scale = 1.0f;
};

AnimSprite BuildAnimSprite(	SDL_Renderer* r,
							const char* imagePath,
							int tileSize,
							int tilesPerFrameW,
							int tilesPerFrameH,
							float scale,
							int startCol,
							int startRow,
							int frameCount,
							int colsInSheet,
							int colSkipCount,
							int frameMs,
							bool loop);

void DestroyAnimSprite(AnimSprite& a);
void PlayAnimSprite(AnimSprite& a, uint64_t nowMs, bool loop);
void StopAnimSprite(AnimSprite& a);
bool FinishedNonLoopingSprite(const AnimSprite& a, uint64_t nowMs);
const SDL_FRect& CurrentFrameSprite(const AnimSprite& a);
void UpdateAnimSprite(AnimSprite& a, uint64_t nowMs);


AnimGif LoadAnimGif(SDL_Renderer* r, const char* kAssetPath, int frame_ms);
void DestroyAnimGif(AnimGif& a);
void PlayAnimGif(AnimGif& a, uint64_t now_ms, bool loop);
void StopAnimGif(AnimGif& a);
SDL_Texture* CurrentFrameGif(const AnimGif& kA, uint64_t now_ms);
bool FinishedNonLoopingGif(const AnimGif& kA, uint64_t now_ms);
#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>

struct ACE_AnimSprite {
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

struct ACE_AnimGif {  // for already animated gifs
	std::vector<SDL_Texture*> frames;
	float w = 0;
	float h = 0;
	int frame_ms = 100;  // How long a frame lasts in ms
	uint64_t start_ms = 0;
	bool playing = false;
	bool loop = false;
	float scale = 1.0f;
};

ACE_AnimSprite ACE_BuildAnimSprite(	SDL_Renderer* r,
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

void ACE_DestroyAnimSprite(ACE_AnimSprite& a);
void ACE_PlayAnimSprite(ACE_AnimSprite& a, uint64_t nowMs, bool loop);
void ACE_StopAnimSprite(ACE_AnimSprite& a);
bool ACE_FinishedNonLoopingSprite(const ACE_AnimSprite& a, uint64_t nowMs);
const SDL_FRect& ACE_CurrentFrameSprite(const ACE_AnimSprite& a);
void ACE_UpdateAnimSprite(ACE_AnimSprite& a, uint64_t nowMs);


ACE_AnimGif ACE_LoadAnimGif(SDL_Renderer* r, const char* kAssetPath, int frame_ms);
void ACE_DestroyAnimGif(ACE_AnimGif& a);
void ACE_PlayAnimGif(ACE_AnimGif& a, uint64_t now_ms, bool loop);
void ACE_StopAnimGif(ACE_AnimGif& a);
SDL_Texture* ACE_CurrentFrameGif(const ACE_AnimGif& kA, uint64_t now_ms);
bool ACE_FinishedNonLoopingGif(const ACE_AnimGif& kA, uint64_t now_ms);
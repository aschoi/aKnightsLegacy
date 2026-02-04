#include "AChoiEngine/gfx/public/AnimationComponent.h"
#include <SDL3/sdl.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <cstdint>

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
							bool loop) {

	AnimSprite a{};

	SDL_Surface* surf = IMG_Load(imagePath);
	if (!surf) {
		SDL_Log("BOOOOO (%s): %s", "tiles as surface failed to load", SDL_GetError());
		return a;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
	if (!tex) {
		SDL_Log("BOOOOO (%s): %s", "Tiles as texture failed to load", SDL_GetError());
		return a;
	}
	SDL_DestroySurface(surf);

	a.scale = scale;
	a.loop = loop;
	a.frameMs = frameMs;
	a.frameW = (float)(tileSize * tilesPerFrameW);
	a.frameH = (float)(tileSize * tilesPerFrameH);
	a.tex = tex;

	a.frames.reserve((size_t)frameCount);
	for (int i = 0; i < frameCount; i += colSkipCount) {
		int col = startCol + (i % colsInSheet);
		int row = startRow + (i / colsInSheet);

		SDL_FRect src{
			col * a.frameW,
			row * a.frameH,
			a.frameW,
			a.frameH
		};
		a.frames.push_back(src);
	}

	a.playing = false;
	a.startMs = SDL_GetTicks();
	a.iCurr = 0;
	return a;

}

void DestroyAnimSprite(AnimSprite& a) {
	if (a.tex) {
		SDL_DestroyTexture(a.tex);
	}
	a = AnimSprite{};
}

void PlayAnimSprite(AnimSprite& a, uint64_t nowMs, bool loop) {
	if (!a.tex || a.frames.empty()) return;

	a.playing = true;
	a.loop = loop;
	a.startMs = nowMs;
	a.iCurr = 0;
}

void StopAnimSprite(AnimSprite& a) { 
	a.playing = false; 
	a.loop = false;
}

bool FinishedNonLoopingSprite(const AnimSprite& a, uint64_t nowMs) {
	if (!a.playing || a.loop || a.frames.empty()) return false;
	uint64_t elapsed = nowMs - a.startMs;
	int idx = (a.frameMs > 0) ? (int)(elapsed / (uint64_t)a.frameMs) : 0;

	return idx >= (int)a.frames.size();
}

const SDL_FRect& CurrentFrameSprite(const AnimSprite& a) {
	static SDL_FRect dummy{ 0, 0, 0, 0 };
	if (a.frames.empty()) return dummy;

	int idx = a.iCurr;
	if (idx < 0) idx = 0;
	if (idx >= (int)a.frames.size()) idx = (int)a.frames.size() - 1;
	return a.frames[idx];
}

void UpdateAnimSprite(AnimSprite& a, uint64_t nowMs) {
	if (!a.playing || a.frames.empty() || a.frameMs <= 0) return;

	uint64_t elapsed = nowMs - a.startMs;
	int idx = (int)(elapsed / (uint64_t)a.frameMs);

	if (a.loop) {
		idx = idx % (int)a.frames.size();
		a.iCurr = idx;
	}
	else {
		if (idx >= (int)a.frames.size()) {
			a.iCurr = (int)a.frames.size() - 1;
			a.playing = false;
		}
		else {
			a.iCurr = idx;
		}
	}
}


AnimGif LoadAnimGif(SDL_Renderer* r, const char* kAssetPath, int frame_ms) {
	AnimGif a{};
	a.frame_ms = frame_ms;
	IMG_Animation* gif = IMG_LoadAnimation(kAssetPath);
	if (!gif) {
		SDL_Log("IMG_LoadAnimation failed (%s): %s", kAssetPath, SDL_GetError());
		return a;
	}

	a.w = gif->w;
	a.h = gif->h;

	a.frames.reserve(gif->count);
	for (int i = 0; i < gif->count; ++i) {
		SDL_Texture* tex = SDL_CreateTextureFromSurface(r, gif->frames[i]);
		if (!tex) {
			SDL_Log("SDL_CreateTextureFromSurface failed (frame %d): %s", i, SDL_GetError());
		}
		a.frames.push_back(tex);
	}
	IMG_FreeAnimation(gif);
	return a;
}

void DestroyAnimGif(AnimGif& a) {
	for (auto* t : a.frames) {
		SDL_DestroyTexture(t);
	}
	a.frames.clear();
	a.w = 0;
	a.h = 0;
}

void PlayAnimGif(AnimGif& a, uint64_t now_ms, bool loop) {
	a.playing = true;
	a.loop = loop;
	a.start_ms = now_ms;
}

void StopAnimGif(AnimGif& a) { a.playing = false; }

SDL_Texture* CurrentFrameGif(const AnimGif& kA, uint64_t now_ms) {
	if (kA.frames.empty()) return nullptr;
	if (!kA.playing) return kA.frames[0];

	uint64_t elapsed = now_ms - kA.start_ms;
	int idx = (int)(elapsed / (uint64_t)kA.frame_ms);

	if (kA.loop) idx %= (int)kA.frames.size();
	else if (idx >= (int)kA.frames.size()) idx = (int)kA.frames.size() - 1;

	return kA.frames[idx];
}

bool FinishedNonLoopingGif(const AnimGif& kA, uint64_t now_ms) {
	if (!kA.playing || kA.loop || kA.frames.empty()) return false;
	uint64_t elapsed = now_ms - kA.start_ms;
	int idx = (int)(elapsed / (uint64_t)kA.frame_ms);
	return idx >= (int)kA.frames.size();
}

#include "knightsLegacy/core/public/App.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdint>
#include "knightsLegacy/core/public/GameManager.h"
#include "knightsLegacy/debug/DebugConfigGame.h"
#include "assets.h"

App::App(int appWidthPixels, int appHeightPixels, std::string appTitle)
	:appW_pixels_(appWidthPixels), appH_pixels_(appHeightPixels), appTitle_(std::move(appTitle)) { }
App::~App() { Shutdown(); }


void App::Shutdown() {
	if (appRenderer_) { SDL_DestroyRenderer(appRenderer_); appRenderer_ = nullptr; }
	if (appWindow_) { SDL_DestroyWindow(appWindow_); appWindow_ = nullptr; }
	if (appMixer_) { MIX_DestroyMixer(appMixer_); appMixer_ = nullptr; }

	TTF_Quit();
	SDL_Quit();
	MIX_Quit();
}

int App::Run(GameManager& appNewGame) {
	// 1) Initialize SDL Video, Audio, and Text
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize Video: %s", SDL_GetError());
		return 3;
	}
	if (!MIX_Init()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize Audio: %s", SDL_GetError());
		return 3;
	}
	if (!TTF_Init()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF Text: %s", SDL_GetError());
		return 3;
	}

	// 2) Create GAME WINDOW and RENDERER 
	appWindow_ = SDL_CreateWindow(appTitle_.c_str(), appW_pixels_, appH_pixels_, 0);
	if (!appWindow_) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window in App.cpp: %s", SDL_GetError());
		return 3;
	}
	appRenderer_ = SDL_CreateRenderer(appWindow_, nullptr);
	if (!appRenderer_) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create Renderer in App.cpp: %s", SDL_GetError());
		return 3;
	}
	SDL_SetRenderDrawBlendMode(appRenderer_, SDL_BLENDMODE_BLEND);
	
	appMixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	if (!appMixer_) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create Mixer in App.cpp: %s", SDL_GetError());
		return 3;
	}

	audioManager = new AudioManager();
	if (!audioManager->ACE_Init(appMixer_))
		SDL_Log("Audio Manager Init Failed in App.cpp");

	if (!appNewGame.Init(appRenderer_, appMixer_, audioManager, appW_pixels_, appH_pixels_, 16, 16.0f)) {
		SDL_Log("Game init failed in App.cpp");
		return 3;
	}

	// Fixed timestep (60Hz)
	const double dt = 1.0 / 60.0;
	double accumulator = 0.0;

	uint64_t prevTimestamp = SDL_GetPerformanceCounter();
	const uint64_t freq = SDL_GetPerformanceFrequency();

	// MAIN GAME LOOP
	bool appRunning = true;
	while (appRunning) {
		// Timing
		uint64_t currTimestamp = SDL_GetPerformanceCounter();
		double frameTime = static_cast<double>(currTimestamp - prevTimestamp) / static_cast<double>(freq);
		prevTimestamp = currTimestamp;
		if (frameTime > 0.25) frameTime = 0.25; // If processing is taking too long
		accumulator += frameTime;

		// Events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			appNewGame.HandleEvent(event);
			if (event.type == SDL_EVENT_QUIT) appRunning = false;
			//if (appNewGame.get_isStartMenuGS() && event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) { appRunning = false; }
			if (appNewGame.get_closeGame()) appRunning = false;
		}

		// Fixed Updates - do unlimited game state updates until its time to render.
		while (accumulator >= dt) {
			const bool* kKeys = SDL_GetKeyboardState(nullptr);
			appNewGame.UpdateFixed(dt, kKeys);

			accumulator -= dt;
		}

		// Per-frame updates (animation and such)
		const uint64_t kNow_ms = SDL_GetTicks();
		appNewGame.UpdateFrame(kNow_ms);

		audioManager->unloadQ();

		// Render once per frame
		SDL_SetRenderDrawColor(appRenderer_, 15, 15, 18, 255);
		SDL_RenderClear(appRenderer_);

		appNewGame.Render(appRenderer_);

		SDL_RenderPresent(appRenderer_);
	}

	#if DEBUG_GAME
		SDL_Log("App.cpp - Game Manager Shutdown Start");
	#endif

	// NEED SHUTDOWN LOGIC FOR AUDIO MANAGER
	audioManager->ACE_Shutdown();

	appNewGame.Shutdown();


	#if DEBUG_GAME
		SDL_Log("App.cpp - Game Manager Shutdown End");
	#endif

	return 0;
}		
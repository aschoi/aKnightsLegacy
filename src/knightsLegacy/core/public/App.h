#pragma once
#include <string>
#include <SDL3_mixer/SDL_mixer.h>
#include "knightsLegacy/audioManager/public/AudioManager.h"

class GameManager;

class App {

public:
	// Constructor
	App(int appWidthPixels, int appHeightPixels, std::string appTitle);
	// Destructor
	~App();

	// No copies allowed
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	int Run(GameManager& appNewGame);

	SDL_Renderer* getAppRenderer() const { return appRenderer_; }
	MIX_Mixer* getAppMixer() const { return appMixer_; }
	int getAppWidthPixels() const { return appW_pixels_; }
	int getAppHeightPixels() const { return appH_pixels_; }

private:
	void Shutdown();

	int appW_pixels_ = 0;
	int appH_pixels_ = 0;
	std::string appTitle_;

	SDL_Window* appWindow_ = nullptr;
	SDL_Renderer* appRenderer_ = nullptr;
	MIX_Mixer* appMixer_ = nullptr;

	AudioManager* audioManager = nullptr;


};
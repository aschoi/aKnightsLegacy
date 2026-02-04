#pragma once
#include <vector>
#include <map>
#include <deque>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/camera/public/Camera.h"
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include "AChoiEngine/audio/public/AudioInterface.h"
#include "knightsLegacy/gameStates/public/StartMenuGameState.h"
#include "knightsLegacy/gameStates/public/MainGameplayGameState.h"
#include "knightsLegacy/gameStates/public/CreditsGameState.h"
#include "knightsLegacy/audioManager/public/AudioManager.h"


// If you store pointers to game states:
class MainGameplayGameState;

class GameManager {
public:
	bool Init(SDL_Renderer* appR, MIX_Mixer* appMixer, float appW, float appH);
	bool Init(SDL_Renderer* appR, float appW, float appH);
	bool Init(SDL_Renderer* appR, MIX_Mixer* appM, AudioManager* audioM, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat);

	void Shutdown();

	void HandleEvent(const SDL_Event& kEvent);
	void UpdateFixed(double dt, const bool* kKeys);
	void UpdateFrame(uint64_t now_ms);

	void Render(SDL_Renderer* appR);

	bool get_isStartMenuGS();

	bool debugMode = false;

	float appW_pixels_ = 0;
	float appH_pixels_ = 0;
	float tileSizeAsFloat_ = 0.0f;
	int tileSizeAsInt_ = 0;

	//float curWorldW_ = 0;
	//float curWorldH_ = 0;

	bool get_closeGame();
	void set_closeGame(bool new_close);

	//void setAM(AudioManager& new_AM);

protected:

private:

	SDL_Renderer* appR_ = nullptr;
	StartMenuGameState* startMenuGS_ = nullptr;
	MainGameplayGameState* mainGameGS_ = nullptr;
	CreditsGameState* creditsGS_ = nullptr;

	GameState curGS_ = GameState::StartMenu;

	bool closeGame = false;

	MIX_Mixer* appMixer_ = nullptr;
	AudioManager* audioManager_ = nullptr;
	
};
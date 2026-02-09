#pragma once
#include "AChoiEngine/audio/public/AudioInterface.h"
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <SDL3_mixer/SDL_mixer.h>


enum class PlayerAudioState { QCast, WCast, ECast, RCast, Attack, Walking, Running, TakingDamage, Death };
enum class SkeletonAudioState { Attack, Walking, Running, TakingDamage, Death };
enum class HammerAudioState { InFlight };
enum class ProjectileAudioState { Charging, InFlight, Impact };
enum class SoundFX { Hammer, Splat, Ult, Swipe, Select, Confirm, Count};
enum class BackgroundMusic { StartMenu, MainGameplay, Credits };


class AudioManager : public ACE_AudioInterface {
public:
	
	bool ACE_Init(MIX_Mixer* mixer) override;
	void ACE_Shutdown() override;

	bool ACE_LoadSfx(const std::string& id, const std::string& path) override;
	bool ACE_LoadMusic(const std::string& id, const std::string& path) override;

	void ACE_LoadPlayMusic(BackgroundMusic bgMusic);

	void ACE_PlaySfx(const std::string& id) override;
	void ACE_PlayMusic(const std::string& id) override;
	void ACE_StopMusic(const std::string& id) override;

	void loadQ(SoundFX sfx);
	void unloadQ();

private:
	MIX_Mixer* mixer_ = nullptr;

	MIX_Audio* hammerAudio = nullptr;
	MIX_Track* hammerTrack = nullptr;

	MIX_Audio* splatAudio = nullptr;
	MIX_Track* splatTrack = nullptr;

	MIX_Audio* ultAudio = nullptr;
	MIX_Track* ultTrack = nullptr;

	MIX_Audio* music1Audio = nullptr;
	MIX_Track* music1Track = nullptr;

	MIX_Audio* music2Audio = nullptr;
	MIX_Track* music2Track = nullptr;

	MIX_Audio* swipeAudio = nullptr;
	MIX_Track* swipeTrack = nullptr;

	MIX_Audio* confirmAudio = nullptr;
	MIX_Track* confirmTrack = nullptr;

	MIX_Audio* selectAudio = nullptr;
	MIX_Track* selectTrack = nullptr;


	std::queue<SoundFX> q;
	std::queue<MIX_Track*> tracks;
};

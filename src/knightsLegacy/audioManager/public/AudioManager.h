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

enum class SoundFX { Hammer, Splat };

class AudioManager : public AudioInterface {
public:
	
	bool Init(MIX_Mixer* mixer) override;
	void Shutdown() override;

	bool LoadSfx(const std::string& id, const std::string& path) override;
	bool LoadMusic(const std::string& id, const std::string& path) override;

	void PlaySfx(const std::string& id) override;
	void PlayMusic(const std::string& id) override;
	void StopMusic(const std::string& id) override;

	void loadQ(SoundFX sfx);
	void unloadQ();

private:
	MIX_Mixer* mixer_ = nullptr;

	MIX_Audio* hammerAudio = nullptr;
	MIX_Track* hammerTrack = nullptr;

	MIX_Audio* splatAudio = nullptr;
	MIX_Track* splatTrack = nullptr;


	std::queue<SoundFX> q;
	std::queue<MIX_Track*> tracks;
};

#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

class AudioInterface {
public:
	virtual ~AudioInterface() = default;
	virtual bool Init(MIX_Mixer* mixer_) = 0;
	virtual void Shutdown() = 0;

	virtual bool LoadSfx(const std::string& id, const std::string& path) = 0;
	virtual bool LoadMusic(const std::string& id, const std::string& path) = 0;

	virtual void PlaySfx(const std::string& id) = 0;
	virtual void PlayMusic(const std::string& id) = 0;
	virtual void StopMusic(const std::string& id) = 0;

};


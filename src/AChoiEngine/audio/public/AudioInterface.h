#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include <string>



class ACE_AudioInterface {
public:
	virtual ~ACE_AudioInterface() = default;
	virtual bool ACE_Init(MIX_Mixer* mixer_) = 0;
	virtual void ACE_Shutdown() = 0;

	virtual bool ACE_LoadSfx(const std::string& id, const std::string& path) = 0;
	virtual bool ACE_LoadMusic(const std::string& id, const std::string& path) = 0;

	virtual void ACE_PlaySfx(const std::string& id) = 0;
	virtual void ACE_PlayMusic(const std::string& id) = 0;
	virtual void ACE_StopMusic(const std::string& id) = 0;

};


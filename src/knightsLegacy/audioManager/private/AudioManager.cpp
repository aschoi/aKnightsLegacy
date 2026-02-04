#include "knightsLegacy/audioManager/public/AudioManager.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include "assets.h"

bool AudioManager::Init(MIX_Mixer* mixer) {

    mixer_ = mixer;
    
    // Load all sfx
    hammerAudio = MIX_LoadAudio(mixer_, sfxHammerPath.c_str(), true);
    splatAudio = MIX_LoadAudio(mixer_, sfxSplatPath.c_str(), true);

    hammerTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(hammerTrack, 0.05f);
    MIX_SetTrackAudio(hammerTrack, hammerAudio);

    splatTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(splatTrack, 0.05f);
    MIX_SetTrackAudio(splatTrack, splatAudio);

	return true;
}
void AudioManager::Shutdown() {

    MIX_DestroyAudio(hammerAudio);
    MIX_DestroyTrack(hammerTrack);
    MIX_DestroyAudio(splatAudio);
    MIX_DestroyTrack(splatTrack);
}


bool AudioManager::LoadSfx(const std::string& id, const std::string& path) {

	return true;
}
bool AudioManager::LoadMusic(const std::string& id, const std::string& path) {
	return true;
}

void AudioManager::loadQ(SoundFX sfx) {

    if (sfx == SoundFX::Hammer ) {
        q.push(sfx);
    }
    else if (sfx == SoundFX::Splat ) {
        q.push(sfx);
    }
}

void AudioManager::unloadQ() {

    while (!q.empty()) {
        if (q.front() == SoundFX::Hammer) {
            tracks.push(hammerTrack);
        }
        else if (q.front() == SoundFX::Splat) {
            tracks.push(splatTrack);
        }
        q.pop();
    }

    while (!tracks.empty()) {
        MIX_PlayTrack(tracks.front(), 0);
        tracks.pop();
    }
}

void AudioManager::PlaySfx(const std::string& id) {

}

void AudioManager::PlayMusic(const std::string& id) {


}
void AudioManager::StopMusic(const std::string& id) {

}




#include "knightsLegacy/audioManager/public/AudioManager.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include "assets.h"

bool AudioManager::ACE_Init(MIX_Mixer* mixer) {

    mixer_ = mixer;
    
    // Load all audio
    hammerAudio = MIX_LoadAudio(mixer_, sfxHammerPath.c_str(), true);
    splatAudio = MIX_LoadAudio(mixer_, sfxSplatPath.c_str(), true);
    ultAudio = MIX_LoadAudio(mixer_, sfxUltPath.c_str(), true);
    swipeAudio = MIX_LoadAudio(mixer_, sfxSwipe.c_str(), true);
    confirmAudio = MIX_LoadAudio(mixer_, uiConfirm.c_str(), true);
    selectAudio = MIX_LoadAudio(mixer_, uiSelect.c_str(), true);
    music1Audio = MIX_LoadAudio(mixer_, music1.c_str(), true);
    music2Audio = MIX_LoadAudio(mixer_, music2.c_str(), true);

    // set sfx tracks
    hammerTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(hammerTrack, 0.05f);
    MIX_SetTrackAudio(hammerTrack, hammerAudio);

    splatTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(splatTrack, 0.05f);
    MIX_SetTrackAudio(splatTrack, splatAudio);

    ultTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(ultTrack, 0.05f);
    MIX_SetTrackAudio(ultTrack, ultAudio);

    swipeTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(swipeTrack, 0.05f);
    MIX_SetTrackAudio(swipeTrack, swipeAudio);

    confirmTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(confirmTrack, 0.08f);
    MIX_SetTrackAudio(confirmTrack, confirmAudio);

    selectTrack = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(selectTrack, 0.08f);
    MIX_SetTrackAudio(selectTrack, selectAudio);

    // set all music tracks
    music1Track = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(music1Track, 0.08f);
    MIX_SetTrackAudio(music1Track, music1Audio);

    music2Track = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(music2Track, 0.08f);
    MIX_SetTrackAudio(music2Track, music2Audio);

	return true;
}

void AudioManager::ACE_Shutdown() {

    MIX_DestroyAudio(hammerAudio);
    MIX_DestroyTrack(hammerTrack);
    MIX_DestroyAudio(splatAudio);
    MIX_DestroyTrack(splatTrack);
    MIX_DestroyAudio(ultAudio);
    MIX_DestroyTrack(ultTrack);
    MIX_DestroyAudio(swipeAudio);
    MIX_DestroyTrack(swipeTrack);
    MIX_DestroyAudio(selectAudio);
    MIX_DestroyTrack(selectTrack);
    MIX_DestroyAudio(music1Audio);
    MIX_DestroyTrack(music1Track);
    MIX_DestroyAudio(music2Audio);
    MIX_DestroyTrack(music2Track);
}


bool AudioManager::ACE_LoadSfx(const std::string& id, const std::string& path) {

	return true;
}

bool AudioManager::ACE_LoadMusic(const std::string& id, const std::string& path) {
    return true;
}

void AudioManager::ACE_LoadPlayMusic(BackgroundMusic bgMusic) {

    MIX_StopAllTracks(mixer_, 0);

    SDL_PropertiesID options = SDL_CreateProperties();
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    
    if (bgMusic == BackgroundMusic::StartMenu)
        MIX_PlayTrack(music2Track, options);
    else if (bgMusic == BackgroundMusic::MainGameplay)
        MIX_PlayTrack(music1Track, options);

    SDL_DestroyProperties(options);
}


void AudioManager::loadQ(SoundFX sfx) {

    if (SoundFX::Hammer <= sfx && sfx <= SoundFX::Count) {
        q.push(sfx);
    }

}

void AudioManager::unloadQ() {

    while (!q.empty()) {
        if (q.front() == SoundFX::Swipe) tracks.push(swipeTrack);
        else if (q.front() == SoundFX::Splat) tracks.push(splatTrack);
        else if (q.front() == SoundFX::Hammer) tracks.push(hammerTrack);
        else if (q.front() == SoundFX::Ult) tracks.push(ultTrack);
        else if (q.front() == SoundFX::Confirm) tracks.push(confirmTrack);
        else if (q.front() == SoundFX::Select) tracks.push(selectTrack);
        q.pop();
    }

    while (!tracks.empty()) {
        MIX_PlayTrack(tracks.front(), 0);
        tracks.pop();
    }
}

void AudioManager::ACE_PlaySfx(const std::string& id) {

}

void AudioManager::ACE_PlayMusic(const std::string& id) {


}
void AudioManager::ACE_StopMusic(const std::string& id) {

}




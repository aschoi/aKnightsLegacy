#include "knightsLegacy/core/public/GameManager.h"
#include <string>
#include <iostream>
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include "AChoiEngine/input/public/Keyboard.h"
#include "knightsLegacy/gameStates/public/StartMenuGameState.h"
#include "knightsLegacy/gameStates/public/MainGameplayGameState.h"
#include "assets.h"


bool GameManager::Init(SDL_Renderer* appR, MIX_Mixer* appM, float appW, float appH) {
    return false;
}
bool GameManager::Init(SDL_Renderer* appR, float appW, float appH) {
    return false;
}
bool GameManager::Init(SDL_Renderer* appR, MIX_Mixer* appM, AudioManager* audioM, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {

    appR_ = appR;
    appMixer_ = appM;
    appW_pixels_ = appW_pixels;
    appH_pixels_ = appH_pixels;
    tileSizeAsInt_ = tileSizeAsInt;
    tileSizeAsFloat_ = tileSizeAsFloat;
    curGS_ = GameState::StartMenu;
    audioManager_ = audioM;

    startMenuGS_ = new StartMenuGameState();
    startMenuGS_->Init(appR, audioM, appW_pixels_, appH_pixels_, tileSizeAsInt_, tileSizeAsFloat_);

    
    return true;
}

void GameManager::Shutdown() {
    
    if (startMenuGS_) {
        startMenuGS_->ACE_Shutdown();
        delete startMenuGS_;
        startMenuGS_ = nullptr;
    }
    if (mainGameGS_) {
        mainGameGS_->ACE_Shutdown();
        delete mainGameGS_;
        mainGameGS_ = nullptr;
    }

    if (creditsGS_) {
        creditsGS_->ACE_Shutdown();
        delete creditsGS_;
        creditsGS_ = nullptr;
    }    
    appR_ = nullptr;
    appMixer_ = nullptr;
    audioManager_ = nullptr;

}


void GameManager::HandleEvent(const SDL_Event& event) {
    
    if (startMenuGS_ && curGS_ == GameState::StartMenu) {
        if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            if (event.key.key == SDLK_UP) startMenuGS_->ACE_HandleEvent(Keys::Up);
            if (event.key.key == SDLK_DOWN) startMenuGS_->ACE_HandleEvent(Keys::Down);
            if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER) {
                if (startMenuGS_->curOption == StartMenuOption::StartGame) {
                    startMenuGS_->ACE_HandleEvent(Keys::Confirm);
                }
                else if (startMenuGS_->curOption == StartMenuOption::Credits) {
                    startMenuGS_->ACE_HandleEvent(Keys::Confirm);
                }
            }
            if (event.key.key == SDLK_ESCAPE) {
                set_closeGame(true);
            }

        }
        if (startMenuGS_->gameStartPressed == true) {
            startMenuGS_->gameStartPressed = false;
            curGS_ = GameState::MainGameplay;
            if (mainGameGS_) {
                mainGameGS_->ACE_Shutdown();
                delete mainGameGS_;
                mainGameGS_ = nullptr;
            }
            mainGameGS_ = new MainGameplayGameState();
            mainGameGS_->Init(appR_, audioManager_, appW_pixels_, appH_pixels_, tileSizeAsInt_, tileSizeAsFloat_);

        }
        else if (startMenuGS_->creditsPressed == true) {
            startMenuGS_->creditsPressed = false;
            curGS_ = GameState::Credits;
            if (creditsGS_) {
                creditsGS_->ACE_Shutdown();
                delete creditsGS_;
                creditsGS_ = nullptr;
            }
            creditsGS_ = new CreditsGameState();
            creditsGS_->ACE_Init(appR_, appW_pixels_, appH_pixels_, tileSizeAsInt_, tileSizeAsFloat_);
        }
    }
    else if (mainGameGS_ && curGS_ == GameState::MainGameplay) {
        if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            if (event.key.scancode == SDL_SCANCODE_SPACE) mainGameGS_->ACE_HandleEvent(Keys::Space);
            if (event.key.scancode == SDL_SCANCODE_Q) mainGameGS_->ACE_HandleEvent(Keys::Q);
            if (event.key.scancode == SDL_SCANCODE_W) mainGameGS_->ACE_HandleEvent(Keys::W);
            if (event.key.scancode == SDL_SCANCODE_E) mainGameGS_->ACE_HandleEvent(Keys::E);
            if (event.key.scancode == SDL_SCANCODE_R) mainGameGS_->ACE_HandleEvent(Keys::R);

            if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                curGS_ = GameState::StartMenu;
                audioManager_->ACE_LoadPlayMusic(BackgroundMusic::StartMenu);
                // DOING THIS TO NOT CAUSE A MEMORY LEAK. I WANT TO HAVE A BETTER FUTURE SOLUTION
                // BUT SHUTTING DOWN MAIN GAME STATE EVERY TIME JUST IN CASE.
                // MAYBE A FUTURE FEATURE IS TO HAVE AN OPTION TO RETURN TO CURRENT GAME OR START NEW GAME.
                mainGameGS_->ACE_Shutdown();
                delete mainGameGS_;
                mainGameGS_ = nullptr;
            }
        }
    }

    else if (creditsGS_ && curGS_ == GameState::Credits) {
        if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            if (event.key.scancode == SDL_SCANCODE_SPACE || event.key.scancode == SDL_SCANCODE_ESCAPE) {
                curGS_ = GameState::StartMenu;
                creditsGS_->ACE_Shutdown();
                delete creditsGS_;
                creditsGS_ = nullptr;
            }
        }
    }
}

void GameManager::UpdateFixed(double dt, const bool* kKeys) {
    if (startMenuGS_ && curGS_ == GameState::StartMenu) {
        startMenuGS_->ACE_UpdateFixed(dt, kKeys);
    }
    else if (mainGameGS_ && curGS_ == GameState::MainGameplay) {
        mainGameGS_->ACE_UpdateFixed(dt, kKeys);
    }
    else if (creditsGS_ && curGS_ == GameState::Credits) {
        creditsGS_->ACE_UpdateFixed(dt, kKeys);
    }
}

void GameManager::UpdateFrame(uint64_t now_ms) {
    (void)now_ms;

    if (startMenuGS_ && curGS_ == GameState::StartMenu) {
        startMenuGS_->ACE_UpdateFrame(now_ms);
    }
    else if (mainGameGS_ && curGS_ == GameState::MainGameplay) {
        mainGameGS_->ACE_UpdateFrame(now_ms);
    }
    else if (creditsGS_ && curGS_ == GameState::Credits) {
        creditsGS_->ACE_UpdateFrame(now_ms);
    }
}

void GameManager::Render(SDL_Renderer* appR) {

    if (startMenuGS_ && curGS_ == GameState::StartMenu) {
        startMenuGS_->ACE_Render(appR);
    }
    else if (mainGameGS_ && curGS_ == GameState::MainGameplay) {
        mainGameGS_->ACE_Render(appR);
    }
    else if (creditsGS_ && curGS_ == GameState::Credits) {
        creditsGS_->ACE_Render(appR);
    }

}

bool GameManager::get_isStartMenuGS() {
    return curGS_ == GameState::StartMenu;
}

bool GameManager::get_closeGame() {
    return closeGame;
}
void GameManager::set_closeGame(bool new_close) {
    closeGame = new_close;
}



    
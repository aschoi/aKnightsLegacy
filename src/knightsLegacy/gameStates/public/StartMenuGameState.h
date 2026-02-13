#pragma once
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"
#include "AChoiEngine/worldLayer/public/MapObject.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "AChoiEngine/gfx/public/AnimationSys.h"
#include "AChoiEngine/camera/public/Camera.h"

class AudioManager;

enum class StartMenuOption { StartGame, Credits };


class StartMenuGameState : public ACE_GameStateInterface {

public:

    bool ACE_Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) override;
    bool Init(SDL_Renderer* appR, AudioManager* audMan, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat);

    void ACE_Shutdown() override;

    void ACE_HandleEvent(Keys key) override;

    void ACE_UpdateFixed(double dt, const bool* keys) override;
    void ACE_UpdateFrame(uint64_t now_ms) override;

    void ACE_Render(SDL_Renderer* r) override;

    float appW_pixels_ = 0;
    float appH_pixels_ = 0;
    int tileSizeAsInt_ = 0;
    float tileSizeAsFloat_ = 0.0f;

    SDL_Renderer* r_ = nullptr;   // does NOT own
    //float worldW_ = 0;
    //float worldH_ = 0;

    SDL_Texture* startGameTex = nullptr;
    SDL_FRect startGameTexRect = { 0 };
    bool gameStartPressed = false;

    SDL_Texture* creditsTex = nullptr;
    SDL_FRect creditsTexRect = { 0 };
    bool creditsPressed = false;

    ACE_MapObject backgroundMapLayer;

    SDL_Texture* backgroundTex = nullptr;
    SDL_FRect backgroundTexRect = { 0 };


    SDL_FRect selectTL_ = { 0 };
    SDL_FRect selectTR_ = { 0 };
    SDL_FRect selectBL_ = { 0 };
    SDL_FRect selectBR_ = { 0 };

    StartMenuOption curOption = StartMenuOption::StartGame;

    TTF_Font* boldFont = nullptr;  // OWNS

    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 255 };
    SDL_Color greyColor{ 40, 40, 40, 255 };
    SDL_Color darkBlueColor{ 10, 25, 33, 255 };
    SDL_Color gold1Color{ 191, 151, 48, 255 };
    SDL_Color gold2Color{ 140, 106, 46, 255 };
    SDL_Color white2Color{ 252, 250, 240, 255 };


protected:

private:
    ACE_TileMap startMenuBaseLayer_;
    ACE_TileMap startMenuTextLayer_;

    ACE_AnimSprite selectionTL_;
    ACE_AnimSprite selectionTR_;
    ACE_AnimSprite selectionBL_;
    ACE_AnimSprite selectionBR_;
    ACE_Camera2D cam;
    AudioManager* audioManager_ = nullptr;

};
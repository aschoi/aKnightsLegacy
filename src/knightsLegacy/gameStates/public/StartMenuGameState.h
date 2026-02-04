#pragma once
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "AChoiEngine/gfx/public/AnimationComponent.h"

class AudioManager;

enum class StartMenuOption { StartGame, Credits };


class StartMenuGameState : public GameStateInterface {

public:

    bool Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) override;
    bool Init(SDL_Renderer* appR, AudioManager* audMan, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat);

    void Shutdown() override;

    void HandleEvent(Keys key) override;

    void UpdateFixed(double dt, const bool* keys) override;
    void UpdateFrame(uint64_t now_ms) override;

    void Render(SDL_Renderer* r) override;

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


    SDL_FRect selectTL_ = { 0 };
    SDL_FRect selectTR_ = { 0 };
    SDL_FRect selectBL_ = { 0 };
    SDL_FRect selectBR_ = { 0 };

    StartMenuOption curOption = StartMenuOption::StartGame;

    TTF_Font* boldFont = nullptr;  // OWNS

    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 255 };

protected:

private:
    TileMap startMenuBaseLayer_;
    TileMap startMenuTextLayer_;

    AnimSprite selectionTL_;
    AnimSprite selectionTR_;
    AnimSprite selectionBL_;
    AnimSprite selectionBR_;
    Camera2D cam;


};
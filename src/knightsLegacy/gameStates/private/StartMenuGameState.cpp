#include "knightsLegacy/gameStates/public/StartMenuGameState.h"
#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "knightsLegacy/core/public/GameManager.h"
#include "knightsLegacy/world/public/TileCatalog.h"
#include "assets.h"


bool StartMenuGameState::ACE_Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {
    return false;
}

bool StartMenuGameState::Init(SDL_Renderer* appR, AudioManager* audMan, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {
    appW_pixels_ = appW_pixels;
    appH_pixels_ = appH_pixels;
    tileSizeAsInt_ = tileSizeAsInt;
    tileSizeAsFloat_ = tileSizeAsFloat;
    r_ = appR;

    backgroundMapLayer.ACE_Init(appR, jsonStartMenu.c_str());

    SDL_Surface* bgSurf = IMG_Load(backgroundCastle.c_str());
    if (!bgSurf) {
        SDL_Log("BOOOOO (%s): %s", "tiles as surface failed to load", SDL_GetError());
        ACE_Shutdown();
        return false;
    }
    backgroundTex = SDL_CreateTextureFromSurface(appR, bgSurf);
    backgroundTexRect = {
        0.0f,
        350.0f,
        (float)bgSurf->w,
        (float)bgSurf->h - 509
    };
    SDL_DestroySurface(bgSurf);


    boldFont = TTF_OpenFont(fontBold1Path.c_str(), 44);
    
    const char* startGameMsg = "Start Game";
    const char* creditsMsg = "Credits";

    SDL_Surface* startGameSurf = TTF_RenderText_Blended(boldFont, startGameMsg, SDL_strlen(startGameMsg), white2Color);
    SDL_Surface* creditsSurf = TTF_RenderText_Blended(boldFont, creditsMsg, SDL_strlen(creditsMsg), white2Color);

    startGameTex = SDL_CreateTextureFromSurface(appR, startGameSurf);
    creditsTex = SDL_CreateTextureFromSurface(appR, creditsSurf);

    // Lotta hardcoded numbers. Try to refactor.
    startGameTexRect = {
        500.0f - startGameSurf->w * 0.5f,
        200.0f - startGameSurf->h * 0.5f,
        (float)startGameSurf->w,
        (float)startGameSurf->h
    };

    creditsTexRect = {
        500.0f - creditsSurf->w * 0.5f,
        300.0f - creditsSurf->h * 0.5f,
        (float)creditsSurf->w,
        (float)creditsSurf->h
    };

    SDL_DestroySurface(startGameSurf);
    SDL_DestroySurface(creditsSurf);

    const float rendW = selectionTL_.frameW * selectionTL_.scale;
    const float rendH = selectionTL_.frameH * selectionTL_.scale;


    selectionTL_ = ACE_BuildAnimSprite(appR, ui2Path.c_str(),       // renderer, imagePath, 
                                    tileSizeAsInt_, 1, 1, 2.0f, // tileSize, tilesPerFrameW, tilesPerFrameH, scale, 
                                    0, 0, 4, 8,                 // startCol, startRow, frameCount, colsInSheet, 
                                    2, 300, true);              // colSkipCount, frameMs, loop

    selectionTR_ = ACE_BuildAnimSprite(appR, ui2Path.c_str(),
                                    tileSizeAsInt_, 1, 1, 2.0f,
                                    1, 0, 4, 8,
                                    2, 300, true);
    selectionBL_ = ACE_BuildAnimSprite(appR, ui2Path.c_str(),
                                    tileSizeAsInt_, 1, 1, 2.0f,
                                    0, 1, 4, 8,
                                    2, 300, true);

    selectionBR_ = ACE_BuildAnimSprite(appR, ui2Path.c_str(),
                                    tileSizeAsInt_, 1, 1, 2.0f,
                                    1, 1, 4, 8,
                                    2, 300, true);

    ACE_PlayAnimSprite(selectionTL_, SDL_GetTicks(), true);
    ACE_PlayAnimSprite(selectionTR_, SDL_GetTicks(), true);
    ACE_PlayAnimSprite(selectionBL_, SDL_GetTicks(), true);
    ACE_PlayAnimSprite(selectionBR_, SDL_GetTicks(), true);

    cam.ACE_SetViewport(appW_pixels_, appH_pixels_);

    audioManager_ = audMan;

    audioManager_->ACE_LoadPlayMusic(BackgroundMusic::StartMenu);

    return true;

}


void StartMenuGameState::ACE_Shutdown() {
    
    backgroundMapLayer.ACE_Shutdown();

    SDL_DestroyTexture(startGameTex);
    SDL_DestroyTexture(creditsTex);
    SDL_DestroyTexture(backgroundTex);
    if (boldFont) {
        TTF_CloseFont(boldFont);
        boldFont = nullptr;
    }

    startGameTex = nullptr;
    creditsTex = nullptr;
    backgroundTex = nullptr;

    ACE_DestroyAnimSprite(selectionTL_);
    ACE_DestroyAnimSprite(selectionTR_);
    ACE_DestroyAnimSprite(selectionBL_);
    ACE_DestroyAnimSprite(selectionBR_);
    audioManager_ = nullptr;

    r_ = nullptr;  
}

void StartMenuGameState::ACE_HandleEvent(Keys keyPressed) {

    if (keyPressed == Keys::Up) {
        curOption = StartMenuOption::StartGame;
        audioManager_->loadQ(SoundFX::Select);
    }
    else if (keyPressed == Keys::Down) {
        curOption = StartMenuOption::Credits;
        audioManager_->loadQ(SoundFX::Select);
    }
    else if (keyPressed == Keys::Confirm) {
        if (curOption == StartMenuOption::StartGame) {
            gameStartPressed = true;
        }
        else if (curOption == StartMenuOption::Credits) {
            creditsPressed = true;
        }
        audioManager_->loadQ(SoundFX::Confirm);
    }
}

void StartMenuGameState::ACE_UpdateFixed(double dt, const bool* keys) {

}

void StartMenuGameState::ACE_UpdateFrame(uint64_t now_ms) {
    (void)now_ms;

    ACE_UpdateAnimSprite(selectionTL_, now_ms);
    ACE_UpdateAnimSprite(selectionTR_, now_ms);
    ACE_UpdateAnimSprite(selectionBL_, now_ms);
    ACE_UpdateAnimSprite(selectionBR_, now_ms);

}

void StartMenuGameState::ACE_Render(SDL_Renderer* r) {
    
    SDL_FRect bgDst{ 0.0f, 0.0f, appW_pixels_, appH_pixels_ };
    SDL_RenderTexture(r, backgroundTex, &backgroundTexRect, &bgDst);
    
    backgroundMapLayer.ACE_Render(r, 160);
    
    SDL_RenderTexture(r, startGameTex, nullptr, &startGameTexRect);
    SDL_RenderTexture(r, creditsTex, nullptr, &creditsTexRect);


    const float rendW = selectionTL_.frameW * selectionTL_.scale;
    const float rendH = selectionTL_.frameH * selectionTL_.scale;


    if (curOption == StartMenuOption::StartGame) {
        // START GAME
        const SDL_FRect dstTL{ 370.0f - rendW * 0.5f, 175.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTL = &ACE_CurrentFrameSprite(selectionTL_);;
        SDL_RenderTextureRotated(r, selectionTL_.tex, srcTL, &dstTL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstTR{ 625.0f - rendW * 0.5f, 175.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTR = &ACE_CurrentFrameSprite(selectionTR_);;
        SDL_RenderTextureRotated(r, selectionTR_.tex, srcTR, &dstTR, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBL{ 370.0f - rendW * 0.5f, 225.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBL = &ACE_CurrentFrameSprite(selectionBL_);;
        SDL_RenderTextureRotated(r, selectionBL_.tex, srcBL, &dstBL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBR{ 625.0f - rendW * 0.5f, 225.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBR = &ACE_CurrentFrameSprite(selectionBR_);;
        SDL_RenderTextureRotated(r, selectionBR_.tex, srcBR, &dstBR, 0.0, nullptr, SDL_FLIP_NONE);
    }

    else if (curOption == StartMenuOption::Credits) {
        // CREDITS
        const SDL_FRect dstTL{ 370.0f - rendW * 0.5f, 275.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTL = &ACE_CurrentFrameSprite(selectionTL_);;
        SDL_RenderTextureRotated(r, selectionTL_.tex, srcTL, &dstTL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstTR{ 625.0f - rendW * 0.5f, 275.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTR = &ACE_CurrentFrameSprite(selectionTR_);;
        SDL_RenderTextureRotated(r, selectionTR_.tex, srcTR, &dstTR, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBL{ 370.0f - rendW * 0.5f, 325.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBL = &ACE_CurrentFrameSprite(selectionBL_);;
        SDL_RenderTextureRotated(r, selectionBL_.tex, srcBL, &dstBL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBR{ 625.0f - rendW * 0.5f, 325.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBR = &ACE_CurrentFrameSprite(selectionBR_);;
        SDL_RenderTextureRotated(r, selectionBR_.tex, srcBR, &dstBR, 0.0, nullptr, SDL_FLIP_NONE);
    }



}


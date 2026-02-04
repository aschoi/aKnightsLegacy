#pragma once
#include "knightsLegacy/gameStates/public/StartMenuGameState.h"
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
//#include <SDL3_mixer/SDL_mixer.h>
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "knightsLegacy/core/public/GameManager.h"
#include "knightsLegacy/world/public/TileCatalog.h"
#include "assets.h"

bool Init(SDL_Renderer* r, AudioManager* audMan, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {
    return false;
}

bool StartMenuGameState::Init(SDL_Renderer* appR, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {
    appW_pixels_ = appW_pixels;
    appH_pixels_ = appH_pixels;
    tileSizeAsInt_ = tileSizeAsInt;
    tileSizeAsFloat_ = tileSizeAsFloat;
    r_ = appR;

    startMenuBaseLayer_.LoadTiles(appR, environmentTilesheets, ui1Path.c_str(), startMenuBase.c_str(), tileSizeAsFloat_); 
    startMenuTextLayer_.LoadTiles(appR, environmentTilesheets, ui1Path.c_str(), startMenuTextV6.c_str(), tileSizeAsFloat_);

    boldFont = TTF_OpenFont(fontBold1Path.c_str(), 44);
    
    const char* startGameMsg = "Start Game";
    const char* creditsMsg = "Credits";

    SDL_Surface* startGameSurf = TTF_RenderText_Blended(boldFont, startGameMsg, SDL_strlen(startGameMsg), whiteColor);
    SDL_Surface* creditsSurf = TTF_RenderText_Blended(boldFont, creditsMsg, SDL_strlen(creditsMsg), whiteColor);

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


    selectionTL_ = BuildAnimSprite(appR, ui2Path.c_str(),       // renderer, imagePath, 
                                    tileSizeAsInt_, 1, 1, 2.0f, // tileSize, tilesPerFrameW, tilesPerFrameH, scale, 
                                    0, 0, 4, 8,                 // startCol, startRow, frameCount, colsInSheet, 
                                    2, 300, true);              // colSkipCount, frameMs, loop

    selectionTR_ = BuildAnimSprite(appR, ui2Path.c_str(),
                                    tileSizeAsInt_, 1, 1, 2.0f,
                                    1, 0, 4, 8,
                                    2, 300, true);
    selectionBL_ = BuildAnimSprite(appR, ui2Path.c_str(),
                                    tileSizeAsInt_, 1, 1, 2.0f,
                                    0, 1, 4, 8,
                                    2, 300, true);

    selectionBR_ = BuildAnimSprite(appR, ui2Path.c_str(),
                                    tileSizeAsInt_, 1, 1, 2.0f,
                                    1, 1, 4, 8,
                                    2, 300, true);

    PlayAnimSprite(selectionTL_, SDL_GetTicks(), true);
    PlayAnimSprite(selectionTR_, SDL_GetTicks(), true);
    PlayAnimSprite(selectionBL_, SDL_GetTicks(), true);
    PlayAnimSprite(selectionBR_, SDL_GetTicks(), true);

    cam.SetViewport(appW_pixels_, appH_pixels_);
    return true;
}


void StartMenuGameState::Shutdown() {
    
    startMenuBaseLayer_.Shutdown();
    startMenuTextLayer_.Shutdown();

    SDL_DestroyTexture(startGameTex);
    SDL_DestroyTexture(creditsTex);
    if (boldFont) {
        TTF_CloseFont(boldFont);
        boldFont = nullptr;
    }

    startGameTex = nullptr;
    creditsTex = nullptr;

    DestroyAnimSprite(selectionTL_);
    DestroyAnimSprite(selectionTR_);
    DestroyAnimSprite(selectionBL_);
    DestroyAnimSprite(selectionBR_);

    r_ = nullptr;  
}

void StartMenuGameState::HandleEvent(Keys keyPressed) {

    if (keyPressed == Keys::Up) {
        curOption = StartMenuOption::StartGame;
    }
    if (keyPressed == Keys::Down) {
        curOption = StartMenuOption::Credits;
    }
    
    if (keyPressed == Keys::Confirm) {
        if (curOption == StartMenuOption::StartGame) {
            gameStartPressed = true;
        }
        else if (curOption == StartMenuOption::Credits) {
            creditsPressed = true;
        }
    }
}

void StartMenuGameState::UpdateFixed(double dt, const bool* keys) {

}

void StartMenuGameState::UpdateFrame(uint64_t now_ms) {
    (void)now_ms;

    UpdateAnimSprite(selectionTL_, now_ms);
    UpdateAnimSprite(selectionTR_, now_ms);
    UpdateAnimSprite(selectionBL_, now_ms);
    UpdateAnimSprite(selectionBR_, now_ms);

}

void StartMenuGameState::Render(SDL_Renderer* r) {
    
    startMenuBaseLayer_.Render(r, cam, GetTileCatalog());
    startMenuTextLayer_.Render(r, cam, GetTileCatalog());
    SDL_RenderTexture(r, startGameTex, nullptr, &startGameTexRect);
    SDL_RenderTexture(r, creditsTex, nullptr, &creditsTexRect);


    const float rendW = selectionTL_.frameW * selectionTL_.scale;
    const float rendH = selectionTL_.frameH * selectionTL_.scale;


    if (curOption == StartMenuOption::StartGame) {
        // START GAME
        const SDL_FRect dstTL{ 370.0f - rendW * 0.5f, 175.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTL = &CurrentFrameSprite(selectionTL_);;
        SDL_RenderTextureRotated(r, selectionTL_.tex, srcTL, &dstTL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstTR{ 625.0f - rendW * 0.5f, 175.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTR = &CurrentFrameSprite(selectionTR_);;
        SDL_RenderTextureRotated(r, selectionTR_.tex, srcTR, &dstTR, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBL{ 370.0f - rendW * 0.5f, 225.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBL = &CurrentFrameSprite(selectionBL_);;
        SDL_RenderTextureRotated(r, selectionBL_.tex, srcBL, &dstBL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBR{ 625.0f - rendW * 0.5f, 225.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBR = &CurrentFrameSprite(selectionBR_);;
        SDL_RenderTextureRotated(r, selectionBR_.tex, srcBR, &dstBR, 0.0, nullptr, SDL_FLIP_NONE);
    }

    else if (curOption == StartMenuOption::Credits) {
        // CREDITS
        const SDL_FRect dstTL{ 370.0f - rendW * 0.5f, 275.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTL = &CurrentFrameSprite(selectionTL_);;
        SDL_RenderTextureRotated(r, selectionTL_.tex, srcTL, &dstTL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstTR{ 625.0f - rendW * 0.5f, 275.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcTR = &CurrentFrameSprite(selectionTR_);;
        SDL_RenderTextureRotated(r, selectionTR_.tex, srcTR, &dstTR, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBL{ 370.0f - rendW * 0.5f, 325.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBL = &CurrentFrameSprite(selectionBL_);;
        SDL_RenderTextureRotated(r, selectionBL_.tex, srcBL, &dstBL, 0.0, nullptr, SDL_FLIP_NONE);

        const SDL_FRect dstBR{ 625.0f - rendW * 0.5f, 325.0f - rendH * 0.5f, rendW, rendH };
        const SDL_FRect* srcBR = &CurrentFrameSprite(selectionBR_);;
        SDL_RenderTextureRotated(r, selectionBR_.tex, srcBR, &dstBR, 0.0, nullptr, SDL_FLIP_NONE);
    }



}


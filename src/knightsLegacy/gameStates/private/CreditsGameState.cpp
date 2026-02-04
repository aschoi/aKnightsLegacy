#include "knightsLegacy/gameStates/public/CreditsGameState.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "knightsLegacy/core/public/GameManager.h"
#include "assets.h"


bool CreditsGameState::Init(SDL_Renderer* appR, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {
    appW_pixels_ = appW_pixels;
    appH_pixels_ = appH_pixels;
    tileSizeAsInt_ = tileSizeAsInt;
    tileSizeAsFloat_ = tileSizeAsFloat;
    r_ = appR;

    creditBaseLayer_.LoadTiles(appR, environmentTilesheets, ui1Path.c_str(), startMenuBase.c_str(), tileSizeAsFloat_);

    boldFont = TTF_OpenFont(fontBold1Path.c_str(), 44);
    SDL_Color white{ 255, 255, 255, 255 };
    SDL_Color black{ 0, 0, 0, 255 };

    std::vector<std::string> credits = {
        "Game Director:             Alex Choi", 
        "", 
        "Systems Architect:         Alex Choi",
        "",
        "Lead Engine Programmer:    Alex Choi",
        "",
        "Lead AI Developer:         Alex Choi",
        "",
        "Lead Physics Developer:    Alex Choi",
        "",
        "Renderer:                  SDL3",
        "",
        "Audio Mixer:               SDL3_mixer",
        "",
        "Lead Game Programmer:      Alex Choi",
        "",
        "Lead Game Designer:        Alex Choi",
        "",
        "Lead Character Desiner:    Alex Choi",
        "",
        "VFX Artist:                Alex Choi",
        "",
        "Executive Producer:        Alex Choi",
        "",
        "Screenplay:                Alex Choi",
        "",
        "Artists:",
        "CRAFTPIX",
        "Pixel_Poem",
        "crusenho", 
        "bdragon1727",
        "aamatniekss",
        "frostwindz",
        "",
        "Special Thanks:        Jon Sourbeer"   
    };


    float y = appH_pixels_;
    for (const auto& text : credits) {
        if (text.empty()) {
            y += 20; // blank line spacing
            continue;
        }

        SDL_Surface* surf = TTF_RenderText_Blended(boldFont, text.c_str(), text.size(), white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(appR, surf);

        CreditLine line;
        line.tex = tex;
        line.w = surf->w;
        line.h = surf->h;
        line.y = y;

        y += line.h + 10;
        blockWidth = std::max(blockWidth, line.w);
        SDL_DestroySurface(surf);
        lines_.push_back(line);
    }

    cam.SetViewport(appW_pixels_, appH_pixels_);

    TTF_CloseFont(boldFont);
    boldFont = nullptr;

    return true;
}


void CreditsGameState::Shutdown() {
    for (auto& line : lines_) {
        SDL_DestroyTexture(line.tex);
        line.tex = nullptr;
    }

    lines_.clear();

    if (boldFont) {
        TTF_CloseFont(boldFont);
        boldFont = nullptr;
    }
    r_ = nullptr;
}

void CreditsGameState::HandleEvent(Keys keyPressed) {
}

void CreditsGameState::UpdateFixed(double dt, const bool* keys) {
    for (auto& line : lines_) {
        line.y -= scrollSpeed_ * (float)dt;
    }
}
void CreditsGameState::UpdateFrame(uint64_t now_ms) {
}

void CreditsGameState::Render(SDL_Renderer* r) {
    int winW, winH;
    SDL_GetRenderOutputSize(r, &winW, &winH);
    float startX = (winW - blockWidth) * 0.5f;

    for (const auto& line : lines_) {

        SDL_FRect dst{
            //startX,  // center horizontally
            (winW - line.w) * 0.5f,
            line.y,
            (float)line.w,
            (float)line.h
        };

        SDL_RenderTexture(r, line.tex, nullptr, &dst);
    }
}


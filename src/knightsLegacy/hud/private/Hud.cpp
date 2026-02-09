#include "knightsLegacy/hud/public/Hud.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <cstdint>
#include "AChoiEngine/camera/public/Camera.h"
#include "knightsLegacy/entities/player/public/Player.h"
#include "assets.h"


bool Hud::Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, ACE_Camera2D& cam, Player& p) {
    worldW_pixels = p.get_world_w_pixels();
    worldH_pixels = p.get_world_h_pixels();

    appW_pixels_ = appW_pixels;
    appH_pixels_ = appH_pixels;

    appR = r;

	boldFont = TTF_OpenFont(fontBold1Path.c_str(), 20);
	SDL_Color white{ 255, 255, 255, 255 };
	SDL_Color black{ 0, 0, 0, 255 };

    playerHealth_ = p.getPlayerHealth();
    health_string = "Health: " + std::to_string(playerHealth_);
    qCD_ = std::ceil(p.getSlideTimer());
    qCD_string = "Q CD: " + std::to_string(qCD_);
    wCD_ = std::ceil(p.getHammerTimer());
    wCD_string = "W CD: " + std::to_string(wCD_);
    eCD_ = std::ceil(p.getRollTimer());
    eCD_string = "E CD: " + std::to_string(eCD_);
    rCD_ = std::ceil(p.getProjectileTimer());
    rCD_string = "R CD: " + std::to_string(rCD_);

    float curY_px = 10;

    // I can clean this up with a for loop (later)
    SDL_Surface* surf1 = TTF_RenderText_Blended(boldFont, health_string.c_str(), health_string.size(), black);
    SDL_Texture* tex1 = SDL_CreateTextureFromSurface(r, surf1);
    ACE_HudLine healthLine;
    healthLine.tex = tex1;
    healthLine.w_pixels = surf1->w;
    healthLine.h_pixels = surf1->h;
    healthLine.y_px = curY_px;
    healthLine.dst = { 20, healthLine.y_px, healthLine.w_pixels, healthLine.h_pixels };
    curY_px += (healthLine.h_pixels) * 0.75;
    SDL_DestroySurface(surf1);
    hudLines_.push_back(healthLine);

    SDL_Surface* surf2 = TTF_RenderText_Blended(boldFont, qCD_string.c_str(), qCD_string.size(), black);
    SDL_Texture* tex2 = SDL_CreateTextureFromSurface(r, surf2);
    ACE_HudLine qCDLine;
    qCDLine.tex = tex2;
    qCDLine.w_pixels = surf2->w;
    qCDLine.h_pixels = surf2->h;
    qCDLine.y_px = curY_px;
    qCDLine.dst = { 20, qCDLine.y_px, qCDLine.w_pixels, qCDLine.h_pixels };
    curY_px += (qCDLine.h_pixels) * 0.75;
    SDL_DestroySurface(surf2);
    hudLines_.push_back(qCDLine);

    SDL_Surface* surf3 = TTF_RenderText_Blended(boldFont, wCD_string.c_str(), wCD_string.size(), black);
    SDL_Texture* tex3 = SDL_CreateTextureFromSurface(r, surf3);
    ACE_HudLine wCDLine;
    wCDLine.tex = tex3;
    wCDLine.w_pixels = surf3->w;
    wCDLine.h_pixels = surf3->h;
    wCDLine.y_px = curY_px;
    wCDLine.dst = { 20, wCDLine.y_px, wCDLine.w_pixels, wCDLine.h_pixels };
    curY_px += (wCDLine.h_pixels) * 0.75;
    SDL_DestroySurface(surf3);
    hudLines_.push_back(wCDLine);

    SDL_Surface* surf4 = TTF_RenderText_Blended(boldFont, eCD_string.c_str(), eCD_string.size(), black);
    SDL_Texture* tex4 = SDL_CreateTextureFromSurface(r, surf4);
    ACE_HudLine eCDLine;
    eCDLine.tex = tex4;
    eCDLine.w_pixels = surf4->w;
    eCDLine.h_pixels = surf4->h;
    eCDLine.y_px = curY_px;
    eCDLine.dst = { 20, eCDLine.y_px, eCDLine.w_pixels, eCDLine.h_pixels };
    curY_px += (eCDLine.h_pixels) * 0.75;
    SDL_DestroySurface(surf4);
    hudLines_.push_back(eCDLine);

    SDL_Surface* surf5 = TTF_RenderText_Blended(boldFont, rCD_string.c_str(), rCD_string.size(), black);
    SDL_Texture* tex5 = SDL_CreateTextureFromSurface(r, surf5);
    ACE_HudLine rCDLine;
    rCDLine.tex = tex5;
    rCDLine.w_pixels = surf5->w;
    rCDLine.h_pixels = surf5->h;
    rCDLine.y_px = curY_px;
    rCDLine.dst = { 20, rCDLine.y_px, rCDLine.w_pixels, rCDLine.h_pixels };
    curY_px += (rCDLine.h_pixels) * 0.75;
    SDL_DestroySurface(surf5);
    hudLines_.push_back(rCDLine);

    cam.ACE_SetViewport(appW_pixels_, appH_pixels_);
    return true;

}
bool Hud::ACE_Init(SDL_Renderer* r, float appW, float appH, ACE_Camera2D& cam) {
    return false;
}


void Hud::ACE_UpdateFrame(uint64_t now_ms) {}


void Hud::ACE_Shutdown() {
    for (auto& line : hudLines_) {
        SDL_DestroyTexture(line.tex);
    }

    hudLines_.clear(); 

    if (boldFont) {
        TTF_CloseFont(boldFont);
        boldFont = nullptr;
    }
    appR = nullptr;

}
void Hud::ACE_HandleEvent(Keys key, SDL_Renderer* r) {

}
void Hud::ACE_UpdateFixed(double dt, const bool* keys) {

}

void Hud::UpdateFrame(uint64_t now_ms, Player& p) {
    (void)now_ms;

    if (!(playerHealth_ == p.getPlayerHealth())) {
        playerHealth_ = p.getPlayerHealth();
        health_string = "Health: " + std::to_string(playerHealth_);
        ACE_SetLineText(0, health_string);
    }
    if (!(qCD_ == std::ceil(p.getSlideTimer()))) {
        qCD_ = std::ceil(p.getSlideTimer());
        qCD_string = "Q CD: " + std::to_string(qCD_);
        ACE_SetLineText(1, qCD_string);
    }
    if (!(wCD_ == std::ceil(p.getHammerTimer()))) {
        wCD_ = std::ceil(p.getHammerTimer());
        wCD_string = "W CD: " + std::to_string(wCD_);
        ACE_SetLineText(2, wCD_string);
    }
    if (!(eCD_ == std::ceil(p.getRollTimer()))) {
        eCD_ = std::ceil(p.getRollTimer());
        eCD_string = "E CD: " + std::to_string(eCD_);
        ACE_SetLineText(3, eCD_string);
    }
    if (!(rCD_ == std::ceil(p.getProjectileTimer()))) {
        rCD_ = std::ceil(p.getProjectileTimer());
        rCD_string = "R CD: " + std::to_string(rCD_);
        ACE_SetLineText(4, rCD_string);
    }
}

void Hud::ACE_Render(SDL_Renderer* r, ACE_Camera2D& cam) {

    for (ACE_HudLine& line : hudLines_) {
        if (!line.tex) continue;

        SDL_RenderTexture(
            r,
            line.tex,
            nullptr,
            &line.dst
        );
    }
}


// reset hudline texture (i could maybe add this to HudInterface. Seems good to have in the engine.)
void Hud::ACE_SetLineText(int iLine, const std::string& s) {
    if (iLine >= (int)hudLines_.size()) return;

    if (hudLines_[iLine].tex) {
        SDL_DestroyTexture(hudLines_[iLine].tex);
        hudLines_[iLine].tex = nullptr;
    }

    SDL_Color white{ 255,255,255,255 };
    SDL_Color black{ 0, 0, 0, 255 };

    SDL_Surface* surf = TTF_RenderText_Blended(boldFont, s.c_str(), s.size(), white);
    if (!surf) return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(appR, surf);
    SDL_DestroySurface(surf);
    if (!tex) return;

    float tw = 0.0f, th = 0.0f;
    SDL_GetTextureSize(tex, &tw, &th);
    hudLines_[iLine].w_pixels = tw;
    hudLines_[iLine].dst.w = tw;
    hudLines_[iLine].h_pixels = th;
    hudLines_[iLine].dst.h = th;
    hudLines_[iLine].tex = tex;

}
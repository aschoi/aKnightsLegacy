#pragma once
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <unordered_map>
#include <vector>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"


struct CreditLine {
    SDL_Texture* tex = nullptr;
    int w = 0;
    int h = 0;
    float y = 0.0f;
};

class CreditsGameState : public GameStateInterface {

public:

    bool Init(SDL_Renderer* r, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) override;

    void Shutdown() override;

    void HandleEvent(Keys key) override;

    void UpdateFixed(double dt, const bool* keys) override;
    void UpdateFrame(uint64_t now_ms) override;

    void Render(SDL_Renderer* r) override;

    float appW_pixels_ = 0;
    float appH_pixels_ = 0;
    int tileSizeAsInt_ = 0;
    float tileSizeAsFloat_ = 0.0f;
    SDL_Renderer* r_ = nullptr;
    //float worldW_ = 0;
    //float worldH_ = 0;

    SDL_Texture* creditsTex = nullptr;
    SDL_FRect creditsTexRect = { 0 };

    TTF_Font* boldFont = nullptr;

    int blockWidth = 0;
protected:

private:

    TileMap creditBaseLayer_;
    std::vector<CreditLine> lines_;
    float scrollSpeed_ = 40.0f;   // pixels per second
    float startY_ = 0.0f;

    Camera2D cam;


};
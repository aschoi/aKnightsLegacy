//#pragma once
//#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
//#include "AChoiEngine/input/public/Keyboard.h"
////#include "knightsLegacy/world/public/TileMap.h"
//#include "knightsLegacy/worldLayer/public/TxtMapLoader.h"
//#include <unordered_map>
//#include "AChoiEngine/gfx/public/AnimationComponent.h"
//#include <SDL3_ttf/SDL_ttf.h>
//
//
//
//enum class PauseMenuOption { StartMenu, ReturnToGame };
//
//
//class PauseGameState : public GameStateInterface {
//
//public:
//
//    bool Init(SDL_Renderer* r, float appW, float appH) override;
//    void Shutdown() override;
//
//    void HandleEvent(Keys key, SDL_Renderer* r) override;
//
//    void UpdateFixed(double dt, const bool* keys) override;
//    void UpdateFrame(uint64_t now_ms) override;
//
//    void Render(SDL_Renderer* r) override;
//
//    float appW_ = 0;
//    float appH_ = 0;
//    //float worldW_ = 0;
//    //float worldH_ = 0;
//
//
//    SDL_Texture* returnToGameTex = nullptr;
//    SDL_FRect returnToGameTexRect = { 0 };
//
//    SDL_Texture* startMenuTex = nullptr;
//    SDL_FRect startMenuTexRect = { 0 };
//
//    SDL_FRect selectTL_ = { 0 };
//    SDL_FRect selectTR_ = { 0 };
//    SDL_FRect selectBL_ = { 0 };
//    SDL_FRect selectBR_ = { 0 };
//
//    PauseMenuOption curOption = PauseMenuOption::ReturnToGame;
//
//    TTF_Font* boldFont = nullptr;
//
//
//protected:
//
//private:
//    TileMap startMenuBaseLayer_;
//    TileMap startMenuTextLayer_;
//
//
//    AnimSprite selectionTL_;
//    AnimSprite selectionTR_;
//    AnimSprite selectionBL_;
//    AnimSprite selectionBR_;
//    Camera2D cam;
//
//
//};
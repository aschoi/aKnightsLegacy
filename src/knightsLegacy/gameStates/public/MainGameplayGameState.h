#pragma once
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include <vector>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"
#include "AChoiEngine/worldLayer/public/Map.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"

class GameObject;
class AudioManager;
class Skeleton;
class Player;
class Stun;
class Hud;
class SwordWaveProjectile;


class MainGameplayGameState : public GameStateInterface {

public:

    bool Init(SDL_Renderer* r, float appW, float appH, int tileSizeAsInt, float tileSizeAsFloat) override;
    bool Init(SDL_Renderer* appR, AudioManager* audMan, float appW, float appH, int tileSizeAsInt, float tileSizeAsFloat);

    void Shutdown() override;
    void HandleEvent(Keys key) override;
    void UpdateFixed(double dt, const bool* keys) override;
    void UpdateFrame(uint64_t now_ms) override;
    void Render(SDL_Renderer* r) override;

    void SkeletonBehavior(Skeleton* curSkeleton_);
    
    void buildArrows();  // For displaying Vector Map Arrows (in AI Componnent)

    float appW_pixels_ = 0;
    float appH_pixels_ = 0;
    int tileSizeAsInt_ = 0;
    float tileSizeAsFloat_ = 0.0f;

    float world_level1_w_ = 0.0f;
    float world_level1_h_ = 0.0f;

    int tempTimer = 0;

    SDL_Color whiteColor{ 255, 255, 255, 255 };
    SDL_Color blackColor{ 0, 0, 0, 255 };
    std::vector<SDL_Texture*> arrowTexures;
    

protected:

private:

    AudioManager* audioManager_ = nullptr;
    SDL_Renderer* r_ = nullptr;

    //TileMap baseLayer_;
    //TileMap floorLayer_;  
    Map newMapLayer_;

    Player* playerLayer_ = nullptr;
    SwordWaveProjectile* projectileLayer_ = nullptr;
    Stun* stunLayer_ = nullptr;

    Skeleton* skeletonOne_ = nullptr;
    Skeleton* skeletonTwo_ = nullptr;
    Skeleton* skeletonThree_ = nullptr;
    Skeleton* skeletonFour_ = nullptr;
    Skeleton* skeletonFive_ = nullptr;
    Skeleton* skeletonSix_ = nullptr;
    Skeleton* skeletonSeven_ = nullptr;
    
    Hud* hudLayer_ = nullptr;
    Camera2D cam;
    
    std::vector<std::vector<int>> costMap;
    std::vector<std::vector<std::pair<int, int>>> vectorMap;

    std::vector<GameObject*> entitiesList_;

};  
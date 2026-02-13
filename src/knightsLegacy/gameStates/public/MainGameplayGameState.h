#pragma once
#include "AChoiEngine/gameStateInterface/public/GameStateInterface.h"
#include <vector>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"
#include "AChoiEngine/worldLayer/public/MapObject.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "AChoiEngine/camera/public/CameraObject.h"
#include "AChoiEngine/camera/public/CameraSys.h"
#include "AChoiEngine/camera/public/Camera.h"


class ACE_GameObject;
class AudioManager;
class Skeleton;
class Player;
class Stun;
class Hud;
class SwordWaveProjectile;


class MainGameplayGameState : public ACE_GameStateInterface {

public:

    bool ACE_Init(SDL_Renderer* r, float appW, float appH, int tileSizeAsInt, float tileSizeAsFloat) override;
    bool Init(SDL_Renderer* appR, AudioManager* audMan, float appW, float appH, int tileSizeAsInt, float tileSizeAsFloat);

    void ACE_Shutdown() override;
    void ACE_HandleEvent(Keys key) override;
    void ACE_UpdateFixed(double dt, const bool* keys) override;
    void ACE_UpdateFrame(uint64_t now_ms) override;
    void ACE_Render(SDL_Renderer* r) override;

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
    
    int entityCount = 0;
    int skeletonCount = 0;

protected:

private:

    AudioManager* audioManager_ = nullptr;
    SDL_Renderer* r_ = nullptr;

    //TileMap baseLayer_;
    //TileMap floorLayer_;  
    ACE_MapObject newMapLayer_;

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
    ACE_Camera2D cam;
    ACE_Camera2D_Center camCenter;
    
    std::vector<std::vector<int>> costMap;
    std::vector<std::vector<std::pair<int, int>>> vectorMap;

    
    std::vector<ACE_GameObject*> entitiesList_;

    std::vector<Skeleton*> skeletonsList_;



};  
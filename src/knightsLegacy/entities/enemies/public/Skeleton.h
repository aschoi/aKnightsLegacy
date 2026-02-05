#pragma once
#include "AChoiEngine/gameObject/public/GameObject.h"
#include "AChoiEngine/gfx/public/AnimationComponent.h"
#include "AChoiEngine/camera/public/Camera.h"

enum class Mood{ Chill, Patrol, Following, Pissed };
enum class Intelligence{ Smart, Dumb };

class Skeleton : public GameObject{

public:
    Skeleton(   float posX_px, float posY_px, float w_pixels, float h_pixels,
                float appW_pixels, float appH_pixels, float worldW_pixels, float worldH_pixels,
                int obj_w_gridUnits, int obj_h_gridUnits, int guSideLen_inPixels,
                float speed_pixels, float direction, int health, int damage)
                : GameObject(ObjectType::Enemy, AliveState::IsAlive,
                    posX_px, posY_px, w_pixels, h_pixels, 
                    appW_pixels, appH_pixels, worldW_pixels, worldH_pixels,
                    obj_w_gridUnits, obj_h_gridUnits, guSideLen_inPixels,
                    speed_pixels, direction, health, damage) { }

    bool Init(SDL_Renderer* appR, float spawnX_px, float spawnY_px, Facing spawnDirection, Intelligence iq, int health);

    void Shutdown();
    void HandleEvent(const SDL_Event& event);
    void UpdateFixed(double dt, const bool* keys);
    void UpdateFrame(uint64_t now_ms);
    void Render(SDL_Renderer* appR, Camera2D& cam) const;
    AnimSprite getIdle();
    AnimSprite getAttack();
    void collisionResponse();
    void takesDamage(int dmgAmount);
    void takesStun(uint64_t now, uint64_t stunLength);

    bool playerNearby = false;
    
    uint64_t INVINCIBLE_DURATION = 400;
    uint64_t invincibleTimer = 0;
    Facing facing = Facing::Right;
    EnemyState curEnemyState = EnemyState::Idle;
    Mood curMood = Mood::Chill;
    Intelligence iq_ = Intelligence::Dumb;

    uint64_t stunStartTimer_ms = 0;
    uint64_t stunMaxTimer_ms = 0;    

private:

    AnimSprite skeletonIdle_;
    AnimSprite skeletonAttack_;
    AnimSprite skeletonDead_;
    AnimSprite skeletonTakesDamage_;
    AnimSprite skeletonAttack2_;
    AnimSprite skeletonMove_;

    AnimGif stunnedAnimation_;
};



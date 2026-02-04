#pragma once
#include "AChoiEngine/gameObject/public/GameObject.h"
#include <cstdint>
#include "AChoiEngine/gfx/public/AnimationComponent.h"
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/camera/public/Camera.h"


class Map;
class TileMap;

class Player : public GameObject {

public:
    Player( 
        float posX_px, float posY_px, float obj_w_pixels, float obj_h_pixels,
        float appW_pixels, float appH_pixels, float worldW_pixels, float worldH_pixels,
        int obj_w_gridUnits, int obj_h_gridUnits, int guSideLen_inPixels,
        float speed_pixels, float direction, int health, int damage)
        // obj type, x, y, w, h, appW, appH, worldW, worldH, isAlive, gravity
        : GameObject(ObjectType::Player, AliveState::IsAlive,
            posX_px, posY_px, obj_w_pixels, obj_h_pixels, 
            appW_pixels, appH_pixels, worldW_pixels, worldH_pixels, 
            obj_w_gridUnits, obj_h_gridUnits, guSideLen_inPixels,
            speed_pixels, direction, health, damage) {}

    bool Init(SDL_Renderer* appR, float spawnX_px, float spawnY_px);

    void Shutdown();
    void HandleEvent(Keys key);
    void UpdateFixed(double dt, const bool* keys, TileMap& m);
    void UpdateFixed(double dt, const bool* keys);
    void UpdateFixed(double dt, const bool* keys, Map& solids);

    void UpdateFrame(uint64_t now_ms);
    void Render(SDL_Renderer* appR, Camera2D& cam) const;
    float getPx();
    float getPy();
    float getPlayerHeight();
    float getPlayerWidth();
    bool canFire(float& duration, float& timer, float deltaTime);
    uint64_t getSlideTimer();
    uint64_t getRollTimer();
    uint64_t getProjectileTimer();
    uint64_t getHammerTimer() const;
    uint64_t getInvincibleTimer();
    uint64_t getInvincibleDuration();
    int getPlayerHealth() const;

    void setInvincibleTimer(uint64_t newTime);
    void takesDamage(int dmgAmount);

    Facing facing = Facing::Right;
    uint64_t stunnedTimer = 0;
    
private:
    // Base stats
    int health_ = 50;
    float BASE_SPEED_ = 160.0f;
    //bool isAlive_ = true;

    // input flags
    bool attackPressed_ = false;
    bool runPressed_ = false;
    bool rollPressed_ = false;
    bool slidePressed_ = false;
    bool projectilePressed_ = false;
    bool hammerPressed_ = false;

    // utility variables
    //bool isMoving_ = false;
    bool crouchActivated_ = false;

    uint64_t INVINCIBLE_DURATION_ = 400;
    uint64_t invincibleTimer_ = 0;

    // SLIDE rules
    float SLIDE_SPEED_ = 400.0f;
    uint64_t SLIDE_DURATION_ = 600;
    uint64_t slideTimer_ = 0;

    // STUN HAMMER rules
    float HAMMER_SPEED_ = 0.0f;
    uint64_t HAMMER_DURATION_ = 2000;
    uint64_t hammerTimer_ = 0;

    // ROLL rules
    float ROLL_SPEED_ = 350.0f;
    uint64_t ROLL_DURATION_ = 1000;
    uint64_t rollTimer_ = 0;

    // PROJECTILE rules
    float PROJECTILE_SPEED_ = 0.0f;
    uint64_t PROJECTILE_DURATION_ = 3000;
    uint64_t projectileTimer_ = 0;


    // Animations
    enum class PlayerAnimState { Idle, Run, Attack, Crouch, Roll, Slide, TakesHit, Dead, Projectile };
    PlayerAnimState playerAnimState_ = PlayerAnimState::Idle;
    AnimGif playerIdle_;
    AnimGif playerRun_;
    AnimGif playerAttack_;
    AnimGif playerCrouch_;
    AnimGif playerRoll_;
    AnimGif playerSlide_;
    AnimGif playerTakesHit_;
    AnimGif playerDeath_;
    AnimGif playerProjectile_;

};
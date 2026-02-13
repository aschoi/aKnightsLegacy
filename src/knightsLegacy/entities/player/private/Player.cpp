#include "knightsLegacy/configs/configs.h"

#include "knightsLegacy/entities/player/public/Player.h"
#include <iostream>
#include <string>
#include <vector>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/physics/public/OverlappingHitboxDetectionSys.h"
#include "AChoiEngine/worldLayer/public/MapObject.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "AChoiEngine/camera/public/CameraObject.h"
#include "AChoiEngine/camera/public/CameraSys.h"
#include "knightsLegacy/weapons/public/Projectile.h"
#include "knightsLegacy/world/public/TileCatalog.h"
#include "assets.h"


bool Player::Init(SDL_Renderer* appR, float spawnX_px, float spawnY_px) {

    set_x_px(spawnX_px);
    set_y_px(spawnY_px);
    set_center_px();
    set_speed_pixels(BASE_SPEED_);
    set_x_gu(static_cast<int>(std::floor(spawnX_px / 16)));
    set_y_gu(static_cast<int>(std::floor(spawnY_px / 16)));

    // Load IDLE animation
    playerIdle_ = ACE_LoadAnimGif(appR, playerIdlePath.c_str(), 60);
    if (playerIdle_.frames.empty()) {
        std::cerr << "Failed to load idle GIF: " << playerIdlePath << "\n";
        return false;
    }
    // Loop IDLE animation 
    ACE_PlayAnimGif(playerIdle_, SDL_GetTicks(), true);

    if (!playerAttackPath.empty()) {  // Load ATTACK animation
        playerAttack_ = ACE_LoadAnimGif(appR, playerAttackPath.c_str(), 60);
        if (playerAttack_.frames.empty()) std::cerr << "Warning: failed to load attack GIF: " << playerAttackPath << "\n";
    }
    if (!playerRunPath.empty()) {  // RUNNING
        playerRun_ = ACE_LoadAnimGif(appR, playerRunPath.c_str(), 70);
        if (playerRun_.frames.empty()) std::cerr << "Warning: failed to load running GIF: " << playerRunPath << "\n";
    }
    if (!playerCrouchPath.empty()) {  // CROUCH
        playerCrouch_ = ACE_LoadAnimGif(appR, playerCrouchPath.c_str(), 100);
        if (playerCrouch_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerCrouchPath << "\n";
    }
    if (!playerRollPath.empty()) {  // ROLL 
        playerRoll_ = ACE_LoadAnimGif(appR, playerRollPath.c_str(), 40);
        if (playerRoll_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerRollPath << "\n";
    }
    if (!playerSlidePath.empty()) {  // SLIDE 
        playerSlide_ = ACE_LoadAnimGif(appR, playerSlidePath.c_str(), 70);
        if (playerSlide_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerSlidePath << "\n";
    }
    if (!playerTakesHitPath.empty()) {  // TAKES HIT 
        playerTakesHit_ = ACE_LoadAnimGif(appR, playerTakesHitPath.c_str(), 100);
        if (playerTakesHit_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerTakesHitPath << "\n";
    }
    if (!playerDeathPath.empty()) {  // DEATH 
        playerDeath_ = ACE_LoadAnimGif(appR, playerDeathPath.c_str(), 100);
        if (playerDeath_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerDeathPath << "\n";
    }

    if (!playerTripleSwingGifPath.empty()) {  // PROJECTILE
        playerProjectile_ = ACE_LoadAnimGif(appR, playerTripleSwingGifPath.c_str(), 85);
        if (playerProjectile_.frames.empty()) std::cerr << "Warning: failed to load running GIF: " << playerTripleSwingGifPath << "\n";
    }

    // Initialize Collision Hitboxes (Inherited from GameObject superclass)
    set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px(), get_y_px(), 18.0f, 40.0f);
    //set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px(), get_y_px(), get_w_pixels(), get_h_pixels());
    set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px(), get_y_px(), 18.0f, 40.0f);
    set_hitbox_pixels(HitboxType::AttackRange, get_x_px(), get_y_px(), 0.0f, 0.0f);
    set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px()-8.0f, get_y_px(), 28.0f, 48.0f);

    return true;
}

void Player::Shutdown() {
    ACE_DestroyAnimGif(playerCrouch_);
    ACE_DestroyAnimGif(playerAttack_);
    ACE_DestroyAnimGif(playerRun_);
    ACE_DestroyAnimGif(playerIdle_);
    ACE_DestroyAnimGif(playerRoll_);
    ACE_DestroyAnimGif(playerSlide_);
    ACE_DestroyAnimGif(playerTakesHit_);
    ACE_DestroyAnimGif(playerDeath_);
    ACE_DestroyAnimGif(playerProjectile_);
}

void Player::HandleEvent(Keys key) {
    uint64_t now = SDL_GetTicks();

    if (key == Keys::Space) {
        attackPressed_ = true;
        playerAnimState_ = PlayerAnimState::Attack;
    }
    else if (key == Keys::Q && now >= slideAvailTime_) {
        slidePressed_ = true;
        playerAnimState_ = PlayerAnimState::Slide;
        slideAvailTime_ = now + SLIDE_DURATION_;
    }
    else if (key == Keys::W && now >= hammerAvailTime_) {
        hammerPressed_ = true;
        hammerAvailTime_ = now + HAMMER_DURATION_;
    }
    else if (key == Keys::E && now >= rollAvailTime_) {
        rollPressed_ = true;
        playerAnimState_ = PlayerAnimState::Roll;
        rollAvailTime_ = now + ROLL_DURATION_;
    }
    else if (key == Keys::R && now >= projectileAvailTime_) {
        projectilePressed_ = true;
        playerAnimState_ = PlayerAnimState::Projectile;
        projectileAvailTime_ =  now + PROJECTILE_DURATION_;
    } 
}

// Update player position
void Player::UpdateFixed(double dt, const bool* keys, ACE_TileMap& floorLayer_) {

    float vx = 0.0f;
    float vy = 0.0f;

    if (playerAnimState_ == PlayerAnimState::Slide) {
        set_speed_pixels(SLIDE_SPEED_);
        if (facing == Facing::Left) vx -= 1.0f;
        else if (facing == Facing::Right) vx += 1.0f;
    }
    else if (playerAnimState_ == PlayerAnimState::Projectile) {
        set_speed_pixels(PROJECTILE_SPEED_);
        if (facing == Facing::Left) vx -= 1.0f;
        else if (facing == Facing::Right) vx += 1.0f;
    }
    else { //(animState_ != PlayerAnimState::Slide) {
        if (playerAnimState_ == PlayerAnimState::Roll) set_speed_pixels(ROLL_SPEED_);
        else set_speed_pixels(BASE_SPEED_);

        if (keys[SDL_SCANCODE_UP]) vy -= 1.0f;
        if (keys[SDL_SCANCODE_DOWN]) vy += 1.0f;
        if (keys[SDL_SCANCODE_LEFT]) vx -= 1.0f;
        if (keys[SDL_SCANCODE_RIGHT]) vx += 1.0f;
    }

    if (collisionStates_.n) vy += 1.0f;
    if (collisionStates_.s) vy -= 1.0f;
    if (collisionStates_.w) vx += 1.0f;
    if (collisionStates_.e) vx -= 1.0f;

    if (vx != 0.0f && vy != 0.0f) {
        vx *= 0.70710678f;
        vy *= 0.70710678f;
    }

    // update facing based on horizontal intent
    if (vx < 0.0f) facing = Facing::Left;
    else if (vx > 0.0f) facing = Facing::Right;

    if (vx != 0.0f || vy != 0.0f) set_moving_state(MovingState::IsMoving);
    else set_moving_state(MovingState::NotMoving);

    if (playerAttack_.playing) playerAnimState_ = PlayerAnimState::Attack;
    else if (playerSlide_.playing) playerAnimState_ = PlayerAnimState::Slide;
    else if (playerProjectile_.playing) playerAnimState_ = PlayerAnimState::Projectile;
    else if (playerRoll_.playing) playerAnimState_ = PlayerAnimState::Roll;
    else if (get_moving_state() == MovingState::IsMoving) playerAnimState_ = PlayerAnimState::Run;
    else playerAnimState_ = PlayerAnimState::Idle;

    float oldX = get_x_px();
    float oldY = get_y_px();

    set_x_px(get_x_px() + vx * get_speed_pixels() * (float)dt);
    set_y_px(get_y_px() + vy * get_speed_pixels() * (float)dt);

    if (floorLayer_.ACE_IsSolidAt(get_x_px(), get_y_px(), GetTileCatalog())) {
        if (floorLayer_.ACE_DoesDamageAt(get_x_px(), get_y_px(), GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.ACE_IsSolidAt(get_x_px() + 8, get_y_px(), GetTileCatalog())) {
        if (floorLayer_.ACE_DoesDamageAt(get_x_px() + 8, get_y_px(), GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.ACE_IsSolidAt(get_x_px(), get_y_px() + 36, GetTileCatalog())) {
        if (floorLayer_.ACE_DoesDamageAt(get_x_px(), get_y_px() + 36, GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.ACE_IsSolidAt(get_x_px()+ 8, get_y_px() + 36, GetTileCatalog())) {
        if (floorLayer_.ACE_DoesDamageAt(get_x_px() + 8, get_y_px() + 36, GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    set_center_px();

    set_x_gu(static_cast<int>(std::floor(get_x_px() / get_singleGU_sideLen_inPixels())));
    set_y_gu(static_cast<int>(std::floor(get_y_px() / get_singleGU_sideLen_inPixels())));

    // update facing based on horizontal intent
    if (facing == Facing::Right) {
        set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox));
        set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::AwarenessRange), get_hitbox_h_pixels(HitboxType::AwarenessRange));
        set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1));
    }
    else if (facing == Facing::Left) {
        set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px() - 4.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox));
        set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px() + 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::AwarenessRange), get_hitbox_h_pixels(HitboxType::AwarenessRange));
        set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px() + 9.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1));
    }

    // update attack range box depending on if attacking or not attacking
    if (playerAnimState_ == PlayerAnimState::Attack) {
        if (facing == Facing::Right) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px() + 16.0f, get_y_px(), 38.0f, 40.0f);
        }
        else if (facing == Facing::Left) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px() - 54.0f, get_y_px(), 38.0f, 40.0f);
        }
    }
    else {
        set_hitbox_pixels(HitboxType::AttackRange, 0.0f, 0.0f, 0.0f, 0.0f);
    }
}

// Update player position
void Player::UpdateFixed(double dt, const bool* keys) {

    float vx = 0.0f;
    float vy = 0.0f;

    if (playerAnimState_ == PlayerAnimState::Slide) {
        set_speed_pixels(SLIDE_SPEED_);
        if (facing == Facing::Left) vx -= 1.0f;
        else if (facing == Facing::Right) vx += 1.0f;
    }
    else if (playerAnimState_ == PlayerAnimState::Projectile) {
        set_speed_pixels(PROJECTILE_SPEED_);
        if (facing == Facing::Left) vx -= 1.0f;
        else if (facing == Facing::Right) vx += 1.0f;
    }
    else { //(animState_ != PlayerAnimState::Slide) {
        if (playerAnimState_ == PlayerAnimState::Roll) set_speed_pixels(ROLL_SPEED_);
        else set_speed_pixels(BASE_SPEED_);

        if (keys[SDL_SCANCODE_UP]) vy -= 1.0f;
        if (keys[SDL_SCANCODE_DOWN]) vy += 1.0f;
        if (keys[SDL_SCANCODE_LEFT]) vx -= 1.0f;
        if (keys[SDL_SCANCODE_RIGHT]) vx += 1.0f;
    }

    if (collisionStates_.n) vy += 1.0f;
    if (collisionStates_.s) vy -= 1.0f;
    if (collisionStates_.w) vx += 1.0f;
    if (collisionStates_.e) vx -= 1.0f;

    if (vx != 0.0f && vy != 0.0f) {
        vx *= 0.70710678f;
        vy *= 0.70710678f;
    }

    // update facing based on horizontal intent
    if (vx < 0.0f) facing = Facing::Left;
    else if (vx > 0.0f) facing = Facing::Right;

    if (vx != 0.0f || vy != 0.0f) set_moving_state(MovingState::IsMoving);  
    else set_moving_state(MovingState::NotMoving);

    if (playerAttack_.playing) playerAnimState_ = PlayerAnimState::Attack;
    else if (playerSlide_.playing) playerAnimState_ = PlayerAnimState::Slide;
    else if (playerProjectile_.playing) playerAnimState_ = PlayerAnimState::Projectile;
    else if (playerRoll_.playing) playerAnimState_ = PlayerAnimState::Roll;
    else if (get_moving_state() == MovingState::IsMoving) playerAnimState_ = PlayerAnimState::Run;
    else playerAnimState_ = PlayerAnimState::Idle;

    float oldX = get_x_px();
    float oldY = get_y_px();

    set_x_px(get_x_px() + vx * get_speed_pixels() * (float)dt);
    set_y_px(get_y_px() + vy * get_speed_pixels() * (float)dt);

    /*if (floorLayer_.IsSolidAt(get_x_px(), get_y_px())) {
        if (floorLayer_.DoesDamageAt(get_x_px(), get_y_px())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.IsSolidAt(get_x_px() + 8, get_y_px())) {
        if (floorLayer_.DoesDamageAt(get_x_px() + 8, get_y_px())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.IsSolidAt(get_x_px(), get_y_px() + 36)) {
        if (floorLayer_.DoesDamageAt(get_x_px(), get_y_px() + 36)) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.IsSolidAt(get_x_px() + 8, get_y_px() + 36)) {
        if (floorLayer_.DoesDamageAt(get_x_px() + 8, get_y_px() + 36)) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }*/

    set_center_px();

    set_x_gu(static_cast<int>(std::floor(get_x_px() / get_singleGU_sideLen_inPixels())));
    set_y_gu(static_cast<int>(std::floor(get_y_px() / get_singleGU_sideLen_inPixels())));


    // update facing based on horizontal intent
    if (facing == Facing::Right) {
        set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox));
        set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::AwarenessRange), get_hitbox_h_pixels(HitboxType::AwarenessRange));
        set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1));
    }
    else if (facing == Facing::Left) {
        set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px() - 4.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox));
        set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px() + 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::AwarenessRange), get_hitbox_h_pixels(HitboxType::AwarenessRange));
        set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px() + 9.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1));
    }

    // update attack range box depending on if attacking or not attacking
    if (playerAnimState_ == PlayerAnimState::Attack) {
        if (facing == Facing::Right) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px() + 16.0f, get_y_px(), 38.0f, 40.0f);
        }
        else if (facing == Facing::Left) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px() - 54.0f, get_y_px(), 38.0f, 40.0f);
        }
    }
    else {
        set_hitbox_pixels(HitboxType::AttackRange, 0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void Player::UpdateFixed(double dt, const bool* keys, ACE_MapObject& curMap) {

    float vx = 0.0f;
    float vy = 0.0f;
    uint64_t now = SDL_GetTicks();

    if (playerAnimState_ == PlayerAnimState::Slide) {
        set_speed_pixels(SLIDE_SPEED_);
        if (facing == Facing::Left) vx -= 1.0f;
        else if (facing == Facing::Right) vx += 1.0f;
    }
    else if (playerAnimState_ == PlayerAnimState::Projectile) {
        set_speed_pixels(PROJECTILE_SPEED_);
        if (facing == Facing::Left) vx -= 1.0f;
        else if (facing == Facing::Right) vx += 1.0f;
    }
    else { //(animState_ != PlayerAnimState::Slide) {
        if (playerAnimState_ == PlayerAnimState::Roll) set_speed_pixels(ROLL_SPEED_);
        else set_speed_pixels(BASE_SPEED_);

        if (keys[SDL_SCANCODE_UP]) vy -= 1.0f;
        if (keys[SDL_SCANCODE_DOWN]) vy += 1.0f;
        if (keys[SDL_SCANCODE_LEFT]) vx -= 1.0f;
        if (keys[SDL_SCANCODE_RIGHT]) vx += 1.0f;
    }
    
    if (collisionStates_.n) vy += 1.0f;
    if (collisionStates_.s) vy -= 1.0f;
    if (collisionStates_.w) vx += 1.0f;
    if (collisionStates_.e) vx -= 1.0f;

    if (vx != 0.0f && vy != 0.0f) {
        vx *= 0.70710678f;
        vy *= 0.70710678f;
    }

    // update facing based on horizontal intent
    if (vx < 0.0f) facing = Facing::Left;
    else if (vx > 0.0f) facing = Facing::Right;

    if (vx != 0.0f || vy != 0.0f) set_moving_state(MovingState::IsMoving);
    else set_moving_state(MovingState::NotMoving);

    if (playerAttack_.playing) playerAnimState_ = PlayerAnimState::Attack;
    else if (playerSlide_.playing) playerAnimState_ = PlayerAnimState::Slide;
    else if (playerProjectile_.playing) playerAnimState_ = PlayerAnimState::Projectile;
    else if (playerRoll_.playing) playerAnimState_ = PlayerAnimState::Roll;
    else if (get_moving_state() == MovingState::IsMoving) playerAnimState_ = PlayerAnimState::Run;
    else playerAnimState_ = PlayerAnimState::Idle;

    float oldX = get_x_px();
    float oldY = get_y_px();

    set_x_px(get_x_px() + vx * get_speed_pixels() * (float)dt);
    set_y_px(get_y_px() + vy * get_speed_pixels() * (float)dt);

#ifdef PERF_TEST_OFF
    int tl = curMap.ACE_intGridType(get_x_px(), get_y_px());
    int tr = curMap.ACE_intGridType(get_x_px() + 8, get_y_px());
    int bl = curMap.ACE_intGridType(get_x_px(), get_y_px() + 36);
    int br = curMap.ACE_intGridType(get_x_px() + 8, get_y_px() + 36);

    if ((0 < tl && tl < 3) || (0 < tr && tr < 3) || (0 < bl && bl < 3) || (0 < br && br < 3)) {
        set_x_px(oldX);
        set_y_px(oldY);
        if (tl == 2 || tr == 2 || bl == 2 || br == 2) {
            if (now >= getInvincibleTimer()) {
                takesDamage(3);
                setInvincibleTimer(getInvincibleDuration());
            }
        }
    }
#endif

    set_center_px();

    set_x_gu(static_cast<int>(std::floor(get_x_px() / get_singleGU_sideLen_inPixels())));
    set_y_gu(static_cast<int>(std::floor(get_y_px() / get_singleGU_sideLen_inPixels())));

    // update facing based on horizontal intent
    if (facing == Facing::Right) {
        set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox));
        set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px() - 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::AwarenessRange), get_hitbox_h_pixels(HitboxType::AwarenessRange));
        set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px() - 18.0f, get_y_px() - 4.0f, get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1));
    }
    else if (facing == Facing::Left) {
        set_hitbox_pixels(HitboxType::BodyHitbox, get_x_px() - 4.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox));
        set_hitbox_pixels(HitboxType::AwarenessRange, get_x_px() + 12.0f, get_y_px(), get_hitbox_w_pixels(HitboxType::AwarenessRange), get_hitbox_h_pixels(HitboxType::AwarenessRange));
        set_hitbox_pixels(HitboxType::UtilityRange1, get_x_px() - 8.0f, get_y_px() - 4.0f, get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1));    
    }

    // update attack range box depending on if attacking or not attacking
    if (playerAnimState_ == PlayerAnimState::Attack) {
        if (facing == Facing::Right) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px() + 16.0f, get_y_px(), 38.0f, 40.0f);
        }
        else if (facing == Facing::Left) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px() - 54.0f, get_y_px(), 38.0f, 40.0f);
        }
    }
    else {
        set_hitbox_pixels(HitboxType::AttackRange, 0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void Player::UpdateFrame(uint64_t now_ms) {

    if (get_alive_state() == AliveState::IsAlive) {

        if (invincibleFinishTime_ >= now_ms + INVINCIBLE_DURATION_) {
            ACE_StopAnimGif(playerIdle_);
            ACE_StopAnimGif(playerRun_);
            ACE_StopAnimGif(playerAttack_);
            ACE_StopAnimGif(playerCrouch_);
            ACE_StopAnimGif(playerRoll_);
            ACE_StopAnimGif(playerSlide_);
            ACE_StopAnimGif(playerProjectile_);

            if (!playerTakesHit_.playing) {
                ACE_PlayAnimGif(playerTakesHit_, now_ms, false);
            }
        }
        // initiate a player action if button pressed
        else if (projectilePressed_) {
            ACE_PlayAnimGif(playerProjectile_, now_ms, false);
            projectilePressed_ = false;
        }
        else if (attackPressed_) {
            ACE_PlayAnimGif(playerAttack_, now_ms, false);
            attackPressed_ = false;
        }
        else if (slidePressed_) {
            ACE_PlayAnimGif(playerSlide_, now_ms, false);
            slidePressed_ = false;
        }
        else if (rollPressed_) {
            ACE_PlayAnimGif(playerRoll_, now_ms, false);
            rollPressed_ = false;
        }
        else if (hammerPressed_) {
            hammerPressed_ = false;
        }

        // Return to idle after player action ends
        if (ACE_FinishedNonLoopingGif(playerProjectile_, now_ms))   ACE_StopAnimGif(playerProjectile_);
        if (ACE_FinishedNonLoopingGif(playerAttack_, now_ms))       ACE_StopAnimGif(playerAttack_);
        if (ACE_FinishedNonLoopingGif(playerSlide_, now_ms))        ACE_StopAnimGif(playerSlide_);
        if (ACE_FinishedNonLoopingGif(playerRoll_, now_ms))         ACE_StopAnimGif(playerRoll_);
        if (ACE_FinishedNonLoopingGif(playerTakesHit_, now_ms))     ACE_StopAnimGif(playerTakesHit_);

        if (!playerAttack_.playing && !playerSlide_.playing && !playerRoll_.playing 
            && !playerProjectile_.playing && !playerTakesHit_.playing) {

            static PlayerAnimState prevState = PlayerAnimState::Idle;
            if (playerAnimState_ != prevState) {
                if (playerAnimState_ == PlayerAnimState::Idle) {
                    ACE_PlayAnimGif(playerIdle_, now_ms, true);
                    ACE_StopAnimGif(playerRun_);
                    set_moving_state(MovingState::NotMoving);
                }
                else {
                    ACE_PlayAnimGif(playerRun_, now_ms, true);
                    ACE_StopAnimGif(playerIdle_);
                }
                prevState = playerAnimState_;
            }
        }
    }
}

void Player::Render(SDL_Renderer* appR, ACE_Camera2D& cam) const {

    const float halfW = playerIdle_.w * 0.5f;
    const float halfH = playerIdle_.h * 0.5f;
    bool flashRed = false;

    SDL_FRect dst{
        (get_x_px() - halfW),
        (get_y_px() - halfH),
        playerIdle_.w,
        playerIdle_.h
    };

    uint64_t now_ms = SDL_GetTicks();
    SDL_Texture* tex = nullptr;

    if (now_ms < invincibleFinishTime_ && invincibleFinishTime_ <= now_ms + INVINCIBLE_DURATION_)
        flashRed = true;

    if (playerTakesHit_.playing)        tex = ACE_CurrentFrameGif(playerTakesHit_, now_ms);
    else if (playerProjectile_.playing) tex = ACE_CurrentFrameGif(playerProjectile_, now_ms);

    else if (playerAttack_.playing)     tex = ACE_CurrentFrameGif(playerAttack_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Run)      tex = ACE_CurrentFrameGif(playerRun_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Slide)    tex = ACE_CurrentFrameGif(playerSlide_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Roll)     tex = ACE_CurrentFrameGif(playerRoll_, now_ms);
    else tex = ACE_CurrentFrameGif(playerIdle_, now_ms);

    if (!tex) return;

    SDL_FlipMode flip = (facing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (flashRed) SDL_SetTextureColorModFloat(tex, 2.5f, 1.0f, 1.0f);

    SDL_FRect screenDst = cam.ACE_WorldToScreen(dst);
    SDL_RenderTextureRotated(appR, tex, nullptr, &screenDst, 0.0, nullptr, flip);

    if (flashRed) SDL_SetTextureColorModFloat(tex, 1.0f, 1.0f, 1.0f);

    
    // DEBUG COLLISIONS
    /*SDL_FRect hitboxHighlightDst{ get_hitbox_x_px(HitboxType::UtilityRange1), get_hitbox_y_px(HitboxType::UtilityRange1),
                                  get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1) };
    SDL_FRect screenHitboxDst = cam.WorldToScreen(hitboxHighlightDst);
    SDL_SetRenderDrawColor(appR, 0, 255, 0, 40);
    SDL_RenderFillRect(appR, &screenHitboxDst);*/

    /*SDL_FRect hitboxHighlight2Dst{ get_hitbox_x_px(HitboxType::BodyHitbox), get_hitbox_y_px(HitboxType::BodyHitbox),
                              get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox) };
    SDL_FRect screenHitbox2Dst = cam.WorldToScreen(hitboxHighlight2Dst);
    SDL_SetRenderDrawColor(appR, 0, 0, 255, 40);
    SDL_RenderFillRect(appR, &screenHitbox2Dst);*/

}

void Player::Render(SDL_Renderer* appR, ACE_Camera2D_Center& cam) const {
    const float halfW = playerIdle_.w * 0.5f;
    const float halfH = playerIdle_.h * 0.5f;
    bool flashRed = false;

    SDL_FRect dst{
        (get_x_px() - halfW),
        (get_y_px() - halfH),
        playerIdle_.w,
        playerIdle_.h
    };

    uint64_t now_ms = SDL_GetTicks();
    SDL_Texture* tex = nullptr;

    if (now_ms < invincibleFinishTime_ && invincibleFinishTime_ <= now_ms + INVINCIBLE_DURATION_)
        flashRed = true;

    if (playerTakesHit_.playing)        tex = ACE_CurrentFrameGif(playerTakesHit_, now_ms);
    else if (playerProjectile_.playing) tex = ACE_CurrentFrameGif(playerProjectile_, now_ms);

    else if (playerAttack_.playing)     tex = ACE_CurrentFrameGif(playerAttack_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Run)      tex = ACE_CurrentFrameGif(playerRun_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Slide)    tex = ACE_CurrentFrameGif(playerSlide_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Roll)     tex = ACE_CurrentFrameGif(playerRoll_, now_ms);
    else tex = ACE_CurrentFrameGif(playerIdle_, now_ms);

    if (!tex) return;

    SDL_FlipMode flip = (facing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (flashRed) SDL_SetTextureColorModFloat(tex, 2.5f, 1.0f, 1.0f);

    SDL_FRect screenDst = ACE_Cam_WorldToScreen(cam, dst);
    SDL_RenderTextureRotated(appR, tex, nullptr, &screenDst, 0.0, nullptr, flip);

    if (flashRed) SDL_SetTextureColorModFloat(tex, 1.0f, 1.0f, 1.0f);


    // DEBUG COLLISIONS
    /*SDL_FRect hitboxHighlightDst{ get_hitbox_x_px(HitboxType::UtilityRange1), get_hitbox_y_px(HitboxType::UtilityRange1),
                                  get_hitbox_w_pixels(HitboxType::UtilityRange1), get_hitbox_h_pixels(HitboxType::UtilityRange1) };
    SDL_FRect screenHitboxDst = cam.WorldToScreen(hitboxHighlightDst);
    SDL_SetRenderDrawColor(appR, 0, 255, 0, 40);
    SDL_RenderFillRect(appR, &screenHitboxDst);*/

    /*SDL_FRect hitboxHighlight2Dst{ get_hitbox_x_px(HitboxType::BodyHitbox), get_hitbox_y_px(HitboxType::BodyHitbox),
                              get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox) };
    SDL_FRect screenHitbox2Dst = cam.WorldToScreen(hitboxHighlight2Dst);
    SDL_SetRenderDrawColor(appR, 0, 0, 255, 40);
    SDL_RenderFillRect(appR, &screenHitbox2Dst);*/
}


uint64_t Player::getSlideTimer() { return slideAvailTime_; }
uint64_t Player::getRollTimer() { return rollAvailTime_; }
uint64_t Player::getProjectileTimer() { return projectileAvailTime_; }
uint64_t Player::getInvincibleTimer() { return invincibleFinishTime_; }
uint64_t Player::getInvincibleDuration() { return INVINCIBLE_DURATION_; }
uint64_t Player::getHammerTimer() const { return hammerAvailTime_; }
int Player::getPlayerHealth() const { return health_; }


void Player::setInvincibleTimer(uint64_t duration) { invincibleFinishTime_ = SDL_GetTicks() + duration; }


void Player::takesDamage(int dmgAmount) {
    health_ -= dmgAmount;
    if (health_ < 0) {
        playerAnimState_ = PlayerAnimState::Dead;
    }
}


uint64_t Player::getSlide_remainCD() {
    uint64_t now = SDL_GetTicks();
    if (now >= slideAvailTime_) return 0;
    return slideAvailTime_ - now;
}   
uint64_t Player::getRollCD_remainCD() {
    uint64_t now = SDL_GetTicks();
    if (now >= rollAvailTime_) return 0;
    return rollAvailTime_ - now;
}
uint64_t Player::getProjectile_remainCD() {
    uint64_t now = SDL_GetTicks();
    if (now >= projectileAvailTime_) return 0;
    return projectileAvailTime_ - now;
}
uint64_t Player::getInvincible_remainCD() {
    uint64_t now = SDL_GetTicks();
    if (now >= invincibleFinishTime_) return 0;
    return invincibleFinishTime_ - now;
}
uint64_t Player::getHammer_remainCD() const {
    uint64_t now = SDL_GetTicks();
    if (now >= hammerAvailTime_) return 0;
    return hammerAvailTime_ - now;
}
    
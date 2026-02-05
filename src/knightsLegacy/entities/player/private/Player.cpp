#include "knightsLegacy/entities/player/public/Player.h"
#include <iostream>
#include <string>
#include <vector>
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/physics/public/CollisionDetectionComponent.h"
#include "AChoiEngine/worldLayer/public/Map.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
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
    playerIdle_ = LoadAnimGif(appR, playerIdlePath.c_str(), 60);
    if (playerIdle_.frames.empty()) {
        std::cerr << "Failed to load idle GIF: " << playerIdlePath << "\n";
        return false;
    }
    // Loop IDLE animation 
    PlayAnimGif(playerIdle_, SDL_GetTicks(), true);

    if (!playerAttackPath.empty()) {  // Load ATTACK animation
        playerAttack_ = LoadAnimGif(appR, playerAttackPath.c_str(), 60);
        if (playerAttack_.frames.empty()) std::cerr << "Warning: failed to load attack GIF: " << playerAttackPath << "\n";
    }
    if (!playerRunPath.empty()) {  // RUNNING
        playerRun_ = LoadAnimGif(appR, playerRunPath.c_str(), 70);
        if (playerRun_.frames.empty()) std::cerr << "Warning: failed to load running GIF: " << playerRunPath << "\n";
    }
    if (!playerCrouchPath.empty()) {  // CROUCH
        playerCrouch_ = LoadAnimGif(appR, playerCrouchPath.c_str(), 100);
        if (playerCrouch_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerCrouchPath << "\n";
    }
    if (!playerRollPath.empty()) {  // ROLL 
        playerRoll_ = LoadAnimGif(appR, playerRollPath.c_str(), 40);
        if (playerRoll_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerRollPath << "\n";
    }
    if (!playerSlidePath.empty()) {  // SLIDE 
        playerSlide_ = LoadAnimGif(appR, playerSlidePath.c_str(), 70);
        if (playerSlide_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerSlidePath << "\n";
    }
    if (!playerTakesHitPath.empty()) {  // TAKES HIT 
        playerTakesHit_ = LoadAnimGif(appR, playerTakesHitPath.c_str(), 100);
        if (playerTakesHit_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerTakesHitPath << "\n";
    }
    if (!playerDeathPath.empty()) {  // DEATH 
        playerDeath_ = LoadAnimGif(appR, playerDeathPath.c_str(), 100);
        if (playerDeath_.frames.empty()) std::cerr << "Warning: failed to load crouch GIF: " << playerDeathPath << "\n";
    }

    if (!playerTripleSwingGifPath.empty()) {  // PROJECTILE
        playerProjectile_ = LoadAnimGif(appR, playerTripleSwingGifPath.c_str(), 85);
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
    DestroyAnimGif(playerCrouch_);
    DestroyAnimGif(playerAttack_);
    DestroyAnimGif(playerRun_);
    DestroyAnimGif(playerIdle_);
    DestroyAnimGif(playerRoll_);
    DestroyAnimGif(playerSlide_);
    DestroyAnimGif(playerTakesHit_);
    DestroyAnimGif(playerDeath_);
    DestroyAnimGif(playerProjectile_);
}

void Player::HandleEvent(Keys key) {

    if (key == Keys::Space) {
        attackPressed_ = true;
        playerAnimState_ = PlayerAnimState::Attack;
    }
    if (key == Keys::Q && slideTimer_ <= 0) {
        slidePressed_ = true;
        playerAnimState_ = PlayerAnimState::Slide;
        slideTimer_ = SLIDE_DURATION_;
    }
    if (key == Keys::W && hammerTimer_ <= 0) {
        hammerPressed_ = true;
        hammerTimer_ = HAMMER_DURATION_;
    }
    if (key == Keys::E && rollTimer_ <= 0) {
        rollPressed_ = true;
        playerAnimState_ = PlayerAnimState::Roll;
        rollTimer_ = ROLL_DURATION_;
    }
    if (key == Keys::R && projectileTimer_ <= 0) {
        projectilePressed_ = true;
        playerAnimState_ = PlayerAnimState::Projectile;
        projectileTimer_ = PROJECTILE_DURATION_;
    } 
}

// Update player position
void Player::UpdateFixed(double dt, const bool* keys, TileMap& floorLayer_) {

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

    if (floorLayer_.IsSolidAt(get_x_px(), get_y_px(), GetTileCatalog())) {
        if (floorLayer_.DoesDamageAt(get_x_px(), get_y_px(), GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.IsSolidAt(get_x_px() + 8, get_y_px(), GetTileCatalog())) {
        if (floorLayer_.DoesDamageAt(get_x_px() + 8, get_y_px(), GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.IsSolidAt(get_x_px(), get_y_px() + 36, GetTileCatalog())) {
        if (floorLayer_.DoesDamageAt(get_x_px(), get_y_px() + 36, GetTileCatalog())) {
            setInvincibleTimer(getInvincibleDuration());
            takesDamage(1);
        }
        set_x_px(oldX);
        set_y_px(oldY);
    }
    else if (floorLayer_.IsSolidAt(get_x_px()+ 8, get_y_px() + 36, GetTileCatalog())) {
        if (floorLayer_.DoesDamageAt(get_x_px() + 8, get_y_px() + 36, GetTileCatalog())) {
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

void Player::UpdateFixed(double dt, const bool* keys, Map& curMap) {

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

    int tl = curMap.intGridType(get_x_px(), get_y_px());
    int tr = curMap.intGridType(get_x_px() + 8, get_y_px());
    int bl = curMap.intGridType(get_x_px(), get_y_px() + 36);
    int br = curMap.intGridType(get_x_px() + 8, get_y_px() + 36);

    if ((0 < tl && tl < 3) || (0 < tr && tr < 3) || (0 < bl && bl < 3) || (0 < br && br < 3)) {
        set_x_px(oldX);
        set_y_px(oldY);
        if (tl == 2 || tr == 2 || bl == 2 || br == 2) {
            if (getInvincibleTimer() <= 0) {
                takesDamage(3);
                setInvincibleTimer(getInvincibleDuration());
            }
        }
    }
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
        if (slideTimer_ > 0) {
            slideTimer_ -= 1;
        }
        if (rollTimer_ > 0) {
            rollTimer_ -= 1;
        }
        if (projectileTimer_ > 0) {
            projectileTimer_ -= 1;
        }
        if (hammerTimer_ > 0) {
            hammerTimer_ -= 1;
        }


        if (get_alive_state() == AliveState::IsAlive && invincibleTimer_ > 0) {
            invincibleTimer_ -= 1;
            StopAnimGif(playerIdle_);
            StopAnimGif(playerRun_);
            StopAnimGif(playerAttack_);
            StopAnimGif(playerCrouch_);
            StopAnimGif(playerRoll_);
            StopAnimGif(playerSlide_);
            StopAnimGif(playerProjectile_);

            if (!playerTakesHit_.playing) {
                PlayAnimGif(playerTakesHit_, now_ms, false);
            }
        }
        // initiate a player action if button pressed
        else if (projectilePressed_) {
            PlayAnimGif(playerProjectile_, now_ms, false);
            projectilePressed_ = false;
        }
        else if (attackPressed_) {
            PlayAnimGif(playerAttack_, now_ms, false);
            attackPressed_ = false;
        }
        else if (slidePressed_) {
            PlayAnimGif(playerSlide_, now_ms, false);
            slidePressed_ = false;
        }
        else if (rollPressed_) {
            PlayAnimGif(playerRoll_, now_ms, false);
            rollPressed_ = false;
        }
        else if (hammerPressed_) {
            hammerPressed_ = false;
        }

        // Return to idle after player action ends
        if (FinishedNonLoopingGif(playerProjectile_, now_ms))   StopAnimGif(playerProjectile_);
        if (FinishedNonLoopingGif(playerAttack_, now_ms))       StopAnimGif(playerAttack_);
        if (FinishedNonLoopingGif(playerSlide_, now_ms))        StopAnimGif(playerSlide_);
        if (FinishedNonLoopingGif(playerRoll_, now_ms))         StopAnimGif(playerRoll_);
        if (FinishedNonLoopingGif(playerTakesHit_, now_ms))     StopAnimGif(playerTakesHit_);

        if (!playerAttack_.playing && !playerSlide_.playing && !playerRoll_.playing 
            && !playerProjectile_.playing && !playerTakesHit_.playing) {

            static PlayerAnimState prevState = PlayerAnimState::Idle;
            if (playerAnimState_ != prevState) {
                if (playerAnimState_ == PlayerAnimState::Idle) {
                    PlayAnimGif(playerIdle_, now_ms, true);
                    StopAnimGif(playerRun_);
                    set_moving_state(MovingState::NotMoving);
                }
                else {
                    PlayAnimGif(playerRun_, now_ms, true);
                    StopAnimGif(playerIdle_);
                }
                prevState = playerAnimState_;
            }
        }
    }
}

void Player::Render(SDL_Renderer* appR, Camera2D& cam) const {

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

    if (invincibleTimer_ > 0) flashRed = true;

    if (playerTakesHit_.playing)        tex = CurrentFrameGif(playerTakesHit_, now_ms);
    else if (playerProjectile_.playing) tex = CurrentFrameGif(playerProjectile_, now_ms);

    else if (playerAttack_.playing)     tex = CurrentFrameGif(playerAttack_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Run)      tex = CurrentFrameGif(playerRun_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Slide)    tex = CurrentFrameGif(playerSlide_, now_ms);
    else if (playerAnimState_ == PlayerAnimState::Roll)     tex = CurrentFrameGif(playerRoll_, now_ms);
    else tex = CurrentFrameGif(playerIdle_, now_ms);

    if (!tex) return;

    SDL_FlipMode flip = (facing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    if (flashRed) SDL_SetTextureColorModFloat(tex, 2.5f, 1.0f, 1.0f);

    SDL_FRect screenDst = cam.WorldToScreen(dst);
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

uint64_t Player::getSlideTimer() { return slideTimer_; }
uint64_t Player::getRollTimer() { return rollTimer_; }
uint64_t Player::getProjectileTimer() { return projectileTimer_; }
uint64_t Player::getInvincibleTimer() { return invincibleTimer_; }
uint64_t Player::getInvincibleDuration() { return INVINCIBLE_DURATION_; }
uint64_t Player::getHammerTimer() const { return hammerTimer_; }
int Player::getPlayerHealth() const { return health_; }

void Player::setInvincibleTimer(uint64_t newTime) { invincibleTimer_ = newTime; }

void Player::takesDamage(int dmgAmount) {
    health_ -= dmgAmount;
    if (health_ < 0) {
        playerAnimState_ = PlayerAnimState::Dead;
    }
}



    
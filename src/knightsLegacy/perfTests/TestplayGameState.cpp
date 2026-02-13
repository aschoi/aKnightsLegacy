#include "knightsLegacy/configs/configs.h"

#include "knightsLegacy/gameStates/public/MainGameplayGameState.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <cstdint>
#include <vector>
#include <random>
#include <unordered_set>
#include <stdexcept>
#include "AChoiEngine/physics/public/OverlappingHitboxDetectionSys.h"
#include "AChoiEngine/physics/public/CollisionResponseSys.h"
#include "AChoiEngine/ai/public/aiPathfindingSys.h"
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/camera/public/Camera.h"
#include "AChoiEngine/camera/public/CameraSys.h"
#include "AChoiEngine/camera/public/CameraObject.h"
#include "knightsLegacy/core/public/GameManager.h"
#include "knightsLegacy/audioManager/public/audioManager.h"
#include "knightsLegacy/entities/player/public/Player.h"
#include "knightsLegacy/entities//enemies/public/Skeleton.h"
#include "knightsLegacy/weapons/public/Projectile.h"
#include "knightsLegacy/weapons/public/Stun.h"
#include "knightsLegacy/hud/public/Hud.h"
#include "knightsLegacy/world/public/TileCatalog.h"
#include "assets.h"


#ifdef PERF_TEST_ON

static uint64_t packXY(int x, int y) {
    // Pack two 32-bit signed ints into 64-bit key (stable for hashing).
    return (uint64_t)(uint32_t)x << 32 | (uint32_t)y;
}

bool MainGameplayGameState::ACE_Init(SDL_Renderer* appR, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {
    return false;
}

bool MainGameplayGameState::Init(SDL_Renderer* appR, AudioManager* audMan, float appW_pixels, float appH_pixels, int tileSizeAsInt, float tileSizeAsFloat) {

    appW_pixels_ = appW_pixels;
    appH_pixels_ = appH_pixels;
    tileSizeAsInt_ = tileSizeAsInt;
    tileSizeAsFloat_ = tileSizeAsFloat;
    r_ = appR;
    audioManager_ = audMan;

    //newMapLayer_.ACE_Init(appR, jsonLevel1v2.c_str());
    newMapLayer_.ACE_Init(appR, wallsMap3008x3008.c_str());
    world_level1_w_ = newMapLayer_.getWorldWidth_pixels();
    world_level1_h_ = newMapLayer_.getWorldHeight_pixels();

    // GameObject Parameter Guide:
    // (posX_pixel, posY_pixel, width_pixels, height_pixels, app_width_pixels, app_height_pixels,
    //  world_width_pixels, world_height_pixels, obj_width_gUnits, obj_height_gUnits, gUnit_side_length_InPixels,
    //  speed_pixels, direction, heatlh, damage)
    // INSTANTIATE PLAYER AND INIT 
    playerLayer_ = new Player(appW_pixels_ * 0.5f, appH_pixels_ * 0.5f, 3.0f, 3.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        7, 5, tileSizeAsInt_, 0, 0, 5, 1);
    playerLayer_->Init(appR, appW_pixels_ * 0.5f, appH_pixels_ * 0.5f);
    entityCount += 1;


    // INSTANTIATE SKELETON AND INTIS
    skeletonCount = 500;
    entityCount += skeletonCount;
    
    uint32_t seed = std::random_device{}();
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distX(2, newMapLayer_.getWorldWidth_gridUnits() - 5);
    std::uniform_int_distribution<int> distY(2, newMapLayer_.getWorldHeight_gridUnits() - 5);
    std::unordered_set<uint64_t> usedGrids;
    usedGrids.reserve((size_t)skeletonCount * 2);

    for (int r = playerLayer_->get_y_gu() - 3; r < playerLayer_->get_y_gu() + playerLayer_->get_h_gridUnits() + 3; r++) {
        for (int c = playerLayer_->get_x_gu() - 3; c < playerLayer_->get_x_gu() + playerLayer_->get_w_gridUnits() + 3; c++) {
            uint64_t key = packXY(c, r);
            usedGrids.insert(key);
        }
    }

    int curCount = 0;
    const int maxAttempts = skeletonCount * 200; // tune as you like
    int attempts = 0;
    while (curCount < skeletonCount && attempts < maxAttempts) {
        ++attempts;

        int tx = distX(rng);
        int ty = distY(rng);

        bool goodToGo = true;
        for (int r = ty; r < ty + 2; ++r) {
            for (int c = tx; c < tx + 2; ++c) {
                if (newMapLayer_.ACE_intGridType(c * tileSizeAsFloat_, r * tileSizeAsFloat_) == 1 
                    || usedGrids.contains(packXY(c, r))) {
                    goodToGo = false;
                    break;
                }
            }
        }
        if (!goodToGo) continue;
        for (int r = ty; r < ty + 2; ++r) {
            for (int c = tx; c < tx + 2; ++c) {
                uint64_t key = packXY(c, r);
                usedGrids.insert(key);
            }
        }

        Skeleton* skeleton_ = new Skeleton(tx * tileSizeAsFloat, ty * tileSizeAsFloat, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
            2, 2, tileSizeAsInt_, 0, 0, 5, 1);
        skeleton_->Init(appR, tx * tileSizeAsFloat, ty * tileSizeAsFloat, Facing::Right, Intelligence::Smart, 4);
        skeleton_->curMood = Mood::Pissed;

        skeletonsList_.push_back(skeleton_);
        ++curCount;
        
    }

    // INSTANTIATE ULT
    projectileLayer_ = new SwordWaveProjectile(0, 0, 3.0f, 3.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        8, 8, tileSizeAsInt_, 0, 0, 0, 3);
    
    // INSTANTIATE STUN
    stunLayer_ = new Stun(0, 0, 3.0f, 3.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        8, 8, tileSizeAsInt_, 0, 0, 0, 1);

    // Set Camera parameters
    /*cam.ACE_SetViewport(appW_pixels_, appH_pixels_);
    cam.ACE_SetWorldBounds(world_level1_w_, world_level1_h_);*/


    ACE_Cam_SetScreenSize(camCenter, appW_pixels_, appH_pixels_);
    ACE_Cam_SetViewport(camCenter, appW_pixels_, appH_pixels_);

    ACE_Cam_SetWorldBounds(camCenter, world_level1_w_, world_level1_h_);
    //camCenter.viewWidth_pixels /= 2.0f;
    //camCenter.viewHeight_pixels /= 2.0f;
    //ACE_Cam_SetZoom(camCenter, 1);
    //ACE_Cam_Zoom(camCenter, 2.0f);
    // Instantiate hud and init with parameters
    hudLayer_ = new Hud();
    //hudLayer_->Init(appR, appW_pixels_, appH_pixels_, cam, *playerLayer_);
    hudLayer_->Init(appR, appW_pixels_, appH_pixels_, camCenter, *playerLayer_);

    entitiesList_.reserve(entityCount * 2);
    for (Skeleton* s : skeletonsList_) {
        entitiesList_.push_back(s);
    }
    entitiesList_.push_back(playerLayer_);

    audioManager_->ACE_LoadPlayMusic(BackgroundMusic::MainGameplay);


#if SHOW_VECTOR_MAP
    buildArrows();
#endif

    return true;
}


void MainGameplayGameState::ACE_Shutdown() {

    newMapLayer_.ACE_Shutdown();

    if (projectileLayer_) {
        projectileLayer_->Shutdown();
        delete projectileLayer_;
        projectileLayer_ = nullptr;
    }
    if (stunLayer_) {
        stunLayer_->Shutdown();
        delete stunLayer_;
        stunLayer_ = nullptr;
    }
    for (Skeleton* s : skeletonsList_) {
        if (s) {
            s->Shutdown();
            delete s;
            s = nullptr;
        }
    }
    
    if (playerLayer_) {
        playerLayer_->Shutdown();
        delete playerLayer_;
        playerLayer_ = nullptr;
    }
    if (hudLayer_) {
        hudLayer_->ACE_Shutdown();
        delete hudLayer_;
        hudLayer_ = nullptr;
    }

    r_ = nullptr;
    audioManager_ = nullptr;

}

void MainGameplayGameState::ACE_HandleEvent(Keys keyPress) {

    uint64_t now = SDL_GetTicks();

    if (keyPress == Keys::Space) {
        playerLayer_->HandleEvent(Keys::Space);
        audioManager_->loadQ(SoundFX::Swipe);
    }
    else if (keyPress == Keys::Q) {
        playerLayer_->HandleEvent(Keys::Q);

    }
    else if (keyPress == Keys::W && now >= playerLayer_->getHammerTimer()) {
        playerLayer_->HandleEvent(Keys::W);
        stunLayer_->HandleEvent(Keys::W);
    }
    else if (keyPress == Keys::E) {
        playerLayer_->HandleEvent(Keys::E);

    }

    else if (keyPress == Keys::R && now >= playerLayer_->getProjectileTimer()) {
        playerLayer_->HandleEvent(Keys::R);
        projectileLayer_->HandleEvent(Keys::R);
        audioManager_->loadQ(SoundFX::Ult);

    }
}

void MainGameplayGameState::ACE_UpdateFixed(double dt, const bool* kKeys) {

    ACE_basicAllEntitiesCollisionResolution(entitiesList_);

    for (Skeleton* s : skeletonsList_) {
        SkeletonBehavior(s);
    }

    // Projectile is charged, Animation starts
    if (projectileLayer_->ProjectileState == WeaponState::ReadyForLaunch) {
        projectileLayer_->Init(r_, playerLayer_->facing, playerLayer_->get_x_px(), playerLayer_->get_y_px(), playerProjectilePath);
    }
    if (stunLayer_->stunHammerState == WeaponState::ReadyForLaunch) {
        stunLayer_->Init(r_, playerLayer_->facing, playerLayer_->get_x_px(), playerLayer_->get_y_px(), playerStunHammerPath);
        audioManager_->loadQ(SoundFX::Hammer);
    }
    // IF projectile hits edge of map or a wall
    if (ACE_checkBodyMapedgeCollision(*projectileLayer_)
        || ACE_checkAttackWallCollision(*projectileLayer_, newMapLayer_)) {
        if (projectileLayer_->ProjectileState == WeaponState::Move) {
            projectileLayer_->ProjectileState = WeaponState::Inactive;
        }
    }

    playerLayer_->UpdateFixed(dt, kKeys, newMapLayer_);

    for (Skeleton* s : skeletonsList_) {
        s->UpdateFixed(dt, kKeys);
    }

    projectileLayer_->UpdateFixed(dt, kKeys);
    stunLayer_->UpdateFixed(dt, kKeys);
    hudLayer_->ACE_UpdateFixed(dt, kKeys);

    //ACE_Cam_Zoom(camCenter, 0.9f);
    ACE_Cam_FollowCenter(camCenter, *playerLayer_);

    tempTimer -= 1;
    if (tempTimer <= 0) {
        tempTimer = 30;
        costMap = ACE_createCostMap(newMapLayer_, *playerLayer_);
        vectorMap = ACE_createVectorMap(newMapLayer_, costMap);
    }

}

void MainGameplayGameState::ACE_UpdateFrame(uint64_t now_ms) {
    (void)now_ms;

    playerLayer_->UpdateFrame(SDL_GetTicks());

    for (Skeleton* s : skeletonsList_) {
        s->UpdateFrame(SDL_GetTicks());
    }

    projectileLayer_->UpdateFrame(SDL_GetTicks());
    stunLayer_->UpdateFrame(SDL_GetTicks());
    hudLayer_->UpdateFrame(SDL_GetTicks(), *playerLayer_);

}

void MainGameplayGameState::ACE_Render(SDL_Renderer* appR) {

    newMapLayer_.ACE_Render(appR, camCenter);

    for (Skeleton* s : skeletonsList_) {
        s->Render(appR, camCenter);
    }

    stunLayer_->Render(appR, camCenter);
    playerLayer_->Render(appR, camCenter);
    projectileLayer_->Render(appR, camCenter);
    hudLayer_->ACE_Render(appR, camCenter);



#if SHOW_VECTOR_MAP
    ACE_renderArrows(appR, cam, arrowTexures, newMapLayer_, tileSizeAsFloat_, vectorMap);
#endif

}


void MainGameplayGameState::SkeletonBehavior(Skeleton* curSkeleton_) {

    uint64_t now = SDL_GetTicks();

    if (curSkeleton_->get_alive_state() == AliveState::IsAlive) {

        // IF Player hits Skeleton
        if (ACE_checkAttackBodyCollision(*playerLayer_, *curSkeleton_) && curSkeleton_->invincibleTimer <= 0) {
            curSkeleton_->invincibleTimer = curSkeleton_->INVINCIBLE_DURATION;
            curSkeleton_->takesDamage(1);
            audioManager_->loadQ(SoundFX::Splat);
            if (curSkeleton_->iq_ == Intelligence::Smart) curSkeleton_->curMood = Mood::Pissed;
        }
        // IF Projectile hits Skeleton
        if (ACE_checkAttackBodyCollision(*projectileLayer_, *curSkeleton_)) {
            if (playerLayer_->get_x_px() > curSkeleton_->get_x_px()) curSkeleton_->facing = Facing::Right;
            else curSkeleton_->facing = Facing::Left;

            curSkeleton_->curMood = Mood::Pissed;
            if (curSkeleton_->invincibleTimer <= 0) {
                curSkeleton_->invincibleTimer = curSkeleton_->INVINCIBLE_DURATION;
                curSkeleton_->takesDamage(2);
            }
        }
        // IF Stun hits Skeleton
        if (ACE_checkAttackBodyCollision(*stunLayer_, *curSkeleton_)) {
            if (curSkeleton_->get_stunned_state() == StunnedState::NotStunned)
                audioManager_->loadQ(SoundFX::Splat);
            curSkeleton_->takesStun(SDL_GetTicks(), stunLayer_->stunMaxTime_ms);
            if (curSkeleton_->iq_ == Intelligence::Smart) curSkeleton_->curMood = Mood::Pissed;
        }

        if (curSkeleton_->get_stunned_state() == StunnedState::NotStunned) {

            // IF Player is within skeleton's Awareness zone
            if (ACE_checkBodyAwarenessCollision(*playerLayer_, *curSkeleton_)) {
                curSkeleton_->playerNearby = true;
                if (playerLayer_->get_x_px() > curSkeleton_->get_x_px()) curSkeleton_->facing = Facing::Right;
                else curSkeleton_->facing = Facing::Left;

                // Slightly adjusted get_speed() to ensure skeleton doesn't follow TOO fast.
                float speed = curSkeleton_->get_speed_pixels();
                if (curSkeleton_->curMood == Mood::Pissed) speed *= 0.3;
                ACE_followTargetSimple(*curSkeleton_, *playerLayer_, newMapLayer_, speed);

            }

            // IF Skeleton hits Player
            if (ACE_checkCollision(*curSkeleton_, HitboxType::AttackRange, *playerLayer_, HitboxType::BodyHitbox) && now >= playerLayer_->getInvincibleTimer()) {
                //playerLayer_->setInvincibleTimer(playerLayer_->getInvincibleDuration());
                //playerLayer_->takesDamage(1);
            }
            // IF you ulted the skeleton, it will follow you forever.
            if (curSkeleton_->curMood == Mood::Pissed) {
                if (playerLayer_->get_x_px() > curSkeleton_->get_x_px()) curSkeleton_->facing = Facing::Right;
                else curSkeleton_->facing = Facing::Left;
                //ACE_smartFollow(curSkeleton_, floorLayer_, vectorMap, costMap);
                ACE_smartFollow_vecField(curSkeleton_, newMapLayer_, vectorMap, costMap);

            }
        }
    }

    if (!ACE_checkBodyAwarenessCollision(*playerLayer_, *curSkeleton_)) {
        curSkeleton_->playerNearby = false;
    }
}


void MainGameplayGameState::buildArrows() {
    for (std::string path : arrowPaths) {
        SDL_Surface* surf = IMG_Load(path.c_str());
        if (!surf) {
            SDL_Log("BOOOOO HOOO (%s): %s", "tiles as surface failed to load", SDL_GetError());
            SDL_DestroySurface(surf);
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(r_, surf);
        if (!tex) {
            SDL_Log("BOOOOO HOOO (%s): %s", "Tiles as texture failed to load", SDL_GetError());
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        }
        arrowTexures.push_back(tex);
        SDL_DestroySurface(surf);
    }
}


#endif
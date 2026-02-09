#pragma once
#include "knightsLegacy/gameStates/public/MainGameplayGameState.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <cstdint>
#include <vector>
#include "AChoiEngine/physics/public/OverlappingHitboxDetectionSys.h"
#include "AChoiEngine/physics/public/CollisionResponseSys.h"
#include "AChoiEngine/ai/public/aiSys.h"
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/camera/public/Camera.h"
#include "knightsLegacy/core/public/GameManager.h"
#include "knightsLegacy/audioManager/public/audioManager.h"
#include "knightsLegacy/entities/player/public/Player.h"
#include "knightsLegacy/entities//enemies/public/Skeleton.h"
#include "knightsLegacy/weapons/public/Projectile.h"
#include "knightsLegacy/weapons/public/Stun.h"
#include "knightsLegacy/hud/public/Hud.h"
#include "knightsLegacy/world/public/TileCatalog.h"
#include "assets.h"


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

    //baseLayer_.ACE_LoadTiles(appR, environmentTilesheets, doorChestAnimPath.c_str(), dungeon1Base.c_str(), tileSizeAsFloat_);
    //floorLayer_.ACE_LoadTiles(appR, environmentTilesheets, doorChestAnimPath.c_str(), map1.c_str(), tileSizeAsFloat_);
             
    newMapLayer_.ACE_Init(appR, jsonLevel1v2.c_str());
    world_level1_w_ = newMapLayer_.getWorldWidth_pixels();
    world_level1_h_ = newMapLayer_.getWorldHeight_pixels();
    
    // GameObject Parameter Guide:
    // (posX_pixel, posY_pixel, width_pixels, height_pixels, app_width_pixels, app_height_pixels,
    //  world_width_pixels, world_height_pixels, obj_width_gUnits, obj_height_gUnits, gUnit_side_length_InPixels,
    //  speed_pixels, direction, heatlh, damage)
    // INSTANTIATE PLAYER AND INIT 
    playerLayer_ = new Player(appW_pixels_ * 0.5f, appH_pixels_ * 0.5f, 3.0f, 3.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
                                7, 5, tileSizeAsInt_, 0, 0, 5, 1);
    playerLayer_->Init(appR, appW_pixels_ *0.5f, appH_pixels_ *0.5f);

    // INSTANTIATE SKELETON AND INTIS
    // One skely in top left room - Dumb AI
    skeletonOne_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_, 
                                2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonOne_->Init(appR, appW_pixels_ *0.3f, appH_pixels_ *0.3f, Facing::Right, Intelligence::Dumb, 4);
    
    // One skely in top right room - Smart AI
    skeletonTwo_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_, 
                                2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonTwo_->Init(appR, appW_pixels_ * 1.3f, appH_pixels_ * 0.4f, Facing::Left, Intelligence::Smart, 6);
    
    // Two skelies in bot left room - Dumb AI
    skeletonThree_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_, 
                                    2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonThree_->Init(appR, appW_pixels_ * 0.6f, appH_pixels_ * 1.85f, Facing::Left, Intelligence::Dumb, 8);

    skeletonFour_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonFour_->Init(appR, appW_pixels_ * 0.3f, appH_pixels_ * 1.5f, Facing::Right, Intelligence::Dumb, 8);
    
    // Three skelies in bot right room - Smart AI
    skeletonFive_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonFive_->Init(appR, appW_pixels_ * 1.46f, appH_pixels_ * 1.69f, Facing::Left, Intelligence::Smart, 7);

    skeletonSix_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonSix_->Init(appR, appW_pixels_ * 1.33f, appH_pixels_ * 2.45f, Facing::Left, Intelligence::Smart, 7);

    skeletonSeven_ = new Skeleton(appW_pixels_ * 5.0f, appH_pixels_ * 5.0f, 2.0f, 2.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_,
        2, 2, tileSizeAsInt_, 0, 0, 5, 1);
    skeletonSeven_->Init(appR, appW_pixels_ * 1.13f, appH_pixels_ * 2.23f, Facing::Left, Intelligence::Smart, 7);

    // INSTANTIATE ULT
    projectileLayer_ = new SwordWaveProjectile(0, 0, 3.0f, 3.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_, 
                                                8, 8, tileSizeAsInt_, 0, 0, 0, 3);
    // INSTANTIATE STUN
    stunLayer_ = new Stun(0, 0, 3.0f, 3.0f, appW_pixels_, appH_pixels_, world_level1_w_, world_level1_h_, 
                            8, 8, tileSizeAsInt_, 0, 0, 0, 1);

    // Set Camera parameters
    cam.ACE_SetViewport(appW_pixels_, appH_pixels_);
    cam.ACE_SetWorldBounds(world_level1_w_, world_level1_h_);

    // Instantiate hud and init with parameters
    hudLayer_ = new Hud();
    hudLayer_->Init(appR, appW_pixels_, appH_pixels_, cam, *playerLayer_);

    entitiesList_.reserve(16);
    entitiesList_.push_back(skeletonOne_);
    entitiesList_.push_back(skeletonTwo_);
    entitiesList_.push_back(skeletonThree_);
    entitiesList_.push_back(skeletonFour_);
    entitiesList_.push_back(skeletonFive_);
    entitiesList_.push_back(skeletonSix_);
    entitiesList_.push_back(skeletonSeven_);
    entitiesList_.push_back(playerLayer_);

    audioManager_->ACE_LoadPlayMusic(BackgroundMusic::MainGameplay);
    

    #if SHOW_VECTOR_MAP
        buildArrows();
    #endif

    return true;
}


void MainGameplayGameState::ACE_Shutdown() {

    //baseLayer_.ACE_Shutdown();
    //floorLayer_.ACE_Shutdown();
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

    if (skeletonOne_) {
        skeletonOne_->Shutdown();
        delete skeletonOne_;
        skeletonOne_ = nullptr;
    }
    if (skeletonTwo_) {
        skeletonTwo_->Shutdown();
        delete skeletonTwo_;
        skeletonTwo_ = nullptr;
    }
    if (skeletonThree_) {
        skeletonThree_->Shutdown();
        delete skeletonThree_;
        skeletonThree_ = nullptr;
    }
    if (skeletonFour_) {
        skeletonFour_->Shutdown();
        delete skeletonFour_;
        skeletonFour_ = nullptr;
    }
    if (skeletonFive_) {
        skeletonFive_->Shutdown();
        delete skeletonFive_;
        skeletonFive_ = nullptr;
    }
    if (skeletonSix_) {
        skeletonSix_->Shutdown();
        delete skeletonSix_;
        skeletonSix_ = nullptr;
    }
    if (skeletonSeven_) {
        skeletonSeven_->Shutdown();
        delete skeletonSeven_;
        skeletonSeven_ = nullptr;
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

    if (keyPress == Keys::Space) {
        playerLayer_->HandleEvent(Keys::Space);
        audioManager_->loadQ(SoundFX::Swipe);
    }
    else if (keyPress == Keys::Q) {
        playerLayer_->HandleEvent(Keys::Q);

    }
    else if (keyPress == Keys::W && playerLayer_->getHammerTimer() <= 0) {
        playerLayer_->HandleEvent(Keys::W);
        stunLayer_->HandleEvent(Keys::W);       
    }
    else if (keyPress == Keys::E) {
        playerLayer_->HandleEvent(Keys::E);

    }

    else if (keyPress == Keys::R && playerLayer_->getProjectileTimer() <= 0) {
        playerLayer_->HandleEvent(Keys::R);
        projectileLayer_->HandleEvent(Keys::R);
        audioManager_->loadQ(SoundFX::Ult);

    }
}

void MainGameplayGameState::ACE_UpdateFixed(double dt, const bool* kKeys) {

    ACE_basicAllEntitiesCollisionResolution(entitiesList_);

    SkeletonBehavior(skeletonOne_);
    SkeletonBehavior(skeletonTwo_);
    SkeletonBehavior(skeletonThree_);
    SkeletonBehavior(skeletonFour_);
    SkeletonBehavior(skeletonFive_);   
    SkeletonBehavior(skeletonSix_);
    SkeletonBehavior(skeletonSeven_);  

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

    //playerLayer_->UpdateFixed(dt, kKeys, floorLayer_);
    playerLayer_->UpdateFixed(dt, kKeys, newMapLayer_);

    skeletonOne_->UpdateFixed(dt, kKeys);
    skeletonTwo_->UpdateFixed(dt, kKeys);
    skeletonThree_->UpdateFixed(dt, kKeys);
    skeletonFour_->UpdateFixed(dt, kKeys);
    skeletonFive_->UpdateFixed(dt, kKeys);
    skeletonSix_->UpdateFixed(dt, kKeys);
    skeletonSeven_->UpdateFixed(dt, kKeys);

    projectileLayer_->UpdateFixed(dt, kKeys);
    stunLayer_->UpdateFixed(dt, kKeys);
    hudLayer_->ACE_UpdateFixed(dt, kKeys);

    cam.ACE_Follow(playerLayer_->get_x_px(), playerLayer_->get_y_px());

    tempTimer -= 1;
    if (tempTimer <= 0) {
        tempTimer = 30;
        //costMap = CreateCostMap(floorLayer_, *playerLayer_, GetTileCatalog());
        //vectorMap = CreateVectorMap(floorLayer_, costMap);
        costMap = ACE_CreateCostMap(newMapLayer_, *playerLayer_);
        vectorMap = ACE_CreateVectorMap(newMapLayer_, costMap);
    }
    

    if (newMapLayer_.ACE_intGridType(playerLayer_->get_x_px(), playerLayer_->get_y_px()) == 4) {
        skeletonSeven_->curMood = Mood::Pissed;
    }
    if (newMapLayer_.ACE_intGridType(playerLayer_->get_x_px(), playerLayer_->get_y_px()) == 5) {
        skeletonFive_->curMood = Mood::Pissed;
        skeletonSix_->curMood = Mood::Pissed;
    }

}

void MainGameplayGameState::ACE_UpdateFrame(uint64_t now_ms) {
    (void)now_ms;

    playerLayer_->UpdateFrame(SDL_GetTicks());

    skeletonOne_->UpdateFrame(SDL_GetTicks());
    skeletonTwo_->UpdateFrame(SDL_GetTicks());
    skeletonThree_->UpdateFrame(SDL_GetTicks());
    skeletonFour_->UpdateFrame(SDL_GetTicks());
    skeletonFive_->UpdateFrame(SDL_GetTicks());
    skeletonSix_->UpdateFrame(SDL_GetTicks());
    skeletonSeven_->UpdateFrame(SDL_GetTicks());

    projectileLayer_->UpdateFrame(SDL_GetTicks());
    stunLayer_->UpdateFrame(SDL_GetTicks());
    hudLayer_->UpdateFrame(SDL_GetTicks(), *playerLayer_);

}

void MainGameplayGameState::ACE_Render(SDL_Renderer* appR) {
    //baseLayer_.ACE_Render(appR, cam);
    //floorLayer_.ACE_Render(appR, cam);

    newMapLayer_.ACE_Render(appR, cam);
    
    skeletonOne_->Render(appR, cam);
    skeletonTwo_->Render(appR, cam);
    skeletonThree_->Render(appR, cam);
    skeletonFour_->Render(appR, cam);
    skeletonFive_->Render(appR, cam);
    skeletonSix_->Render(appR, cam);
    skeletonSeven_->Render(appR, cam);

    stunLayer_->Render(appR, cam);
    playerLayer_->Render(appR, cam);
    projectileLayer_->Render(appR, cam);
    hudLayer_->ACE_Render(appR, cam);

    #if SHOW_VECTOR_MAP
        ACE_RenderArrows(appR, cam, arrowTexures, newMapLayer_, tileSizeAsFloat_, vectorMap);
    #endif

}


void MainGameplayGameState::SkeletonBehavior(Skeleton* curSkeleton_) {
    
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
            if (ACE_checkCollision(*curSkeleton_, HitboxType::AttackRange, *playerLayer_, HitboxType::BodyHitbox) && playerLayer_->getInvincibleTimer() <= 0) {
                playerLayer_->setInvincibleTimer(playerLayer_->getInvincibleDuration());
                playerLayer_->takesDamage(1);
            }
            // IF you ulted the skeleton, it will follow you forever.
            if (curSkeleton_->curMood == Mood::Pissed) {
                if (playerLayer_->get_x_px() > curSkeleton_->get_x_px()) curSkeleton_->facing = Facing::Right;
                else curSkeleton_->facing = Facing::Left;
                //ACE_smartFollow(curSkeleton_, floorLayer_, vectorMap, costMap);
                ACE_smartFollow(curSkeleton_, newMapLayer_, vectorMap, costMap);
                
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


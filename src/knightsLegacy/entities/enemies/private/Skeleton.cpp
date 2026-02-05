#include "knightsLegacy/entities/enemies/public/skeleton.h"
#include <string>
#include "AChoiEngine/ai/public/aiComponent.h"
#include "assets.h"

bool Skeleton::Init(SDL_Renderer* appR, float spawnX_px, float spawnY_px, Facing spawnDirection, Intelligence iq, int health) {

    set_x_px(spawnX_px);
    set_y_px(spawnY_px);
    set_x_gu(static_cast<int>(std::floor(spawnX_px/16)));
    set_y_gu(static_cast<int>(std::floor(spawnY_px/16)));
    facing = spawnDirection;
    set_speed_pixels(100.0f);
    set_health(health);
    iq_ = iq;

    set_hitbox_pixels(HitboxType::BodyHitbox,
        get_x_px(),
        get_y_px(),
        18.0f,
        24.0f);

    set_hitbox_pixels(HitboxType::AwarenessRange,
        get_x_px(),
        get_y_px(),
        105.0f,
        92.0f);

    set_hitbox_pixels(HitboxType::AttackRange,
        get_x_px(),
        get_y_px(),
        46.0f,
        42.0f);

    set_hitbox_pixels(HitboxType::UtilityRange1,
        get_x_px(),
        get_y_px(),
        30.0f,
        30.0f);

    skeletonAttack_ = BuildAnimSprite(appR, skeletonAttack1Path.c_str(),    // renderer, imagePath, 
        16, 2, 2, 1.5,  // tileSize, tilesPerFrameW, tilesPerFrameH, scale, 
        0, 0, 9, 9,     // startCol, startRow, frameCount, colsInSheet, 
        1, 90, false);     // colSkipCount, frameMs, loop

    skeletonIdle_ = BuildAnimSprite(appR, skeletonIdlePath.c_str(),
        16, 2, 2, 1.5,
        0, 0, 6, 6,
        1, 90, true);

    skeletonDead_ = BuildAnimSprite(appR, skeletonDeathPath.c_str(),
        16, 2, 2, 1.5,
        0, 0, 17, 17,
        1, 90, false);

    skeletonTakesDamage_ = BuildAnimSprite(appR, skeletonTakesDmgPath.c_str(),
        16, 2, 2, 1.5,
        0, 0, 5, 5,
        1, 90, false);

    skeletonMove_ = BuildAnimSprite(appR, skeletonMovePath.c_str(),
        16, 2, 2, 1.5,
        0, 0, 10, 10,
        1, 90, false);

    skeletonAttack2_ = BuildAnimSprite(appR, skeletonAttack2Path.c_str(),
        16, 2, 2, 1.5,
        0, 0, 15, 15,
        1, 90, false);

    stunnedAnimation_ = LoadAnimGif(appR, ouch1Path.c_str(), 110);

    PlayAnimSprite(skeletonIdle_, SDL_GetTicks(), true);


    return true;
}

void Skeleton::Shutdown() { 
    DestroyAnimSprite(skeletonAttack_);
    DestroyAnimSprite(skeletonIdle_);
    DestroyAnimSprite(skeletonDead_);
    DestroyAnimSprite(skeletonMove_);
    DestroyAnimSprite(skeletonAttack2_);
    DestroyAnimSprite(skeletonTakesDamage_);
    DestroyAnimGif(stunnedAnimation_);
}

void Skeleton::HandleEvent(const SDL_Event& event) {

}

void Skeleton::UpdateFixed(double dt, const bool* keys) {

    uint64_t now = SDL_GetTicks();

    float vx = 0.0f;
    float vy = 0.0f;
    if (get_health() > 0) {
        if (facing == Facing::Right) {
            set_hitbox_pixels( HitboxType::BodyHitbox,  
                        get_x_px() - 12.0f, 
                        get_y_px() - 4.0f, 
                        get_hitbox_w_pixels(HitboxType::BodyHitbox), 
                        get_hitbox_h_pixels(HitboxType::BodyHitbox));

            set_hitbox_pixels( HitboxType::AwarenessRange,
                        get_x_px() - 30.0f, 
                        get_y_px() - 35.0f, 
                        get_hitbox_w_pixels(HitboxType::AwarenessRange), 
                        get_hitbox_h_pixels(HitboxType::AwarenessRange));

            set_hitbox_pixels( HitboxType::AttackRange,
                        get_x_px() - 20.0f, 
                        get_y_px() - 12.0f, 
                        get_hitbox_w_pixels(HitboxType::AttackRange), 
                        get_hitbox_h_pixels(HitboxType::AttackRange));
            set_hitbox_pixels(HitboxType::UtilityRange1,
                        get_x_px() - 16.0f,
                        get_y_px() - 6.0f,
                        get_hitbox_w_pixels(HitboxType::UtilityRange1),
                        get_hitbox_h_pixels(HitboxType::UtilityRange1));
        }
        else {
            set_hitbox_pixels( HitboxType::BodyHitbox,
                        get_x_px() - 12.0f, 
                        get_y_px() - 4.0f, 
                        get_hitbox_w_pixels(HitboxType::BodyHitbox), 
                        get_hitbox_h_pixels(HitboxType::BodyHitbox));

            set_hitbox_pixels( HitboxType::AwarenessRange,
                        get_x_px() - 72.0f, 
                        get_y_px() - 35.0f, 
                        get_hitbox_w_pixels(HitboxType::AwarenessRange), 
                        get_hitbox_h_pixels(HitboxType::AwarenessRange));

            set_hitbox_pixels( HitboxType::AttackRange,
                        get_x_px() - 20.0f, 
                        get_y_px() - 12.0f, 
                        get_hitbox_w_pixels(HitboxType::AttackRange), 
                        get_hitbox_h_pixels(HitboxType::AttackRange));
            set_hitbox_pixels(HitboxType::UtilityRange1,
                        get_x_px() - 12.0f,
                        get_y_px() - 6.0f,
                        get_hitbox_w_pixels(HitboxType::UtilityRange1),
                        get_hitbox_h_pixels(HitboxType::UtilityRange1));
        }
    }
    else {
        set_alive_state(AliveState::NotAlive);

    }
}

void Skeleton::UpdateFrame(uint64_t now_ms) {

    // INITIATE Animation if needed
    if (get_alive_state() == AliveState::IsAlive) {

        if (invincibleTimer > 0) {
            invincibleTimer -= 1;
            StopAnimSprite(skeletonIdle_);
            StopAnimSprite(skeletonAttack_);
            StopAnimSprite(skeletonMove_);
            if (!skeletonTakesDamage_.playing) {
                PlayAnimSprite(skeletonTakesDamage_, now_ms, false);
            }
        }
        else if (get_stunned_state() == StunnedState::NotStunned && playerNearby && !skeletonAttack_.playing && !skeletonTakesDamage_.playing) {

            StopAnimSprite(skeletonIdle_);
            StopAnimSprite(skeletonMove_);
            PlayAnimSprite(skeletonAttack_, now_ms, false); // one-shot
        }
        else if (curMood == Mood::Pissed) {
            if (!skeletonMove_.playing) {
                PlayAnimSprite(skeletonMove_, now_ms, true);
            }
        }

        if (get_stunned_state() == StunnedState::IsStunned) {
            if (skeletonAttack_.playing) StopAnimSprite(skeletonAttack_);
            if (skeletonIdle_.playing) StopAnimSprite(skeletonIdle_);
            if (skeletonMove_.playing) StopAnimSprite(skeletonMove_);

            if (!skeletonTakesDamage_.playing) {
                PlayAnimSprite(skeletonTakesDamage_, now_ms, true);
            }
            if (!stunnedAnimation_.playing) {
                PlayAnimGif(stunnedAnimation_, now_ms, true);
            }
            if (now_ms - stunStartTimer_ms >= stunMaxTimer_ms) {
                stunMaxTimer_ms = 0;
                StopAnimGif(stunnedAnimation_);
                PlayAnimSprite(skeletonIdle_, now_ms, true);
                set_stunned_state(StunnedState::NotStunned);
            }
        }
    }
    else { 
        if (!skeletonDead_.playing && curEnemyState != EnemyState::Dead) {
            StopAnimSprite(skeletonIdle_);
            StopAnimSprite(skeletonAttack_);
            StopAnimSprite(skeletonTakesDamage_);
            StopAnimGif(stunnedAnimation_);
            StopAnimSprite(skeletonMove_);
            PlayAnimSprite(skeletonDead_, now_ms, false);
            curEnemyState = EnemyState::Dead;
        }
    }


    // ADVANCE animation, if needed
    if (skeletonDead_.playing) {
        UpdateAnimSprite(skeletonDead_, now_ms);
        if (FinishedNonLoopingSprite(skeletonDead_, now_ms)) {
            StopAnimSprite(skeletonDead_);
        }
    }
    else if (skeletonAttack_.playing) {
        UpdateAnimSprite(skeletonAttack_, now_ms);
        if (FinishedNonLoopingSprite(skeletonAttack_, now_ms)) {
            StopAnimSprite(skeletonAttack_);
            if (!skeletonIdle_.playing) {
                PlayAnimSprite(skeletonIdle_, now_ms, true);
            }
        }
    }
    else if (skeletonTakesDamage_.playing) {
        UpdateAnimSprite(skeletonTakesDamage_, now_ms);
        if (FinishedNonLoopingSprite(skeletonTakesDamage_, now_ms)) {
            if (get_stunned_state() == StunnedState::IsStunned) PlayAnimSprite(skeletonTakesDamage_, now_ms, false);
            else {
                StopAnimSprite(skeletonTakesDamage_);
                if (!skeletonIdle_.playing) {
                    PlayAnimSprite(skeletonIdle_, now_ms, true);
                }
            }
        }
    }
    else if (skeletonMove_.playing) {
        UpdateAnimSprite(skeletonMove_, now_ms);
        if (FinishedNonLoopingSprite(skeletonMove_, now_ms)) {
            PlayAnimSprite(skeletonMove_, now_ms, true);
        }
    }
    else {
        if (!skeletonIdle_.playing) {
            PlayAnimSprite(skeletonIdle_, now_ms, true);
        }
        UpdateAnimSprite(skeletonIdle_, now_ms);
    }

}


void Skeleton::Render(SDL_Renderer* appR, Camera2D& cam) const {

    const Uint32 now = SDL_GetTicks();

    // Choose which animation to render + any special-case flags
    const AnimSprite* anim = nullptr;
    bool flashRed = false;
    bool deadFreezeLastFrame = false;

    if (get_alive_state() == AliveState::NotAlive) {
        anim = &skeletonDead_;
        deadFreezeLastFrame = true;
    }
    else if (invincibleTimer > 0) {
        anim = &skeletonTakesDamage_;
        flashRed = true;
    }
    else if (skeletonAttack_.playing) {
        anim = &skeletonAttack_;
    }
    else if (skeletonMove_.playing) {            // or: else if (isMoving)
        anim = &skeletonMove_;
    }
    else {
        anim = &skeletonIdle_;
    }

    const AnimSprite& a = *anim;

    // Common destination rect
    const float rendW = a.frameW * a.scale;
    const float rendH = a.frameH * a.scale;

    const SDL_FRect dst{
        get_x_px() - rendW * 0.5f,
        get_y_px() - rendH * 0.5f,
        rendW,
        rendH
    };

    // Common flip
    const SDL_FlipMode flip = (facing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // Pick source frame (dead animation may freeze on last frame after completion)
    const SDL_FRect* src = nullptr;
    if (deadFreezeLastFrame && FinishedNonLoopingSprite(a, now)) {
        const int last = (int)a.frames.size() - 1;
        src = &a.frames[last];
    }
    else {
        src = &CurrentFrameSprite(a);
    }

    // Damage flash
    if (flashRed) SDL_SetTextureColorModFloat(a.tex, 2.5f, 1.0f, 1.0f);

    SDL_FRect screenDst = cam.WorldToScreen(dst);
    SDL_RenderTextureRotated(appR, a.tex, src, &screenDst, 0.0, nullptr, flip);

    if (flashRed) SDL_SetTextureColorModFloat(a.tex, 1.0f, 1.0f, 1.0f);

    if (get_stunned_state() == StunnedState::IsStunned) {
        SDL_Texture* tex = CurrentFrameGif(stunnedAnimation_, now);
        SDL_FlipMode flip = (facing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_FRect screenOuchDst = cam.WorldToScreen(dst);
        SDL_RenderTextureRotated(appR, tex, nullptr, &screenOuchDst, 0.0, nullptr, flip);
    }


    // DEBUG COLLISIONS
    //SDL_FRect hitboxHighlightDst{ get_hitbox_x_px(HitboxType::AttackRange), get_hitbox_y_px(HitboxType::AttackRange),
    //                              get_hitbox_w_pixels(HitboxType::AttackRange), get_hitbox_h_pixels(HitboxType::AttackRange) };
    //SDL_FRect screenHitboxDst = cam.WorldToScreen(hitboxHighlightDst);
    //SDL_SetRenderDrawColor(appR, 0, 255, 0, 40);
    //SDL_RenderFillRect(appR, &screenHitboxDst);

    /*SDL_FRect hitboxHighlight2Dst{ get_hitbox_x_px(HitboxType::BodyHitbox), get_hitbox_y_px(HitboxType::BodyHitbox),
                          get_hitbox_w_pixels(HitboxType::BodyHitbox), get_hitbox_h_pixels(HitboxType::BodyHitbox) };
    SDL_FRect screenHitbox2Dst = cam.WorldToScreen(hitboxHighlight2Dst);
    SDL_SetRenderDrawColor(appR, 0, 0, 255, 40);
    SDL_RenderFillRect(appR, &screenHitbox2Dst);*/

}


AnimSprite Skeleton::getIdle() {
    return skeletonIdle_;
}
AnimSprite Skeleton::getAttack() {
    return skeletonAttack_;
}

void Skeleton::collisionResponse() {

}

void Skeleton::takesDamage(int dmgAmount) {
    set_health(get_health() - dmgAmount);
    if (get_health() <= 0) {
        set_alive_state(AliveState::NotAlive);
    }
}

void Skeleton::takesStun(uint64_t now, uint64_t stunLength) {
    set_stunned_state(StunnedState::IsStunned);
    stunStartTimer_ms = now;
    stunMaxTimer_ms += stunLength;
}





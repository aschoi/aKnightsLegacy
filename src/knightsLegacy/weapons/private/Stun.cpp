#include "knightsLegacy/weapons/public/Stun.h"
#include "knightsLegacy/core/public/GameManager.h"
#include "assets.h"


bool Stun::Init(SDL_Renderer* r,
                Facing facing,
                float playerX_px,
                float playerY_px,
                std::string animPath) {

    
    set_speed_pixels(0.01f);
    scale_ = 0.75f;
    set_x_px(playerX_px);
    set_y_px(playerY_px);
    stunFacing = facing;
    stunHammer_ = ACE_LoadAnimGif(r, animPath.c_str(), 70);

    ACE_PlayAnimGif(stunHammer_, SDL_GetTicks(), false);
    stunHammerState = WeaponState::Damage;


    set_hitbox_pixels(HitboxType::BodyHitbox, 1.0f, 1.0f, 1.0f, 1.0f);

    if (facing == Facing::Right) set_hitbox_pixels(HitboxType::AttackRange, get_x_px() + 40.0f, get_y_px(), 40.0f, 40.0f);
    else set_hitbox_pixels(HitboxType::AttackRange, get_x_px() - 85.0f, get_y_px(), 40.0f, 40.0f);

    return true;
}

void Stun::Shutdown() {
    ACE_DestroyAnimGif(stunHammer_);
}

void Stun::HandleEvent(Keys key) {
    if (key == Keys::W) {
        if (stunHammerState == WeaponState::Idle) {
            stunHammerState = WeaponState::ReadyForLaunch;
        }
    }
}

void Stun::UpdateFixed(double dt, const bool* keys) {
    if (stunHammerState == WeaponState::Damage) {
        float vx = 1.0f;
        if (stunFacing == Facing::Left) {
            vx = -1.0f;
        }
        float vy = 0.0f;

        if (vx != 0.0f && vy != 0.0f) {
            vx *= 0.70710678f;
            vy *= 0.70710678f;
        }

        set_x_px(get_x_px() + vx * get_speed_pixels() * (float)dt);
        set_y_px(get_y_px() + vy * get_speed_pixels() * (float)dt);
        set_center_px();

    }
    else {
        set_hitbox_pixels(HitboxType::AttackRange, 0, 0, 0, 0);
    }
    
}
void Stun::UpdateFrame(uint64_t now_ms) {
    if (ACE_FinishedNonLoopingGif(stunHammer_, now_ms)) {
        ACE_StopAnimGif(stunHammer_);
        stunHammerState = WeaponState::Idle;
    }
}


void Stun::Render(SDL_Renderer* appR, ACE_Camera2D& cam) const {
    
    const float halfW = stunHammer_.w * 0.3f;
    const float halfH = stunHammer_.h * 0.3f;
    float dstX = get_x_px() - halfW;
    float dstY = get_y_px() - halfH;

    if (stunFacing == Facing::Left) dstX -= 75.0f;
    else dstX += 50.0f;

    SDL_FRect dst{
        dstX,
        dstY,
        stunHammer_.w * scale_,
        stunHammer_.h * scale_
    };

    uint64_t now_ms = SDL_GetTicks();
    SDL_Texture* tex = nullptr;

    if (stunHammer_.playing) {
        tex = ACE_CurrentFrameGif(stunHammer_, now_ms);
    }

    if (!tex) return;

    SDL_FlipMode flip = (stunFacing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    
    SDL_FRect screenDst = cam.ACE_WorldToScreen(dst);
    SDL_RenderTextureRotated(appR, tex, nullptr, &screenDst, 0.0, nullptr, flip);


    // DEBUG COLLISIONS
    //SDL_FRect hitboxHighlightDst{ get_hitbox_x_px(HitboxType::AttackRange), get_hitbox_y_px(HitboxType::AttackRange),
    //                get_hitbox_w_pixels(HitboxType::AttackRange), get_hitbox_h_pixels(HitboxType::AttackRange) };
    //SDL_FRect screenHitboxDst = cam.WorldToScreen(hitboxHighlightDst);
    //SDL_SetRenderDrawColor(appR, 0, 0, 255, 40);
    //SDL_RenderFillRect(appR, &screenHitboxDst);

}


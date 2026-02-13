#include "knightsLegacy/weapons/public/projectile.h"
#include "AChoiEngine/camera/public/CameraObject.h"
#include "AChoiEngine/camera/public/CameraSys.h"

bool SwordWaveProjectile::Init( SDL_Renderer* r,
                                Facing facing,
                                float playerX_px,
                                float playerY_px,
                                std::string animPath) {

    set_speed_pixels(130.0f);
    scale_ = 0.3f;
    set_x_px(playerX_px);
    set_y_px(playerY_px);
    projFacing = facing;
    swordWaveProj_ = ACE_LoadAnimGif(r, animPath.c_str(), 70);

    ACE_PlayAnimGif(swordWaveProj_, SDL_GetTicks(), true);
    ProjectileState = WeaponState::Move;
    existenceStartTime_ms = SDL_GetTicks();

    set_hitbox_pixels(HitboxType::BodyHitbox, 1.0f, 1.0f, 1.0f, 1.0f);
    set_hitbox_pixels(HitboxType::AttackRange, get_x_px(), get_y_px()-16.0f, 50.0f, 70.0f);

    return true;
}

void SwordWaveProjectile::Shutdown() {
    ACE_DestroyAnimGif(swordWaveProj_);
}

void SwordWaveProjectile::HandleEvent(Keys key) {
    if (key == Keys::R) {
        if (ProjectileState == WeaponState::Inactive) {
            ProjectileState = WeaponState::Charging;
            chargingStartTime_ms = SDL_GetTicks();
        }
    }
}

void SwordWaveProjectile::UpdateFixed(double dt, const bool* keys) {

    if (ProjectileState == WeaponState::Move) {
        float vx = 1.0f;
        if (projFacing == Facing::Left) {
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


        if (projFacing == Facing::Right) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px(), get_y_px()-16.0f, 50.0f, 70.0f);
        }
        else if (projFacing == Facing::Left) {
            set_hitbox_pixels(HitboxType::AttackRange, get_x_px()-35.0f, get_y_px()-15.0f, 50.0f, 70.0f);
        }

        // Clamping logic that is currently flawed
       /* const float halfW = swordWaveProj_.w * 0.5f;
        const float halfH = swordWaveProj_.h * 0.5f;
        set_x((get_x() < halfW) ? halfW : (get_x() > get_app_w() - halfW ? get_app_w() - halfW : get_x()));
        set_y((get_y() < halfH) ? halfH : (get_y() > get_app_h() - halfH ? get_app_h() - halfH : get_y()));*/
    }
    else if (ProjectileState == WeaponState::Inactive) {
        set_hitbox_pixels(HitboxType::AttackRange, 0,0,0,0);
    }
}

void SwordWaveProjectile::UpdateFrame(uint64_t now_ms) {
    // Waiting for delayed animation to start
    if (ProjectileState == WeaponState::Charging) {
        if (now_ms - chargingStartTime_ms >= chargingDelayTime_ms) {
            ProjectileState = WeaponState::ReadyForLaunch;
        }
    }

    if (ProjectileState == WeaponState::Move) {
        if (get_x_px() + swordWaveProj_.w > get_world_w_pixels() + 30.0f || get_x_px() - 70.0f < 0) {
            ACE_StopAnimGif(swordWaveProj_);
            ProjectileState = WeaponState::Inactive;
        }
        if (now_ms - existenceStartTime_ms >= existenceMaxTime_ms) {
            ACE_StopAnimGif(swordWaveProj_);
            ProjectileState = WeaponState::Inactive;
        }
    }

    if (swordWaveProj_.playing && ProjectileState == WeaponState::Inactive) {
        ACE_StopAnimGif(swordWaveProj_);
    }
}


void SwordWaveProjectile::Render(SDL_Renderer* appR, ACE_Camera2D& cam) const {
    if (ProjectileState == WeaponState::Move) {
        const float halfW = swordWaveProj_.w * scale_;
        const float halfH = swordWaveProj_.h * scale_;

        SDL_FRect dst{
            (get_x_px() - halfW - 20.0f),
            (get_y_px() - halfH),
            swordWaveProj_.w,
            swordWaveProj_.h
        };

        uint64_t now_ms = SDL_GetTicks();
        SDL_Texture* tex = nullptr;

        if (swordWaveProj_.playing) {
            tex = ACE_CurrentFrameGif(swordWaveProj_, now_ms);
        }

        if (!tex) return;

        SDL_FlipMode flip = (projFacing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDL_FRect screenDst = cam.ACE_WorldToScreen(dst);
        SDL_RenderTextureRotated(appR, tex, nullptr, &screenDst, 0.0, nullptr, flip);


        // DEBUG COLLISIONS
        //SDL_FRect hitboxHighlightDst{ get_hitbox_x_px(HitboxType::AttackRange), get_hitbox_y_px(HitboxType::AttackRange),
        //                get_hitbox_w_pixels(HitboxType::AttackRange), get_hitbox_h_pixels(HitboxType::AttackRange) };
        //SDL_FRect screenHitboxDst = cam.WorldToScreen(hitboxHighlightDst);
        //SDL_SetRenderDrawColor(appR, 0, 0, 255, 40);
        //SDL_RenderFillRect(appR, &screenHitboxDst);

    }

}

void SwordWaveProjectile::Render(SDL_Renderer* appR, ACE_Camera2D_Center& cam) const {
    if (ProjectileState == WeaponState::Move) {
        const float halfW = swordWaveProj_.w * scale_;
        const float halfH = swordWaveProj_.h * scale_;

        SDL_FRect dst{
            (get_x_px() - halfW - 20.0f),
            (get_y_px() - halfH),
            swordWaveProj_.w,
            swordWaveProj_.h
        };

        uint64_t now_ms = SDL_GetTicks();
        SDL_Texture* tex = nullptr;

        if (swordWaveProj_.playing) {
            tex = ACE_CurrentFrameGif(swordWaveProj_, now_ms);
        }

        if (!tex) return;

        SDL_FlipMode flip = (projFacing == Facing::Left) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDL_FRect screenDst = ACE_Cam_WorldToScreen(cam, dst);
        SDL_RenderTextureRotated(appR, tex, nullptr, &screenDst, 0.0, nullptr, flip);


        // DEBUG COLLISIONS
        //SDL_FRect hitboxHighlightDst{ get_hitbox_x_px(HitboxType::AttackRange), get_hitbox_y_px(HitboxType::AttackRange),
        //                get_hitbox_w_pixels(HitboxType::AttackRange), get_hitbox_h_pixels(HitboxType::AttackRange) };
        //SDL_FRect screenHitboxDst = cam.WorldToScreen(hitboxHighlightDst);
        //SDL_SetRenderDrawColor(appR, 0, 0, 255, 40);
        //SDL_RenderFillRect(appR, &screenHitboxDst);

    }

}



#pragma once
#include "AChoiEngine/gameObject/public/GameObject.h"
#include <string>
#include <cstdint>
#include "AChoiEngine/gfx/public/AnimationComponent.h"
#include "AChoiEngine/input/public/Keyboard.h"
#include "AChoiEngine/camera/public/Camera.h"


class SwordWaveProjectile : public ACE_GameObject{

public:
    SwordWaveProjectile(
        float posX_px, float posY_px, float obj_w_pixels, float obj_h_pixels,
        float appW_pixels, float appH_pixels, float worldW_pixels, float worldH_pixels,
        int obj_w_gridUnits, int obj_h_gridUnits, int guSideLen_inPixels,
        float speed_pixels, float direction, int health, int damage)
        : ACE_GameObject(ObjectType::Weapon, AliveState::NotAlive,
            posX_px, posY_px, obj_w_pixels, obj_h_pixels,
            appW_pixels, appH_pixels, worldW_pixels, worldH_pixels,
            obj_w_gridUnits, obj_h_gridUnits, guSideLen_inPixels,
            speed_pixels, direction, health, damage) {}
	
    bool Init(SDL_Renderer* r, Facing facing, float spawnX_px, float spawnY_px, std::string animPath = "");
    void Shutdown();
    void HandleEvent(Keys key);
    void UpdateFixed(double dt, const bool* keys);
    void UpdateFrame(uint64_t now_ms);
    void Render(SDL_Renderer* appR, ACE_Camera2D& cam) const;


	WeaponState ProjectileState = WeaponState::Inactive;
    Facing projFacing = Facing::Right;

    float worldW_ = 0.0f;
    float worldH_ = 0.0f;
    uint64_t chargingStartTime_ms = 0;
    uint64_t chargingDelayTime_ms = 960;

    int damage = 3;
    uint64_t existenceStartTime_ms = 0;
    uint64_t existenceMaxTime_ms = 4000;

private:
    float scale_ = 1.0f;

    ACE_AnimGif swordWaveProj_;

};


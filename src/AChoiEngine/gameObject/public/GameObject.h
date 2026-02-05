#pragma once
#include <SDL3/SDL.h>
#include <array>


enum class ObjectType { Player, Enemy, Npc, Weapon, WorldObj };
enum class Facing { Left, Right };
enum class MovingState { IsMoving, NotMoving };
enum class AttackableState { Invincible, Normal, Vulnerable };
enum class AliveState { IsAlive, NotAlive };
enum class StunnedState { IsStunned, NotStunned };
enum class PlayerState { Idle, Walk, Run, Crouch, Attack1, Attack2, Ability1, Ability2, Ability3, TakesHit, Stunned, Dead };
enum class EnemyState { Idle, Walk, Run, Patrol, Following, Attack1, Ability1, Ability2, TakesHit, Stunned, Dead };
enum class WeaponState { Idle, Move, Collide, Damage, Inactive, Pending, ReadyForLaunch, Charging, OnCooldown };
enum class NpcState { Idle, Walk, Run, Attack, Talk };
enum class WorldObjState { Idle, Move, Inactive };
enum class HitboxType { BodyHitbox, AttackRange, StartAttackRange1, AwarenessRange, BroadHitbox, NarrowHitbox, DealDamageRange1, DealDamageRange2, UtilityRange1, UtilityRange2 };


struct yxhw_px {
    float y_px_;        // y coordinate based on PIXEL coordinate system
    float x_px_;        // x coordinate based on PIXEL coordinate system
    float h_pixels_;    // height of object based on PIXEL measurement system
    float w_pixels_;    // width of object based on PIXEL measurement system
};

struct yxhw_gu {
    int obj_y_gu_ = 0;  // y coordinate based on GRID coordinate system
    int obj_x_gu_ = 0;  // x coordinate based on GRID coordinate system
    int obj_h_gridUnits_;   // height of object based on GRID measurement system
    int obj_w_gridUnits_;   // width of object based on GRID measurement system
};

struct yxPos_px {
    float yPos_px;
    float xPos_px;
};

struct Collisions {
    bool n = false;
    bool e = false;
    bool s = false;
    bool w = false;
};

// velocity vector
struct VelVec {
    float yComp;  // y Component (direciton aka sign, magnitude aka speed)
    float xComp;  // x Component (direciton aka sign, magnitude aka speed)
};

enum class Direction : uint8_t {
    N, NE, E, SE,
    S, SW, W, NW
};

// 8 Directions                                       
constexpr std::array<std::pair<int, int>, 8> DIRS{ {
    {-1, 0},  // N
    {-1, 1},  // NE        
    {0, 1},   // E
    {1, 1},   // SE
    {1, 0},   // S
    {1, -1},  // SW
    {0, -1},  // W
    {-1, -1}} // NW
};

/*
GameObject(
    ObjectType type, AliveState aliveState,
    yxwh_px obj_yxwh_pixel, yxwh_gu obj_yxwh_gUnits, yxwh_px anim_yxwh_pixel,
    float app_w_pixels, float app_h_pixels, float world_w_pixels, float world_h_pixels,
    int guSideLen_inPixels,
    float speed_pixels = 0, float direction = 1, int health = 0, int damage = 0)
    : type_(type), aliveState_(aliveState),
    obj_x_px_(obj_x_px), obj_y_px_(obj_y_px), obj_w_pixels_(obj_w_pixels), obj_h_pixels_(obj_h_pixels),
    app_w_pixels_(app_w_pixels), app_h_pixels_(app_h_pixels), world_w_pixels_(world_w_pixels), world_h_pixels_(world_h_pixels),
    obj_w_gridUnits_(obj_w_gridUnits), obj_h_gridUnits_(obj_h_gridUnits), guSideLen_inPixels_(guSideLen_inPixels),
    speed_pixels_(speed_pixels), direction_(direction), health_(health), damage_(damage) {
}
*/

class GameObject {
public:
    GameObject(
        ObjectType type, AliveState aliveState,
        float obj_x_px, float obj_y_px, float obj_w_pixels, float obj_h_pixels,
        float app_w_pixels, float app_h_pixels, float world_w_pixels, float world_h_pixels, 
        int obj_w_gridUnits, int obj_h_gridUnits, int guSideLen_inPixels, 
        float speed_pixels = 0, float direction = 1, int health = 0, int damage = 0)
        :   type_(type), aliveState_(aliveState),
            obj_x_px_(obj_x_px), obj_y_px_(obj_y_px), obj_w_pixels_(obj_w_pixels), obj_h_pixels_(obj_h_pixels),
            app_w_pixels_(app_w_pixels), app_h_pixels_(app_h_pixels), world_w_pixels_(world_w_pixels), world_h_pixels_(world_h_pixels), 
            obj_w_gridUnits_(obj_w_gridUnits), obj_h_gridUnits_(obj_h_gridUnits), guSideLen_inPixels_(guSideLen_inPixels),
            speed_pixels_(speed_pixels), direction_(direction), health_(health), damage_(damage) {
    }

    // Getters
    ObjectType getType() const;
    AliveState get_alive_state() const;
    float get_x_px() const;
    float get_y_px() const;
    float get_w_pixels() const;
    float get_h_pixels() const;
    yxPos_px get_center_px() const;
    float get_speed_pixels() const;
    float get_direction() const;
    float get_app_w_pixels() const;
    float get_app_h_pixels() const;
    float get_world_w_pixels() const;
    float get_world_h_pixels() const;

    int get_y_gu() const;
    int get_x_gu() const;
    int get_h_gridUnits() const;
    int get_w_gridUnits() const;
    int get_app_h_gridUnits() const;
    int get_app_w_gridUnits() const;
    int get_world_w_gridUnits() const;
    int get_world_h_gridUnits() const;
    int get_singleGU_sideLen_inPixels() const;
    VelVec get_velocity() const;
    float get_mass() const;
    MovingState get_moving_state() const;
    AttackableState get_attackable_state() const;
    StunnedState get_stunned_state() const;

    int get_health() const;
    int get_damage() const;

    //float get_gravity() const;
    //uint64_t get_stunnedTimer() const;

    //std::vector<float> get_hitbox_xywh(HitboxType hitbox_type);
    float get_hitbox_x_px(HitboxType hitbox_type) const;
    float get_hitbox_y_px(HitboxType hitbox_type) const;
    float get_hitbox_w_pixels(HitboxType hitbox_type) const;
    float get_hitbox_h_pixels(HitboxType hitbox_type) const;

    
    // Setters
    void set_type(ObjectType new_type);
    void set_alive_state(AliveState new_alive_state);
    void set_x_px(float new_x);
    void set_y_px(float new_y);
    void set_w_pixels(float new_w);
    void set_h_pixels(float new_h);
    void set_center_px();
    void set_speed_pixels(float new_speed);
    void set_direction(float new_direction);
    void set_app_w_pixels(float new_app_w);
    void set_app_h_pixels(float new_app_h);
    void set_world_w_pixels(float new_world_w);
    void set_world_h_pixels(float new_world_h);
    void set_y_gu(int new_y_gu);
    void set_x_gu(int new_x_gu);
    void set_h_gridUnits(int new_h_gridUnits);
    void set_w_gridUnits(int new_w_gridunits);
    void set_app_h_gridUnits(int new_app_h_gridUnits);
    void set_app_w_gridUnits(int new_app_w_gridUnits);
    void set_world_w_gridUnits(int new_world_w_gridUnits);
    void set_world_h_gridUnits(int new_world_h_gridUnits);
    void set_velocity(VelVec new_velocity_vector);
    void set_mass(float new_mass);
    void set_singleGU_sideLen_inPixels(int new_sideLen);

    void set_health(int new_health);
    void set_damage(int new_damage);
    void set_attackable_state(AttackableState new_attackable_state);
    void set_moving_state(MovingState new_moving_state);
    void set_stunned_state(StunnedState new_stunned_state);

    //void set_gravity(float new_gravity);
    //void set_stunnedTimer(uint64_t new_stunnedTimer);

    void set_hitbox_pixels(HitboxType hitbox_type, float new_hitbox_x_px, float new_hitbox_y_px, float new_hitbox_w_pixels, float new_hitbox_h_pixels);


    Collisions collisionStates_;


private:

    // GridUnits ==> Grid Units (gu) meaning, the size of the object based on
    // a single tile size ==> e.g. 16x16 pixels == 1 gu (grid unit)
    ObjectType type_;
    float obj_y_px_;        // y coordinate based on PIXEL coordinate system
    float obj_x_px_;        // x coordinate based on PIXEL coordinate system
    float obj_h_pixels_;    // height of object based on PIXEL measurement system
    float obj_w_pixels_;    // width of object based on PIXEL measurement system
    yxhw_px obj_yxwh_pixel_ = { 0,0,0,0 };

    int obj_y_gu_ = 0;  // y coordinate based on GRID coordinate system
    int obj_x_gu_ = 0;  // x coordinate based on GRID coordinate system
    int obj_h_gridUnits_;   // height of object based on GRID measurement system
    int obj_w_gridUnits_;   // width of object based on GRID measurement system
    yxhw_gu obj_yxwh_gu_ = { 0,0,0,0 };

    //float anim_y_px_;        
    //float anim_x_px_;        
    //float anim_h_pixels_;    
    //float anim_w_pixels_;
    yxhw_px anim_yxwh_pixel_ = { 0,0,0,0 };

    float app_h_pixels_;
    float app_w_pixels_;
    float world_h_pixels_;
    float world_w_pixels_;

    int app_h_gridUnits_ = 0;
    int app_w_gridUnits_ = 0;
    int world_h_gridUnits_ = 0;
    int world_w_gridUnits_ = 0;

    int guSideLen_inPixels_;
    
    float speed_pixels_;
    float direction_;
    int health_;
    int damage_;
    VelVec velocity_ = { 0, 0 }; // {y component, x component}
    float mass_ = 1;
    yxPos_px centerYX_px_{ 0, 0 };


    MovingState movingState_ = MovingState::NotMoving;
    AttackableState attackableState_ = AttackableState::Normal;
    AliveState aliveState_ = AliveState::IsAlive;
    StunnedState stunnedState_ = StunnedState::NotStunned;


    SDL_FRect bodyHitbox_pixels_{ 0, 0, 0, 0 };
    SDL_FRect attackRange_pixels_{ 0, 0, 0, 0 };
    SDL_FRect startAttackRange1_pixels_{ 0, 0, 0, 0 };
    SDL_FRect awarenessRange_pixels_{ 0, 0, 0, 0 };
    SDL_FRect broadHitbox_pixels_{ 0, 0, 0, 0 };
    SDL_FRect narrowHitbox_pixels_{ 0, 0, 0, 0 };
    SDL_FRect dealDamageRange1_pixels_{ 0, 0, 0, 0 };
    SDL_FRect dealDamageRange2_pixels_{ 0, 0, 0, 0 };
    SDL_FRect utilityRange1_pixels_{ 0, 0, 0, 0 };
    SDL_FRect utilityRange2_pixels_{ 0, 0, 0, 0 };

    //float gravity_;
    //uint64_t stunnedTimer_ = 0;

};
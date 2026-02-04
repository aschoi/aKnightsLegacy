#include "AChoiEngine/gameObject/public/GameObject.h"


ObjectType GameObject::getType() const { return type_; }
float GameObject::get_x_px() const { return obj_x_px_; }
float GameObject::get_y_px() const { return obj_y_px_; }
float GameObject::get_w_pixels() const { return obj_w_pixels_; }
float GameObject::get_h_pixels() const { return obj_h_pixels_; }
float GameObject::get_speed_pixels() const { return speed_pixels_; }
float GameObject::get_direction() const { return direction_; }
float GameObject::get_app_w_pixels() const { return app_w_pixels_; }
float GameObject::get_app_h_pixels() const { return app_h_pixels_; }
float GameObject::get_world_w_pixels() const { return world_w_pixels_; }
float GameObject::get_world_h_pixels() const { return world_h_pixels_; }
int GameObject::get_health() const { return health_; }
int GameObject::get_damage() const { return damage_; }
velVec GameObject::get_velocity() const { return velocity_; }
int GameObject::get_y_gu() const { return obj_y_gu_; }
int GameObject::get_x_gu() const { return obj_x_gu_; }
int GameObject::get_h_gridUnits() const { return obj_h_gridUnits_; }
int GameObject::get_w_gridUnits() const { return obj_w_gridUnits_; }
int GameObject::get_singleGU_sideLen_inPixels() const { return guSideLen_inPixels_; }

AliveState  GameObject::get_alive_state() const { return aliveState_; }
MovingState GameObject::get_moving_state() const { return movingState_; }
AttackableState GameObject::get_attackable_state() const { return attackableState_; }
StunnedState GameObject::get_stunned_state() const { return stunnedState_; }

//float GameObject::get_gravity() const { return gravity_; }
//uint64_t GameObject::get_stunnedTimer() const { return stunnedTimer_; }


float GameObject::get_hitbox_x_px(HitboxType box_type) const {

	float number = 0.0f;
	switch (box_type) {
	case HitboxType::BodyHitbox:
		number = bodyHitbox_pixels_.x;
		break;
	case HitboxType::DealDamageRange1:
		number = dealDamageRange1_pixels_.x;
		break;
	case HitboxType::DealDamageRange2:
		number = dealDamageRange2_pixels_.x;
		break;
	case HitboxType::BroadHitbox:
		number = broadHitbox_pixels_.x;
		break;
	case HitboxType::StartAttackRange1:
		number = startAttackRange1_pixels_.x;
		break;
	case HitboxType::AttackRange:
		number = attackRange_pixels_.x;
		break;
	case HitboxType::AwarenessRange:
		number = awarenessRange_pixels_.x;
		break;
	case HitboxType::UtilityRange1:
		number = utilityRange1_pixels_.x;
		break;
	case HitboxType::UtilityRange2:
		number = utilityRange2_pixels_.x;
		break;
	case HitboxType::NarrowHitbox:
		number = narrowHitbox_pixels_.x;
		break;
	default:
		SDL_Log("get hitbox failed.");
	}
	return number;
}

float GameObject::get_hitbox_y_px(HitboxType box_type) const {

	float number = 0.0f;
	switch (box_type) {
	case HitboxType::BodyHitbox:
		number = bodyHitbox_pixels_.y;
		break;
	case HitboxType::DealDamageRange1:
		number = dealDamageRange1_pixels_.y;
		break;
	case HitboxType::DealDamageRange2:
		number = dealDamageRange2_pixels_.y;
		break;
	case HitboxType::BroadHitbox:
		number = broadHitbox_pixels_.y;
		break;
	case HitboxType::StartAttackRange1:
		number = startAttackRange1_pixels_.y;
		break;
	case HitboxType::AttackRange:
		number = attackRange_pixels_.y;
		break;
	case HitboxType::AwarenessRange:
		number = awarenessRange_pixels_.y;
		break;
	case HitboxType::UtilityRange1:
		number = utilityRange1_pixels_.y;
		break;
	case HitboxType::UtilityRange2:
		number = utilityRange2_pixels_.y;
		break;
	case HitboxType::NarrowHitbox:
		number = narrowHitbox_pixels_.y;
		break;
	default:
		SDL_Log("get hitbox failed.");
	}
	return number;
}

float GameObject::get_hitbox_w_pixels(HitboxType box_type) const {

	float number = 0.0f;
	switch (box_type) {
	case HitboxType::BodyHitbox:
		number = bodyHitbox_pixels_.w;
		break;
	case HitboxType::DealDamageRange1:
		number = dealDamageRange1_pixels_.w;
		break;
	case HitboxType::DealDamageRange2:
		number = dealDamageRange2_pixels_.w;
		break;
	case HitboxType::BroadHitbox:
		number = broadHitbox_pixels_.w;
		break;
	case HitboxType::StartAttackRange1:
		number = startAttackRange1_pixels_.w;
		break;
	case HitboxType::AttackRange:
		number = attackRange_pixels_.w;
		break;
	case HitboxType::AwarenessRange:
		number = awarenessRange_pixels_.w;
		break;
	case HitboxType::UtilityRange1:
		number = utilityRange1_pixels_.w;
		break;
	case HitboxType::UtilityRange2:
		number = utilityRange2_pixels_.w;
		break;
	case HitboxType::NarrowHitbox:
		number = narrowHitbox_pixels_.w;
		break;
	default:
		SDL_Log("get hitbox failed.");
	}
	return number;
}

float GameObject::get_hitbox_h_pixels(HitboxType box_type) const {

	float number = 0.0f;
	switch (box_type) {
	case HitboxType::BodyHitbox:
		number = bodyHitbox_pixels_.h;
		break;
	case HitboxType::DealDamageRange1:
		number = dealDamageRange1_pixels_.h;
		break;
	case HitboxType::DealDamageRange2:
		number = dealDamageRange2_pixels_.h;
		break;
	case HitboxType::BroadHitbox:
		number = broadHitbox_pixels_.h;
		break;
	case HitboxType::StartAttackRange1:
		number = startAttackRange1_pixels_.h;
		break;
	case HitboxType::AttackRange:
		number = attackRange_pixels_.h;
		break;
	case HitboxType::AwarenessRange:
		number = awarenessRange_pixels_.h;
		break;
	case HitboxType::UtilityRange1:
		number = utilityRange1_pixels_.h;
		break;
	case HitboxType::UtilityRange2:
		number = utilityRange2_pixels_.h;
		break;
	case HitboxType::NarrowHitbox:
		number = narrowHitbox_pixels_.h;
		break;
	default:
		SDL_Log("get hitbox failed.");
	}
	return number;
}


//std::vector<float> GameObject::get_hitbox_xywh(HitboxType box_type) {
//	
//	std::vector<float> xywh = xywh = { 0,0,0,0 };
//	switch (box_type) {
//	case HitboxType::BodyHitbox:
//		xywh = { bodyHitbox.x, bodyHitbox.y, bodyHitbox.w, bodyHitbox.h };
//		break;
//	case HitboxType::DealDamageRange1:
//		xywh = { dealDamageRange1.x, dealDamageRange1.y, dealDamageRange1.w, dealDamageRange1.h };
//		break;
//	case HitboxType::DealDamageRange2:
//		xywh = { dealDamageRange2.x, dealDamageRange2.y, dealDamageRange2.w, dealDamageRange2.h };
//		break;
//	case HitboxType::DealDamageRange3:
//		xywh = { dealDamageRange3.x, dealDamageRange3.y, dealDamageRange3.w, dealDamageRange3.h };
//		break;
//
//	case HitboxType::StartAttackRange1:
//		xywh = { startAttackRange1.x, startAttackRange1.y, startAttackRange1.w, startAttackRange1.h };
//		break;
//
//	case HitboxType::AttackRange:
//		xywh = { attackRange.x, attackRange.y, attackRange.w, attackRange.h };
//		break;
//	case HitboxType::AwarenessRange:
//		xywh = { awarenessRange.x, awarenessRange.y, awarenessRange.w, awarenessRange.h };
//		break;
//	default:
//		std::cerr << "set hitbox failed.\n";
//		//xywh = { 0,0,0,0 };
//
//	return xywh;
//}


void GameObject::set_type(ObjectType new_type) { type_ = new_type; }
void GameObject::set_x_px(float new_x) { obj_x_px_ = new_x; }
void GameObject::set_y_px(float new_y) { obj_y_px_ = new_y; }
void GameObject::set_w_pixels(float new_w) { obj_w_pixels_ = new_w; }
void GameObject::set_h_pixels(float new_h) { obj_h_pixels_ = new_h; }
void GameObject::set_speed_pixels(float new_speed) { speed_pixels_ = new_speed; }
void GameObject::set_direction(float new_direction) { direction_ = new_direction; }
void GameObject::set_app_w_pixels(float new_app_w) { app_w_pixels_ = new_app_w; }
void GameObject::set_app_h_pixels(float new_app_h) { app_h_pixels_ = new_app_h; }
void GameObject::set_world_w_pixels(float new_world_w) { world_w_pixels_ = new_world_w; }
void GameObject::set_world_h_pixels(float new_world_h) { world_h_pixels_ = new_world_h; }
void GameObject::set_health(int new_health) { health_ = new_health; }
void GameObject::set_damage(int new_damage) { damage_ = new_damage; }

void GameObject::set_y_gu(int new_y_gu) { obj_y_gu_ = new_y_gu; }
void GameObject::set_x_gu(int new_x_gu) { obj_x_gu_ = new_x_gu; }
void GameObject::set_h_gridUnits(int new_h_gridUnits) { obj_h_gridUnits_ = new_h_gridUnits; }
void GameObject::set_w_gridUnits(int new_w_gridunits) { obj_w_gridUnits_ = new_w_gridunits; }
void GameObject::set_singleGU_sideLen_inPixels(int new_sideLen) { guSideLen_inPixels_ = new_sideLen; }
void GameObject::set_velocity(velVec new_velocity) { velocity_ = new_velocity; }

void GameObject::set_alive_state(AliveState new_alive_state) { aliveState_ = new_alive_state; }
void GameObject::set_attackable_state(AttackableState new_attackable_state) { attackableState_ = new_attackable_state; }
void GameObject::set_moving_state(MovingState new_moving_state) { movingState_  = new_moving_state; }
void GameObject::set_stunned_state(StunnedState new_stunned_state) { stunnedState_ = new_stunned_state; }

//void GameObject::set_gravity(float new_gravity) { gravity_ = new_gravity; }
//void GameObject::set_stunnedTimer(uint64_t new_stunnedTimer) { stunnedTimer_ = new_stunnedTimer; }

void GameObject::set_hitbox_pixels(HitboxType box_type, float new_hitbox_x, float new_hitbox_y, float new_hitbox_w, float new_hitbox_h) {

	switch (box_type) {
	case HitboxType::BodyHitbox:
		bodyHitbox_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::DealDamageRange1:
		dealDamageRange1_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::DealDamageRange2:
		dealDamageRange2_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::BroadHitbox:
		broadHitbox_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::StartAttackRange1:
		startAttackRange1_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::AttackRange:
		attackRange_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::AwarenessRange:
		awarenessRange_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::UtilityRange1:
		utilityRange1_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::UtilityRange2:
		utilityRange2_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	case HitboxType::NarrowHitbox:
		narrowHitbox_pixels_ = { new_hitbox_x, new_hitbox_y, new_hitbox_w, new_hitbox_h };
		break;
	default:
		SDL_Log("set hitbox failed.");
	}

}







#include "AChoiEngine/physics/public/CollisionDetectionComponent.h"
#include <cmath>
#include "AChoiEngine/gameObject/public/GameObject.h"
#include "AChoiEngine/worldLayer/public/Map.h"


bool checkCollision(GameObject& a, HitboxType aHitboxType, GameObject& b, HitboxType bHitboxType) {
	
	float aX;
	float aY;
	float aW;
	float aH;

	float bX;
	float bY;
	float bW;
	float bH;

	switch (aHitboxType) {
	case HitboxType::BodyHitbox:
		aX = a.get_hitbox_x_px(HitboxType::BodyHitbox);
		aY = a.get_hitbox_y_px(HitboxType::BodyHitbox);
		aW = a.get_hitbox_w_pixels(HitboxType::BodyHitbox);
		aH = a.get_hitbox_h_pixels(HitboxType::BodyHitbox);
		break;
	case HitboxType::DealDamageRange1:
		aX = a.get_hitbox_x_px(HitboxType::DealDamageRange1);
		aY = a.get_hitbox_y_px(HitboxType::DealDamageRange1);
		aW = a.get_hitbox_w_pixels(HitboxType::DealDamageRange1);
		aH = a.get_hitbox_h_pixels(HitboxType::DealDamageRange1);
		break;
	case HitboxType::DealDamageRange2:
		aX = a.get_hitbox_x_px(HitboxType::DealDamageRange2);
		aY = a.get_hitbox_y_px(HitboxType::DealDamageRange2);
		aW = a.get_hitbox_w_pixels(HitboxType::DealDamageRange2);
		aH = a.get_hitbox_h_pixels(HitboxType::DealDamageRange2);
		break;
	case HitboxType::StartAttackRange1:
		aX = a.get_hitbox_x_px(HitboxType::StartAttackRange1);
		aY = a.get_hitbox_y_px(HitboxType::StartAttackRange1);
		aW = a.get_hitbox_w_pixels(HitboxType::StartAttackRange1);
		aH = a.get_hitbox_h_pixels(HitboxType::StartAttackRange1);
		break;
	case HitboxType::AttackRange:
		aX = a.get_hitbox_x_px(HitboxType::AttackRange);
		aY = a.get_hitbox_y_px(HitboxType::AttackRange);
		aW = a.get_hitbox_w_pixels(HitboxType::AttackRange);
		aH = a.get_hitbox_h_pixels(HitboxType::AttackRange);
		break;
	case HitboxType::AwarenessRange:
		aX = a.get_hitbox_x_px(HitboxType::AwarenessRange);
		aY = a.get_hitbox_y_px(HitboxType::AwarenessRange);
		aW = a.get_hitbox_w_pixels(HitboxType::AwarenessRange);
		aH = a.get_hitbox_h_pixels(HitboxType::AwarenessRange);
		break;
	case HitboxType::UtilityRange1:
		aX = a.get_hitbox_x_px(HitboxType::UtilityRange1);
		aY = a.get_hitbox_y_px(HitboxType::UtilityRange1);
		aW = a.get_hitbox_w_pixels(HitboxType::UtilityRange1);
		aH = a.get_hitbox_h_pixels(HitboxType::UtilityRange1);
		break;
	case HitboxType::UtilityRange2:
		aX = a.get_hitbox_x_px(HitboxType::UtilityRange2);
		aY = a.get_hitbox_y_px(HitboxType::UtilityRange2);
		aW = a.get_hitbox_w_pixels(HitboxType::UtilityRange2);
		aH = a.get_hitbox_h_pixels(HitboxType::UtilityRange2);
		break;
	default:
		aX = 0;
		aY = 0;
		aW = 0;
		aH = 0;
		break;
	}

	switch (bHitboxType) {
	case HitboxType::BodyHitbox:
		bX = b.get_hitbox_x_px(HitboxType::BodyHitbox);
		bY = b.get_hitbox_y_px(HitboxType::BodyHitbox);
		bW = b.get_hitbox_w_pixels(HitboxType::BodyHitbox);
		bH = b.get_hitbox_h_pixels(HitboxType::BodyHitbox);
		break;
	case HitboxType::DealDamageRange1:
		bX = b.get_hitbox_x_px(HitboxType::DealDamageRange1);
		bY = b.get_hitbox_y_px(HitboxType::DealDamageRange1);
		bW = b.get_hitbox_w_pixels(HitboxType::DealDamageRange1);
		bH = b.get_hitbox_h_pixels(HitboxType::DealDamageRange1);
		break;
	case HitboxType::DealDamageRange2:
		bX = b.get_hitbox_x_px(HitboxType::DealDamageRange2);
		bY = b.get_hitbox_y_px(HitboxType::DealDamageRange2);
		bW = b.get_hitbox_w_pixels(HitboxType::DealDamageRange2);
		bH = b.get_hitbox_h_pixels(HitboxType::DealDamageRange2);
		break;
	case HitboxType::StartAttackRange1:
		bX = b.get_hitbox_x_px(HitboxType::StartAttackRange1);
		bY = b.get_hitbox_y_px(HitboxType::StartAttackRange1);
		bW = b.get_hitbox_w_pixels(HitboxType::StartAttackRange1);
		bH = b.get_hitbox_h_pixels(HitboxType::StartAttackRange1);
		break;
	case HitboxType::AttackRange:
		bX = b.get_hitbox_x_px(HitboxType::AttackRange);
		bY = b.get_hitbox_y_px(HitboxType::AttackRange);
		bW = b.get_hitbox_w_pixels(HitboxType::AttackRange);
		bH = b.get_hitbox_h_pixels(HitboxType::AttackRange);
		break;
	case HitboxType::AwarenessRange:
		bX = b.get_hitbox_x_px(HitboxType::AwarenessRange);
		bY = b.get_hitbox_y_px(HitboxType::AwarenessRange);
		bW = b.get_hitbox_w_pixels(HitboxType::AwarenessRange);
		bH = b.get_hitbox_h_pixels(HitboxType::AwarenessRange);
		break;
	case HitboxType::UtilityRange1:
		bX = b.get_hitbox_x_px(HitboxType::UtilityRange1);
		bY = b.get_hitbox_y_px(HitboxType::UtilityRange1);
		bW = b.get_hitbox_w_pixels(HitboxType::UtilityRange1);
		bH = b.get_hitbox_h_pixels(HitboxType::UtilityRange1);
		break;
	case HitboxType::UtilityRange2:
		bX = b.get_hitbox_x_px(HitboxType::UtilityRange2);
		bY = b.get_hitbox_y_px(HitboxType::UtilityRange2);
		bW = b.get_hitbox_w_pixels(HitboxType::UtilityRange2);
		bH = b.get_hitbox_h_pixels(HitboxType::UtilityRange2);
		break;
	default:
		bX = 1;
		bY = 1;
		bW = 1;
		bH = 1;
		break;
	}


	float aCenterX = aX + aW / 2;
	float aCenterY = aY + aH / 2;

	float bCenterX = bX + bW / 2;
	float bCenterY = bY + bH / 2;

	float deltaX = abs(aCenterX - bCenterX);
	float deltaY = abs(aCenterY - bCenterY);

	float overlapX = (aW / 2) + (bW / 2) - deltaX;
	float overlapY = (aH / 2) + (bH / 2) - deltaY;

	if (overlapX > 0 && overlapY > 0) {
		return true;
	}

	return false;
};




bool checkBodyBodyCollision(GameObject& a, GameObject& b) {	

	float aX = a.get_hitbox_x_px(HitboxType::BodyHitbox);
	float aY = a.get_hitbox_y_px(HitboxType::BodyHitbox);
	float aW = a.get_hitbox_w_pixels(HitboxType::BodyHitbox);
	float aH = a.get_hitbox_h_pixels(HitboxType::BodyHitbox);

	float bX = b.get_hitbox_x_px(HitboxType::BodyHitbox);
	float bY = b.get_hitbox_y_px(HitboxType::BodyHitbox);
	float bW = b.get_hitbox_w_pixels(HitboxType::BodyHitbox);
	float bH = b.get_hitbox_h_pixels(HitboxType::BodyHitbox);

	float aCenterX = aX + aW / 2;
	float aCenterY = aY + aH / 2;

	float bCenterX = bX + bW / 2;
	float bCenterY = bY + bH / 2;

	float deltaX = abs(aCenterX - bCenterX);
	float deltaY = abs(aCenterY - bCenterY);

	float overlapX = (aW / 2) + (bW / 2) - deltaX;
	float overlapY = (aH / 2) + (bH / 2) - deltaY;

	if (overlapX > 0 && overlapY > 0) {
		return true;
	}

	return false;
};


bool checkBodyAwarenessCollision(GameObject& a, GameObject& b) {

	float aX = a.get_hitbox_x_px(HitboxType::BodyHitbox);
	float aY = a.get_hitbox_y_px(HitboxType::BodyHitbox);
	float aW = a.get_hitbox_w_pixels(HitboxType::BodyHitbox);
	float aH = a.get_hitbox_h_pixels(HitboxType::BodyHitbox);

	float bX = b.get_hitbox_x_px(HitboxType::AwarenessRange);
	float bY = b.get_hitbox_y_px(HitboxType::AwarenessRange);
	float bW = b.get_hitbox_w_pixels(HitboxType::AwarenessRange);
	float bH = b.get_hitbox_h_pixels(HitboxType::AwarenessRange);

	float aCenterX = aX + aW / 2;
	float aCenterY = aY + aH / 2;

	float bCenterX = bX + bW / 2;
	float bCenterY = bY + bH / 2;

	float deltaX = abs(aCenterX - bCenterX);
	float deltaY = abs(aCenterY - bCenterY);

	float overlapX = (aW / 2) + (bW / 2) - deltaX;
	float overlapY = (aH / 2) + (bH / 2) - deltaY;

	if (overlapX > 0 && overlapY > 0) {
		return true;
	}

	return false;
};


bool checkAttackBodyCollision(GameObject& a, GameObject& b) {

	float aX = a.get_hitbox_x_px(HitboxType::AttackRange);
	float aY = a.get_hitbox_y_px(HitboxType::AttackRange);
	float aW = a.get_hitbox_w_pixels(HitboxType::AttackRange);
	float aH = a.get_hitbox_h_pixels(HitboxType::AttackRange);

	float bX = b.get_hitbox_x_px(HitboxType::BodyHitbox);
	float bY = b.get_hitbox_y_px(HitboxType::BodyHitbox);
	float bW = b.get_hitbox_w_pixels(HitboxType::BodyHitbox);
	float bH = b.get_hitbox_h_pixels(HitboxType::BodyHitbox);

	float aCenterX = aX + aW / 2;
	float aCenterY = aY + aH / 2;

	float bCenterX = bX + bW / 2;
	float bCenterY = bY + bH / 2;

	float deltaX = abs(aCenterX - bCenterX);
	float deltaY = abs(aCenterY - bCenterY);

	float overlapX = (aW / 2) + (bW / 2) - deltaX;
	float overlapY = (aH / 2) + (bH / 2) - deltaY;

	if (overlapX > 0 && overlapY > 0) {
		return true;
	}

	return false;
};


bool checkBodyMapedgeCollision(GameObject& a) {

	float aX = a.get_hitbox_x_px(HitboxType::AttackRange);
	float aY = a.get_hitbox_y_px(HitboxType::AttackRange);
	float aW = a.get_hitbox_w_pixels(HitboxType::AttackRange);
	float aH = a.get_hitbox_h_pixels(HitboxType::AttackRange);

	float aCenterX = aX + aW / 2;
	float aCenterY = aY + aH / 2;

	// TEMP HARD CODE!!  MAKE SURE TO FIX IT
	if (aCenterX <= 0 || aCenterX >= a.get_world_w_pixels() - 100) {
		return true;
	}
	if (aCenterY <= 0 || aCenterY >= a.get_world_h_pixels() - 50) {
		return true;
	}
	return false;
}

bool checkAttackWallCollision(GameObject& a, Map& m) {
	float aX_west = a.get_hitbox_x_px(HitboxType::AttackRange);
	float aX_east = a.get_hitbox_x_px(HitboxType::AttackRange) + a.get_hitbox_w_pixels(HitboxType::AttackRange);

	float aY_mid = a.get_hitbox_y_px(HitboxType::AttackRange) + a.get_hitbox_h_pixels(HitboxType::AttackRange)/2;

	if (m.intGridType(aX_west, aY_mid)) return true;
	if (m.intGridType(aX_east, aY_mid)) return true;
	return false;

}

int checkRepulsionXAxis(GameObject& a, GameObject& b) {

	float aX = a.get_hitbox_x_px(HitboxType::UtilityRange1);
	float aW = a.get_hitbox_w_pixels(HitboxType::UtilityRange1);

	float bX = b.get_hitbox_x_px(HitboxType::UtilityRange1);
	float bW = b.get_hitbox_w_pixels(HitboxType::UtilityRange1);

	float aCenterX = aX + aW / 2;
	float bCenterX = bX + bW / 2;

	float deltaX = abs(aCenterX - bCenterX);
	float overlapX = (aW / 2) + (bW / 2) - deltaX;

	if (overlapX > 0) {
		if (aCenterX <= bCenterX) return 1;
		else return 2;
	}
	return 0;
};

int checkRepulsionYAxis(GameObject& a, GameObject& b) {

	float aY = a.get_hitbox_y_px(HitboxType::UtilityRange1);
	float aH = a.get_hitbox_h_pixels(HitboxType::UtilityRange1);

	float bY = b.get_hitbox_y_px(HitboxType::UtilityRange1);
	float bH = b.get_hitbox_h_pixels(HitboxType::UtilityRange1);

	float aCenterY = aY + aH / 2;
	float bCenterY = bY + bH / 2;

	float deltaY = abs(aCenterY - bCenterY);
	float overlapY = (aH / 2) + (bH / 2) - deltaY;

	if (overlapY > 0) {
		if (aCenterY <= bCenterY) return 1;
		else return 2;
	}

	return 0;
};
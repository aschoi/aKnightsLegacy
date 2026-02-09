#include "AChoiEngine/physics/public/CollisionResponseSys.h"
#include "AChoiEngine/physics/public/OverlappingHitboxDetectionSys.h"
#include "AChoiEngine/gameObject/public/GameObject.h"
#include <vector>


void ACE_basicAllEntitiesCollisionResolution(std::vector<ACE_GameObject*>& entities) {

	for (ACE_GameObject* a : entities) {
		a->collisionStates_.n = false;
		a->collisionStates_.s = false;
		a->collisionStates_.e = false;
		a->collisionStates_.w = false;
	}

	for (ACE_GameObject* a : entities) {
		for (ACE_GameObject* b : entities) {
			if (a == b) continue;
			if (ACE_checkCollision(*a, HitboxType::UtilityRange1, *b, HitboxType::UtilityRange1)) {

				int result = ACE_checkRepulsionXAxis(*a, *b);
				if (result == 1) {
					a->collisionStates_.e = true;
					b->collisionStates_.w = true;
				}
				else if (result == 2) {
					a->collisionStates_.w = true;
					b->collisionStates_.e = true;
				}
				result = ACE_checkRepulsionYAxis(*a, *b);
				if (result == 1) {
					a->collisionStates_.s = true;
					b->collisionStates_.n = true;
				}
				else if (result == 2) {
					a->collisionStates_.n = true;
					b->collisionStates_.s = true;
				}
			}
		}
	}
}
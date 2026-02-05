#include "AChoiEngine/physics/public/CollisionResponse.h"
#include <vector>
#include "AChoiEngine/physics/public/CollisionDetectionComponent.h"
#include "AChoiEngine/gameObject/public/GameObject.h"


void basicAllEntitiesCollisionResolution(std::vector<GameObject*>& entities) {

    for (GameObject* a : entities) {
        a->collisionStates_.n = false;
        a->collisionStates_.s = false;
        a->collisionStates_.e = false;
        a->collisionStates_.w = false;
    }

    for (GameObject* a : entities) {
        for (GameObject* b : entities) {
            if (a == b) continue;
            if (checkCollision(*a, HitboxType::UtilityRange1, *b, HitboxType::UtilityRange1)) {

                int result = checkRepulsionXAxis(*a, *b);
                if (result == 1) {
                    a->collisionStates_.e = true;
                    b->collisionStates_.w = true;
                }
                else if (result == 2) {
                    a->collisionStates_.w = true;
                    b->collisionStates_.e = true;
                }
                result = checkRepulsionYAxis(*a, *b);
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
#pragma once
#include "AChoiEngine/gameObject/public/GameObject.h"

class Map;

bool checkBodyBodyCollision(GameObject& a, GameObject& b);
bool checkBodyAwarenessCollision(GameObject& a, GameObject& b);
bool checkAttackBodyCollision(GameObject& a, GameObject& b);
bool checkBodyMapedgeCollision(GameObject& a);
bool checkCollision(GameObject& a, HitboxType aHitboxType, GameObject& b, HitboxType bHitboxType);
void bodyBodySurroundingBodyReuplsion(GameObject& a);
bool checkAttackWallCollision(GameObject& a, Map& m);


bool checkANorthBSouthCollision(GameObject& a, GameObject& b);
bool checkAEastBWestCollision(GameObject& a, GameObject& b);
bool checkASouthBNorthCollision(GameObject& a, HitboxType aHitboxType, GameObject& b, HitboxType bHitboxType);
bool checkAWestBEastCollision(GameObject& a, HitboxType aHitboxType, GameObject& b, HitboxType bHitboxType);

// Narrow Phase checks
bool checkRepulsionANorthBSouth(GameObject& a, GameObject& b);
bool checkRepulsionAEastBWest(GameObject& a, GameObject& b);
bool checkRepulsionASouthBNorth(GameObject& a, GameObject& b);
bool checkRepulsionAWestBEast(GameObject& a, GameObject& b);


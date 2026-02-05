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
int checkRepulsionXAxis(GameObject& a, GameObject& b);
int checkRepulsionYAxis(GameObject& a, GameObject& b);



#pragma once
#include "AChoiEngine/gameObject/public/GameObject.h"


class ACE_Map;

bool ACE_checkBodyBodyCollision(ACE_GameObject& a, ACE_GameObject& b);
bool ACE_checkBodyAwarenessCollision(ACE_GameObject& a, ACE_GameObject& b);
bool ACE_checkAttackBodyCollision(ACE_GameObject& a, ACE_GameObject& b);
bool ACE_checkBodyMapedgeCollision(ACE_GameObject& a);
bool ACE_checkCollision(ACE_GameObject& a, HitboxType aHitboxType, ACE_GameObject& b, HitboxType bHitboxType);
bool ACE_checkAttackWallCollision(ACE_GameObject& a, ACE_Map& m);
int ACE_checkRepulsionXAxis(ACE_GameObject& a, ACE_GameObject& b);
int ACE_checkRepulsionYAxis(ACE_GameObject& a, ACE_GameObject& b);







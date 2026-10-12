#pragma once
#include "entity.h"

void moveVelCheck(Entity& entity, float elapsed);
void boundaryCheck(Entity& entity, float height, float width);
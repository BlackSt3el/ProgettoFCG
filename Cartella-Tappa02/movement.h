#pragma once
#include "entity.h"
#include "constants.h"
#include "sound.h"
#include <algorithm>
#include <cmath>

void moveVelCheck(Entity& entity, float elapsed);
void updateDirections(Entity& e);
void boundaryCheck(Entity& entity, float height, float width);
void directionCheck(Entity& entity);
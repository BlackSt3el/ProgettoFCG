#pragma once
#include "entity.h"
#include "constants.h"
#include "sound.h"
#include "tiles.h"
#include <algorithm>
#include <cmath>

void moveVelCheck(Entity& entity, float elapsed, Level& level);
void updateDirections(Entity& e);
void boundaryCheck(Entity& entity, float width, float height, Level& level);
void directionCheck(Entity& entity);
#pragma once
#include "entity.h"
#include "constants.h"
#include <algorithm>
#include <cmath>
#include <SFML/Audio.hpp>

void moveVelCheck(Entity& entity, float elapsed);
void updateDirections(Entity& e);
void boundaryCheck(Entity& entity, float height, float width);
void directionCheck(Entity& entity);
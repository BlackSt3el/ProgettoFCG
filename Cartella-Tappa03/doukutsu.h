#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "movement.h"
#include "entity.h"
#include "constants.h"
#include "textures.h"
#include "bullet.h"
#include "sound.h"

void toggle_fullscreen(sf::RenderWindow& window, bool fullscreen, sf::ContextSettings settings);
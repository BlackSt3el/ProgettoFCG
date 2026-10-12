#pragma once
#include <SFML/Graphics.hpp>
#include "../Cartella-risorse/textures.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "movement.h"
#include "entity.h"
#include "constants.h"
#include "textures.h"

void toggle_fullscreen(sf::RenderWindow& window, bool fullscreen, sf::ContextSettings settings);
#pragma once
#include <SFML/Graphics.hpp>

const int window_width = 640;
const int window_height = 360;
const float mult = 1;//increase size of objects and mantain ratio
const float max_frame_rate = 180;
const float entity_velocity_cap = 120;
const float min_jump_speed = 40;

const int QUOTE_WIDTH = 16; 
const int QUOTE_HEIGHT = 16;
const sf::Vector2i QUOTE_ORIGIN{68,122};

const sf::Vector2f rectangleSize = {QUOTE_WIDTH, QUOTE_HEIGHT};

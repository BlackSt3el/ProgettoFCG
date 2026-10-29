#pragma once
#include <SFML/Graphics.hpp>


//Window settings
const int window_width = 640;
const int window_height = 360;
const int max_frame_rate = 180;

//Physics
const float accel = 1500.f;
const float entity_velocity_cap = 120.f;
const float min_jump_speed = 480.f;
const float gravity_held = 900.f;
const float gravity = 2200.f;
const float max_fall_speed= 700.f;

//Textures
enum class Directions {Left, Right, UpRight, UpLeft, DownRight, DownLeft};
const float FrameTime = 0.2f; //seconds per frame of animations

//Quote data
const int SPRITE_WIDTH = 16; 
const int SPRITE_HEIGHT = 16;
const sf::Vector2i QUOTE_ORIGIN{68,122};
const std::string quoteTexturePath= "./../Cartella-risorse/DoukutsuCharSprites.png";
const sf::Vector2f hitboxSize = {SPRITE_WIDTH, SPRITE_HEIGHT};

//Bullet data
const float bullet_speed = 600.f;
const float bullet_lifetime = 0.25f;   // seconds; speed * lifetime = range
const int   max_bullets = 100;

//Guns
struct GunPosition {
    sf::IntRect rect;      // position and size in the texture sheet
    sf::Vector2f offset;   // hand position from the character's center, in unscaled pixels
};

//------------------------------------ Polar Star ------------------------------------//

//polar star is 9x3 and the right looking one is at 58,27
static const GunPosition polarStar[6] ={
    {sf::IntRect ({53,11}, {9,3}), sf::Vector2f({-6.5f, 4.5f})},//left
    {sf::IntRect ({58,27}, {9,3}), sf::Vector2f({6.5f, 4.5f})},//right
    {sf::IntRect ({58,50}, {3,7}), sf::Vector2f({3.5f, -6.f})},//upRight
    {sf::IntRect ({59,34}, {3,7}), sf::Vector2f({-3.5f, -6.f})},//upLeft
    {sf::IntRect ({55,87}, {3,7}), sf::Vector2f({0.5f, 6.5f})},//downRight
    {sf::IntRect ({62,71}, {3,7}), sf::Vector2f({-0.5f, 6.5f})},//downLeft
};

const sf::IntRect PSspritePos{{128,232},{SPRITE_WIDTH, SPRITE_HEIGHT}};

//------------------------------------ Polar Star ------------------------------------//
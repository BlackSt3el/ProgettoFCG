#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

struct Entity
{    
    sf::RectangleShape hitbox{rectangleSize};

    sf::Vector2f pos = {0,0};

    sf::Texture texture;

    sf::Sprite charSprite{texture};

    sf::Sprite gunSprite{texture};
    
    //Player is going in a direction
    bool inputUp = false, inputDown = false, inputLeft = false, inputRight = false;
    bool facingRight = true; bool turnBack = false;   
    bool special1 = false; bool special2 = false;

    //Player is looking in a direction
    Directions direction = Directions::Right;
    
    bool holdingJump = false;//variable jumping height on hold
    
    bool jumped = false;//jumping button was inputted, returned to false once jump is initiated
    
    bool grounded = false;

    float xvel = 0, yvel = 0;

    float animTimer = 0.f;
    int animFrame = 0;

    Entity(std::string texturePath);
};

void updateEntity(Entity& e, float elapsed);
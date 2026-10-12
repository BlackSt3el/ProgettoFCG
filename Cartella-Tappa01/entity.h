#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

struct Entity
{    
    sf::RectangleShape rectangle{rectangleSize};

    sf::Vector2f pos = {0,0};
    //Player is going in a direction
    bool inputUp = false, inputDown = false, inputLeft = false, inputRight = false;
    //Player is looking in a direction
    bool faceUp = false, faceDown = false, faceLeft = false, faceRight = false; 
    
    bool holdingJump = false;//variable jumping height on hold
    bool jumped = false;//jumping button was inputted, returned to false once jump is initiated
    
    bool grounded = false;

    float xvel = 0, yvel = 0;

};
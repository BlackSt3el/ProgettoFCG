#include "entity.h"
#include "textures.h"
#include "movement.h"
#include <iostream>


Entity::Entity(sf::Texture& texture)
    : charSprite(texture), gunSprite(texture)//fixes the fact that sprites can't have empty declarations as variables
{
    quoteSpriteDirection(*this, charSprite, gunSprite);
    
    //set origins and scale textures
    auto r1 = charSprite.getTextureRect();
    charSprite.setOrigin({r1.size.x / 2.f, r1.size.y / 2.f});
}


void updateEntity(Entity& e, float elapsed){
    
    moveVelCheck(e, elapsed);

    boundaryCheck(e, e.hitbox.getSize().x, e.hitbox.getSize().y);

    directionCheck(e);
}
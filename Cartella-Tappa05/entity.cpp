#include "entity.h"
#include "textures.h"
#include "movement.h"


Entity::Entity(sf::Texture& texture)
    : charSprite(texture), gunSprite(texture)//fixes the fact that sprites can't have empty declarations as variables
{
    quoteSpriteDirection(*this, charSprite, gunSprite);
    
    //set origins and scale textures
    auto r1 = charSprite.getTextureRect();
    charSprite.setOrigin({r1.size.x / 2.f, r1.size.y / 2.f});
}


void updateEntity(Entity& e, float elapsed, Level& level){
    
    moveVelCheck(e, elapsed, level);

    boundaryCheck(e, e.hitbox.getSize().x, e.hitbox.getSize().y, level);

    directionCheck(e);
}
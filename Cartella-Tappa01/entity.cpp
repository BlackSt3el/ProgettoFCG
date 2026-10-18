#include "entity.h"
#include "textures.h"
#include "movement.h"
#include <iostream>


Entity::Entity(std::string texturePath){
    if(!texture.loadFromFile(texturePath)){
        std::cout << "Image Failed to load" << std::endl;
    }
    else{
        charSprite.setTexture(texture, true);
        gunSprite.setTexture(texture, true);

        quoteSpriteDirection(*this, charSprite, gunSprite);
        
        //set origins and scale textures
        auto r1 = charSprite.getTextureRect();
        charSprite.setOrigin({r1.size.x / 2.f, r1.size.y / 2.f});
        charSprite.scale(sf::Vector2f(scale,scale));  
    }
}

void updateEntity(Entity& e, float elapsed){
    
    moveVelCheck(e, elapsed);

    boundaryCheck(e, e.hitbox.getSize().x, e.hitbox.getSize().y);

    directionCheck(e);
}
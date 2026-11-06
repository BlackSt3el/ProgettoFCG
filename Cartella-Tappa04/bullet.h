#pragma once
#include "constants.h"
#include "entity.h"
#include "sound.h"
#include <cmath>


struct Bullet{
    sf::Vector2f pos;
    sf::Vector2f vel;
    int level = 0;
    int damage;
    float life = bullet_lifetime;
    Directions dir;
    sf::IntRect spritePosition;
    Bullet(sf::IntRect position, Directions shooterDir){ 
        dir = shooterDir;
        if(dir == Directions::Right||dir == Directions::Left)spritePosition = position;
        else spritePosition = sf::IntRect({position.position.x+SPRITE_WIDTH, position.position.y},position.size);
    }
};

void spawnBullet(std::vector<Bullet>& bullets, Entity& shooter);
void updateBullets(Entity& shooter,std::vector<Bullet>& bullets, float elapsed);
void drawBullets(sf::RenderWindow& window, std::vector<Bullet>& bullets, sf::Texture& texture);
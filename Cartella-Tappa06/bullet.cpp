#include "bullet.h"

void spawnBullet(std::vector<Bullet>& bullets, Entity& shooter){
    Bullet newBullet{PSspritePos, shooter.direction};
    switch (shooter.direction)
    {
    case Directions::Right:
        newBullet.pos = {shooter.pos.x+SPRITE_WIDTH, shooter.pos.y};
        newBullet.vel = {bullet_speed, 0.f};
        break;
    case Directions::Left:
        newBullet.pos = {shooter.pos.x-SPRITE_WIDTH, shooter.pos.y};
        newBullet.vel = {-bullet_speed, 0.f};
        break;
    case Directions::UpLeft:
    case Directions::UpRight:
        newBullet.pos = {shooter.pos.x, shooter.pos.y-SPRITE_HEIGHT};
        newBullet.vel = {0.f , -bullet_speed};
        break;
    case Directions::DownLeft:
    case Directions::DownRight:
        newBullet.pos = {shooter.pos.x, shooter.pos.y+SPRITE_HEIGHT};
        newBullet.vel = {0.f , bullet_speed};
        break;
    default:
        break;
    }
    bullets.push_back(newBullet);
    sounds::PSSound.play();
}

void updateBullets(Entity& shooter, std::vector<Bullet>& bullets, float elapsed){
    for(size_t cur = 0 ; cur < bullets.size();){
        bullets[cur].pos += bullets[cur].vel*elapsed;
        bullets[cur].life -= elapsed;
        if(bullets[cur].life <= 0.f){
            bullets.erase(bullets.begin()+cur);
        }
        else cur++;
    }
    if(shooter.shooting&&(int)bullets.size() < max_bullets){
        spawnBullet(bullets, shooter);
    }
    shooter.shooting = false;//for now I'm only implementing a gun that shoots once per key press
    
}

void drawBullets(sf::RenderWindow& window, std::vector<Bullet>& bullets, sf::Texture& texture){
    sf::Sprite sprite(texture);

    for(const Bullet& b : bullets){
        sprite.setTextureRect(b.spritePosition);
        sprite.setOrigin({b.spritePosition.size.x / 2.f, b.spritePosition.size.y / 2.f});
        sprite.setPosition({std::round(b.pos.x),std::round(b.pos.y)});//rounding to prevent pixel bleeding from adjacent sprites
        window.draw(sprite);
    }
}
#include "movement.h"

void moveVelCheck(Entity& entity, float elapsed, Level& level){
    
    //directional movement

    if(entity.inputLeft)entity.xvel -= accel*elapsed;
    if(entity.inputRight)entity.xvel += accel*elapsed;
    
    //Friction
    
    if(entity.grounded&&((!entity.inputLeft&&!entity.inputRight)||(entity.inputLeft&&entity.inputRight)))entity.xvel *= std::pow(0.8f,elapsed*60.f);//multypling by 60 makes it so that physics are consistent with any frame rate, 60 are the ticks that should pass in a second
    entity.xvel = std::clamp(entity.xvel, -entity_velocity_cap, entity_velocity_cap);

    //jumping logic
    if(entity.jumped && entity.grounded){
        entity.yvel = -min_jump_speed;
        entity.grounded = false;
        sounds::jumpSound.play();
    }
    entity.jumped = false;

    //gravity
    float g = (entity.holdingJump && entity.yvel<0) ? gravity_held : gravity;//different gravity depending on holding jump
    entity.yvel += g*elapsed;

    entity.yvel = std::clamp(entity.yvel, -min_jump_speed, max_fall_speed);

    entity.pos.x += entity.xvel*elapsed;
    //Check touched tiles
    //Compute edges of player rectangle
    float left   = entity.pos.x - hitboxSize.x / 2.f;
    float right  = entity.pos.x + hitboxSize.x / 2.f - delta;
    float top    = entity.pos.y - hitboxSize.y / 2.f;
    float bottom = entity.pos.y + hitboxSize.y / 2.f - delta;
    //Convert the edges to the adjecent tiles
    int leftTile = static_cast<int>(std::floor(left/16.f));
    int rightTile = static_cast<int>(std::floor(right/16.f));
    int topTile = static_cast<int>(std::floor(top/16.f));
    int bottomTile = static_cast<int>(std::floor(bottom/16.f));

    //Going to the right
    if(entity.xvel > 0){
        for(int ty = topTile; ty <= bottomTile; ty++){
            if(level.isSolid(rightTile , ty)){
                entity.xvel = 0;
                entity.pos.x = (rightTile)*16.f-hitboxSize.x/2;
            }
        }
    }
    //Going to the left
    else if(entity.xvel < 0){
        for(int ty = topTile; ty <= bottomTile; ty++){
            if(level.isSolid(leftTile, ty)){
                entity.xvel = 0;
                entity.pos.x = (rightTile)*16.f+hitboxSize.x/2;
            }
        }
    }

    
    entity.pos.y += entity.yvel*elapsed;
    //Check touched tiles
    //Recompute edges of player rectangle since it was moved
    left   = entity.pos.x - hitboxSize.x / 2.f;
    right  = entity.pos.x + hitboxSize.x / 2.f - delta;
    top    = entity.pos.y - hitboxSize.y / 2.f;
    bottom = entity.pos.y + hitboxSize.y / 2.f - delta;

    leftTile = static_cast<int>(std::floor(left/16.f));
    rightTile = static_cast<int>(std::floor(right/16.f));
    topTile = static_cast<int>(std::floor(top/16.f));
    bottomTile = static_cast<int>(std::floor(bottom/16.f));
    
    entity.grounded = false;//every frame default to fall and set it true if collision is detected
    //falling onto the floor
    if(entity.yvel > 0){
        for(int tx = leftTile; tx <= rightTile; tx++){
            if(level.isSolid(tx , bottomTile)){
                entity.yvel = 0;
                entity.pos.y = (bottomTile)*16.f-hitboxSize.y/2;
                entity.grounded = true;
            }
        }
    }
    //jumping into a ceiling
    else if(entity.yvel < 0){
        for(int tx = leftTile; tx <= rightTile; tx++){
            if(level.isSolid(tx, topTile)){
                entity.yvel = 0;
                entity.pos.y = (topTile+1)*16.f+hitboxSize.y/2;
            }
        }
    }
}

void boundaryCheck(Entity& entity, float width, float height, Level& level){
    if(entity.grounded == false && entity.pos.y >= levelSize.y-height/2.f){entity.grounded = true; sounds::walkSound.play();}
    entity.pos.x = std::clamp(entity.pos.x, width/2.f, levelSize.x - width/2.f);
    entity.pos.y = std::clamp(entity.pos.y, height/2.f, levelSize.y - height/2.f);
    entity.hitbox.setPosition(entity.pos);
}

void directionCheck(Entity& e){
    bool right = e.facingRight;//last inputted direction is saved even without input
    
    if(e.grounded && e.inputDown && !e.inputUp && !e.inputLeft && !e.inputRight)
        e.turnBack = true;

    if(e.inputLeft || e.inputRight || e.inputUp || e.jumped || !e.grounded || e.special1 || e.special2 || e.shooting)
        e.turnBack = false;
    
    if(e.inputLeft && !e.inputRight) right = false;
    else if(!e.inputLeft && e.inputRight)right = true;

    e.facingRight = right;
    
    if(e.inputUp) e.direction = right ? Directions::UpRight : Directions::UpLeft;
    
    else if(e.inputDown&&!e.grounded) e.direction = right ? Directions::DownRight : Directions::DownLeft;
    
    else{
        e.direction = right ? Directions::Right : Directions::Left;
    }

    e.special1 = false;
    e.special2 = false;
}
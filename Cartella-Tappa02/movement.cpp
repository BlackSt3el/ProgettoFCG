#include "movement.h"

void moveVelCheck(Entity& entity, float elapsed){
    
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

    entity.pos.y += entity.yvel*elapsed;
    entity.pos.x += entity.xvel*elapsed;
}

void boundaryCheck(Entity& entity, float height, float width){
    if(entity.grounded == false && entity.pos.y >= window_height-height/2.f){entity.grounded = true; sounds::walkSound.play();}
    entity.pos.x = std::clamp(entity.pos.x, width/2.f, window_width - width/2.f);
    entity.pos.y = std::clamp(entity.pos.y, height/2.f, window_height - height/2.f);
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
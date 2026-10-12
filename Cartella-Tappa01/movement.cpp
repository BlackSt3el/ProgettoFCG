#include "movement.h"
#include "constants.h"
#include <algorithm>

void moveVelCheck(Entity& entity, float elapsed){
    
    //directional movement

    if(entity.inputLeft)entity.xvel -= 5;
    if(entity.inputRight)entity.xvel += 5;

    if(!entity.inputLeft&&!entity.inputRight)entity.xvel *= 0.8f;

    entity.xvel = std::clamp(entity.xvel, -entity_velocity_cap, entity_velocity_cap);
    entity.yvel = std::clamp(entity.yvel, -entity_velocity_cap, entity_velocity_cap);

    //jumping logic
    if(entity.jumped && entity.grounded){
        entity.yvel = -min_jump_speed;
        entity.grounded = false;
    }
    if(entity.holdingJump&& !entity.grounded)entity.yvel -= min_jump_speed*0.5; 
    entity.jumped = false;

    //gravity
    if(!entity.holdingJump && !entity.grounded)entity.yvel += 10;

    entity.pos.y += entity.yvel*elapsed;
    entity.pos.x += entity.xvel*elapsed;



}

void boundaryCheck(Entity& entity, float height, float width){
    if(entity.pos.y >= window_height-height/2.f)entity.grounded = true;
    entity.pos.x = std::clamp(entity.pos.x, width/2.f, window_width - width/2.f);
    entity.pos.y = std::clamp(entity.pos.y, height/2.f, window_height - height/2.f);
}
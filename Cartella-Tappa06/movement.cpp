#include "movement.h"

void moveVelCheck(Entity& entity, float elapsed, Level& level){

//------------------------------- directional movement ------------------------------//
    if(entity.inputLeft)entity.xvel -= accel*elapsed;
    if(entity.inputRight)entity.xvel += accel*elapsed;
//------------------------------- directional movement ------------------------------//

//------------------------------ Friction --------------------------------------------//    
    float acc = entity.grounded ? accel : air_accel;
    if (entity.inputLeft) entity.xvel -= acc*elapsed;
    if (entity.inputRight) entity.xvel += acc*elapsed;
    if(entity.grounded&&(entity.inputLeft==entity.inputRight)){
        float decel = elapsed*friction;
        if(std::abs(entity.xvel)<=decel)entity.xvel = 0.f;
        else entity.xvel -= std::copysign(decel, entity.xvel); 
    }
    entity.xvel = std::clamp(entity.xvel, -entity_velocity_cap, entity_velocity_cap);
//------------------------------ Friction --------------------------------------------//

//----------------------- jumping logic ---------------------------//
    if(entity.jumped && entity.grounded){
        entity.yvel = -jump_speed;
        entity.grounded = false;
        sounds::jumpSound.play();
    }
    entity.jumped = false;
//----------------------- jumping logic ---------------------------//

//-------------------------------------------------------- gravity ------------------------------------------------//
    float g = (entity.holdingJump && entity.yvel<0) ? gravity_held : gravity;//different gravity depending on holding jump
    entity.yvel += g*elapsed;
    entity.yvel = std::clamp(entity.yvel, -jump_speed, max_fall_speed);
//-------------------------------------------------------- gravity ------------------------------------------------//

//--------------------------------------- Check touched tiles X Axis ------------------------------//
    //Compute edges of player rectangle
    entity.pos.x += entity.xvel*elapsed;

    float left   = entity.pos.x - hitboxSize.x / 2.f;
    float right  = entity.pos.x + hitboxSize.x / 2.f - delta;
    float top    = entity.pos.y - hitboxSize.y / 2.f;
    float bottom = entity.pos.y + hitboxSize.y / 2.f - delta;
    //Convert the edges to the adjecent tiles
    int leftTile = static_cast<int>(std::floor(left/16.f));
    int rightTile = static_cast<int>(std::floor(right/16.f));
    int wallTopTile    = static_cast<int>(std::floor((top    + wall_margin)/16.f));
    int wallBottomTile = static_cast<int>(std::floor((bottom - wall_margin)/16.f));

    //Going to the right
    if(entity.xvel > 0){
        for(int ty = wallTopTile; ty <= wallBottomTile; ty++){
            if(level.isSolid(rightTile , ty)){
                entity.xvel = 0;
                entity.pos.x = (rightTile)*16.f-hitboxSize.x/2;
            }
        }
    }
    //Going to the left
    else if(entity.xvel < 0){
        for(int ty = wallTopTile; ty <= wallBottomTile; ty++){
            if(level.isSolid(leftTile, ty)){
                entity.xvel = 0;
                entity.pos.x = (leftTile+1)*16.f+hitboxSize.x/2;
            }
        }
    }
//--------------------------------------- Check touched tiles X Axis ------------------------------//

    
    
//---------------------------------------------------- Check touched tiles Y Axis ----------------------------------------------//
    //Recompute edges of player rectangle since it was moved
    entity.pos.y += entity.yvel*elapsed;

    left   = entity.pos.x - hitboxSize.x / 2.f;
    right  = entity.pos.x + hitboxSize.x / 2.f - delta;
    top    = entity.pos.y - hitboxSize.y / 2.f;
    bottom = entity.pos.y + hitboxSize.y / 2.f - delta;

    leftTile = static_cast<int>(std::floor(left/16.f));
    rightTile = static_cast<int>(std::floor(right/16.f));
    float topTile = static_cast<int>(std::floor(top/16.f));
    float bottomTile = static_cast<int>(std::floor(bottom/16.f));
    
    bool wasGrounded = entity.grounded;
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
//---------------------------------------------------- Check touched tiles Y Axis ----------------------------------------------//


//-------------------------------------- ground slope checks ---------------------------------------//
if(entity.yvel>=0){
    float playerBottomY = entity.pos.y + hitboxSize.y/2.f;
    //on which tile is the entity standing
    int bottomTx = std::floor(entity.pos.x/16.f);
    int bottomTy = std::floor((playerBottomY-delta)/16.f);     

    if(!isFloorSlope(level.evaluateType(bottomTx, bottomTy)))bottomTy++;
    TileType t = level.evaluateType(bottomTx,bottomTy);
    if(isFloorSlope(t))
    {
        float slopeY = slopeTileHeight(t , bottomTx, bottomTy, entity.pos.x);
        bool landed = playerBottomY >= slopeY;
        bool stuck = wasGrounded && slopeY - playerBottomY <= slope_snap_distance;
        if(landed || stuck)
        {
            entity.pos.y = slopeY - hitboxSize.y/2.f;
            entity.yvel = 0;
            entity.grounded = true;
        }
    }
}
//-------------------------------------- ground slope checks ---------------------------------------//

//----------------------------------------------------- Ceiling slope check -----------------------------------------------//
    float headY = entity.pos.y - hitboxSize.y/2.f;
    //on which tile is the entity standing
    int topTx = std::floor(entity.pos.x/16.f);
    int topTy = std::floor((headY)/16.f);     

    TileType t = level.evaluateType(topTx,topTy);
    if(isCeilSlope(t))
    {
        float slopeY = slopeTileHeight(t , topTx, topTy, entity.pos.x);
        if(headY < slopeY){ 
            if(entity.yvel<=0 && !entity.grounded)
            {
                entity.pos.y = slopeY + hitboxSize.y/2.f;
                entity.yvel = 0;
            }
        }
    }
//----------------------------------------------------- Ceiling slope check -----------------------------------------------//
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
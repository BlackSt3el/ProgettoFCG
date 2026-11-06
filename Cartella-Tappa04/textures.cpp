#include "textures.h"
//----------------------- Quote --------------------------//
    //Textures, Quote is 14x16 top left corner at 68,122, Polar Star is 9x3 at top left corner at 53,11
    //68, 122
    //244, 187
    //245, 121
    //67, 186
    //The area with the sprites has the top left angle in 68,120 and the size is 176, 66
    //Polar star shots are 16x16 and 128,232 is the left corner of the horizontal one

void quoteSpriteDirection(Entity& player, sf::Sprite& quoteSprite, sf::Sprite& gunSprite){

    if(player.turnBack){
        quoteCycleSprite(7, player.facingRight ? 1 : 0, quoteSprite);
        return;
    }
    switch (player.direction) {
        case Directions::Left :
            if(!player.grounded)quoteCycleSprite(2,0,quoteSprite);
            else quoteCycleSprite(selectAnimationFrame(player, 0, 1, 2), 0, quoteSprite);
            break;
        case Directions::Right :
            if(!player.grounded)quoteCycleSprite(2,1,quoteSprite);    
            else quoteCycleSprite(selectAnimationFrame(player, 0, 1, 2), 1, quoteSprite); 
            break;
        case Directions::UpRight :
            if(!player.grounded)quoteCycleSprite(4,1,quoteSprite);
            else quoteCycleSprite(selectAnimationFrame(player, 3, 4, 5), 1, quoteSprite);
            break;
        case Directions::DownRight :
            quoteCycleSprite(6, 1, quoteSprite);
            break;
        case Directions::UpLeft :
            if(!player.grounded)quoteCycleSprite(4,0,quoteSprite);
            else quoteCycleSprite(selectAnimationFrame(player, 3, 4, 5), 0, quoteSprite);
            break;
        case Directions::DownLeft :
            quoteCycleSprite(6, 0, quoteSprite);
            break;

        default:
            break;
    }
}

void quoteCycleSprite(int XIndex, int YIndex, sf::Sprite& quoteSprite){
    quoteSprite.setTextureRect(sf::IntRect({XIndex*SPRITE_WIDTH+QUOTE_ORIGIN.x,YIndex*SPRITE_HEIGHT+QUOTE_ORIGIN.y}, {SPRITE_WIDTH, SPRITE_HEIGHT}));
}


void gunUpdateSprite(Entity& player){
    const GunPosition& currentGun = polarStar[static_cast<int>(player.direction) ];
    player.gunSprite.setTextureRect(currentGun.rect);
    player.gunSprite.setOrigin({currentGun.rect.size.x/2.f, currentGun.rect.size.y/2.f});

    float walkingOffset = (player.grounded&&(player.animFrame == 1 || player.animFrame == 3)) ? -1.f : 0.f;//Quote goes up and down by 1 pixel during his walk cycle in frames 1 and 3

    player.gunSprite.setPosition({
        player.pos.x + currentGun.offset.x, 
        player.pos.y + (currentGun.offset.y+walkingOffset)
    });
}

void quoteUpdateAnimation(Entity& e, float elapsed){
    bool walking = e.grounded && (e.inputLeft||e.inputRight);
    if (walking){
        e.animTimer+= elapsed;
        while(e.animTimer >= FrameTime){
            e.animTimer -= FrameTime;
            e.animFrame = (e.animFrame+1) % 4;    
            sounds::walkSound.play();
        }
    } 
    else{
        e.animTimer = 0.f;
        e.animFrame = 0;
    }
}


//this means the cycle will be Idle, walk1, Idle, Walk2, 4 frame walk cycle
int selectAnimationFrame(Entity& e, int idle, int walk1, int walk2){
    switch(e.animFrame){
        case 1 : return walk1;
        case 3: return walk2;
        default: return idle;
    }
    return idle;
} 

//----------------------- Quote --------------------------//
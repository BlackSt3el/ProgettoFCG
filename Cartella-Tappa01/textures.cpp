#include "textures.h"

//----------------------- Quote --------------------------//
    //Textures, Quote is 16x18, 
    //68, 120
    //244, 187
    //245, 121
    //67, 186
    //The area with the sprites has the top left angle in 68,120 and the size is 176, 66
    //are the 4 angles of the rectangle with all the textures

void quoteCycleSprite(int XIndex, int YIndex, sf::Sprite& quoteSpriteSheet){
    quoteSpriteSheet.setTextureRect(sf::IntRect({XIndex*QUOTE_WIDTH+QUOTE_ORIGIN.x,YIndex*QUOTE_HEIGHT+QUOTE_ORIGIN.y}, {QUOTE_WIDTH, QUOTE_HEIGHT}));
}

//----------------------- Quote --------------------------//
#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "entity.h"

void quoteCycleSprite(int XIndex, int YIndex, sf::Sprite& quoteSprite);
void quoteSpriteDirection(Entity& player, sf::Sprite& quoteSprite,  sf::Sprite& gunSprite);
void gunUpdateSprite(Entity& gunSprite);
void quoteUpdateAnimation(Entity& e, float elapsed);
int selectAnimationFrame(Entity& e, int idle, int walk1, int walk2);




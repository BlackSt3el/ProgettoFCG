#include "sound.h"

namespace {
    sf::SoundBuffer jumpBuffer("./../Cartella-risorse/Cave Story - Player/ID0f_snd_quote_jump.wav");
    sf::SoundBuffer walkBuffer("./../Cartella-risorse/Cave Story - Player/ID18_snd_quote_walk.wav");
    sf::SoundBuffer PSBuffer("./../Cartella-risorse/Cave Story - Weapons/ID31_snd_polar_star_l3.wav");
}

namespace sounds{
    sf::Sound jumpSound(jumpBuffer);
    sf::Sound walkSound(walkBuffer);
    sf::Sound PSSound(PSBuffer);
}

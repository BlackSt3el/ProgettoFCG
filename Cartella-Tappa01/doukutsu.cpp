#include "doukutsu.h"

const char* window_title = "DoukutsuClone";

int XIndex = 0;
int YIndex = 0;
int frameCounter = 0;


//------------------------------------ Window/View ------------------------------------//

void update_view(sf::RenderWindow& window, sf::Vector2u size){

    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height);
    float new_aspect = static_cast<float>(size.x)/static_cast<float>(size.y);

    sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});

    if (new_aspect > aspect)
    {
        viewport.size.x = aspect/new_aspect;
        viewport.position.x = (1.f - viewport.size.x)/2.f;
    }
    else
    {
        viewport.size.y = new_aspect/aspect;
        viewport.position.y = (1.f - viewport.size.y)/2.f;
    }
    sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(window_width), static_cast<float>(window_height)}));
    view.setViewport(viewport);
    window.setView(view);
}

void toggle_fullscreen(sf::RenderWindow& window, bool fullscreen, sf::ContextSettings settings){
    if(fullscreen){
        window.create(sf::VideoMode::getDesktopMode(), window_title, sf::Style::None, sf::State::Fullscreen, settings);
        window.setFramerateLimit (max_frame_rate);    
    }
    else{
        sf::VideoMode Desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode ({window_width, window_height}), window_title, sf::Style::Default, sf::State::Windowed, settings);
        window.setFramerateLimit (max_frame_rate);
        window.setPosition({static_cast<int>(Desktop.size.x)/2 - window_width/2, static_cast<int>(Desktop.size.y)/2 - window_height/2});
    }
    window.setKeyRepeatEnabled(false);
    update_view(window, window.getSize());
}

//------------------------------------ Window/View ------------------------------------//

//------------------------------------ State ------------------------------------//

struct State
{
    sf::Vector2i mouse_pos;
    bool paused = false;
    bool fullscreen = false;

    sf::ContextSettings settings;

    sf::Texture playerTexture;

    sf::Sprite playerSprite{playerTexture};

    Entity player;
    Entity entity;


    Entity entityBuff[2] = {player,entity};

    State(){
//------------------------------------ Textures ------------------------------------//
        if(!playerTexture.loadFromFile("./../Cartella-risorse/DoukutsuCharSprites.png")){
            std::cout << "Image Failed to load" << std::endl;
        }
        else{
            std::cout << "Image Loaded" << std::endl;
            playerSprite.setTexture(playerTexture, true);
            quoteCycleSprite(XIndex, YIndex, playerSprite);
            auto r = playerSprite.getTextureRect();
            playerSprite.setOrigin({r.size.x / 2.f, r.size.y / 2.f});
        }   
//------------------------------------ Textures ------------------------------------//
        settings.antiAliasingLevel = 8;
    }


    void update(float elapsed);
    void drawFrame(sf::RenderWindow& window);
    void restart();
};

void State::update(float elapsed){ 
    if(paused) return;
    
    moveVelCheck(player, elapsed);

    boundaryCheck(player, rectangleSize.y, rectangleSize.x);

    player.rectangle.setPosition(player.pos);

    if(frameCounter == 59){
        quoteCycleSprite(XIndex, YIndex, playerSprite);
        XIndex++;
        if(XIndex == 11)YIndex = (YIndex+1)%3;
        XIndex %= 11;
    }
    playerSprite.setPosition(player.rectangle.getPosition());
}

void State::drawFrame(sf::RenderWindow& window){
    window.draw(player.rectangle);
    window.draw(playerSprite);
    frameCounter = (frameCounter + 1)%60;
};

//------------------------------------ State------------------------------------//

//------------------------------------ Events------------------------------------//

template <typename T> void handle (T& event, State& state, sf::RenderWindow& window){}

void handle_close (sf::RenderWindow& window){
    window.close();
};

void handle_resize (const sf::Event::Resized& resized, sf::RenderWindow& window){   // constrain aspect ratio and map always the same portion of the world

    update_view(window, resized.size);
}

void handle (const sf::Event::KeyPressed& key, State& state, sf::RenderWindow& window){
    switch (key.scancode){

        //Settings hotkeys
        case sf::Keyboard::Scancode::F11 :
            state.fullscreen=!state.fullscreen;
            toggle_fullscreen(window, state.fullscreen, state.settings);
            break;
        case sf::Keyboard::Scancode::Space :
            //state.paused = !state.paused;
            break;

        //Holding direction
        case sf::Keyboard::Scancode::Left :
        case sf::Keyboard::Scancode::A :
            state.player.inputLeft = true;
            state.player.faceLeft = true; state.player.faceRight = false;
            break;
        case sf::Keyboard::Scancode::D :
        case sf::Keyboard::Scancode::Right :
            state.player.inputRight = true;
            state.player.faceRight = true; state.player.faceLeft = false;
            break;
        case sf::Keyboard::Scancode::S :
        case sf::Keyboard::Scancode::Down :
            state.player.inputDown = true;
            state.player.faceDown = true; state.player.faceUp = false;  
            break;
        case sf::Keyboard::Scancode::W :
        case sf::Keyboard::Scancode::Up :
            state.player.inputUp = true;
            state.player.faceUp = true; state.player.faceDown = false;  
            break;

        //Action buttons
        case sf::Keyboard::Scancode::J ://Shoot
            break;
        case sf::Keyboard::Scancode::K ://Jump
            state.player.jumped = true;
            state.player.holdingJump = true;
            break;
        case sf::Keyboard::Scancode::L ://Special
            break;
        case sf::Keyboard::Scancode::Semicolon ://Special 2
            break;
        
        
        //Default
        default:
            break;
    
    
    }
}

void handle (const sf::Event::KeyReleased Key, State& state, sf::RenderWindow& window){
    
    //Stopped holding direction
    switch (Key.scancode)
    {
        //Movement
        case sf::Keyboard::Scancode::Left :
        case sf::Keyboard::Scancode::A :
            state.player.inputLeft = false;
            break;
        case sf::Keyboard::Scancode::D :
        case sf::Keyboard::Scancode::Right :
            state.player.inputRight = false;    
            break;
        case sf::Keyboard::Scancode::S :
        case sf::Keyboard::Scancode::Down :
            state.player.inputDown = false; 
            state.player.faceDown = false; 
            break;
        case sf::Keyboard::Scancode::W :
        case sf::Keyboard::Scancode::Up :
            state.player.inputUp = false;    
            break;

        //Action Buttons

        case sf::Keyboard::Scancode::K ://Jump
            state.player.holdingJump = false;
            break;
        //Default
        default:
            break;
    }
    //Stopped holding direction
}


void handle (const sf::Event::FocusLost Focus, State& state, sf::RenderWindow& window){
    state.paused = true;
}


//------------------------------------ Events ------------------------------------//

//------------------------------------ Main ------------------------------------//

int main(){

    float elapsed;
    sf::Clock clock;

    State state;
    
    sf::RenderWindow window;
    
    toggle_fullscreen(window, state.fullscreen,state.settings);

    state.player.rectangle.setOrigin({rectangleSize.x/2,rectangleSize.y/2});
    state.player.rectangle.setFillColor(sf::Color::Blue);

    //Loop
    while (window.isOpen())
    {
        // handle events
        window.handleEvents (
            [&window](const sf::Event::Closed&) { handle_close (window); },
            [&window](const sf::Event::Resized& event) { handle_resize (event, window); },
            [&state, &window](const auto& event) { handle (event, state, window); }//utilizza il polimorfismo per scegliere la handle giusta a seconda del tipo
        );
        // handle events

        elapsed = clock.restart().asSeconds();
        window.clear (sf::Color::White);
        state.update(elapsed);
        state.drawFrame(window);
        window.display ();
    }
}

//------------------------------------ Main ------------------------------------//

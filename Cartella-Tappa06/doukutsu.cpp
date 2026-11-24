#include "doukutsu.h"

const char* window_title = "DoukutsuClone";

int QuoteXIndex = 0;
int QuoteYIndex = 0;
int frameCounter = 0;

//------------------------------------ Window/View ------------------------------------//

void update_view(sf::RenderWindow& window, sf::Vector2u newSize){

    float scale = std::min(newSize.x/viewSize.x, newSize.y/viewSize.y); //By how much can we multiply the current level view to still fit the window () 
    if(scale>=1.f)scale = std::floor(scale);

    //use scale to actually multiply the current size
    float newViewWidth = viewSize.x * scale;
    float newViewHeight = viewSize.y *scale;
    //size of black bars, size of the window - size of the view
    float barX = (newSize.x - newViewWidth) /2.f; 
    float barY = (newSize.y - newViewHeight) /2.f;
    
    sf::View view(sf::FloatRect({0.f, 0.f}, viewSize));
    view.setViewport(sf::FloatRect({ barX/newSize.x , barY/newSize.y }, {newViewWidth/newSize.x, newViewHeight/newSize.y}));
    window.setView(view);
}

void toggle_fullscreen(sf::RenderWindow& window, bool fullscreen, sf::ContextSettings settings){
    if(fullscreen){
        window.create(sf::VideoMode::getDesktopMode(), window_title, sf::Style::None, sf::State::Fullscreen, settings);
        window.setFramerateLimit (max_frame_rate);    
    }
    else{
        sf::VideoMode Desktop = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode ({sight_width, sight_height}), window_title, sf::Style::Default, sf::State::Windowed, settings);
        window.setFramerateLimit (max_frame_rate);
        window.setPosition({static_cast<int>(Desktop.size.x)/2 - sight_width/2, static_cast<int>(Desktop.size.y)/2 - sight_height/2});
    }
    window.setKeyRepeatEnabled(false);
    update_view(window, window.getSize());
}

//resets the view to be centered on the player character
void updateCamera(sf::RenderWindow& window, Entity& player){
return;
}

//------------------------------------ Window/View ------------------------------------//

//------------------------------------ State ------------------------------------//

struct State
{
    sf::Vector2i mouse_pos;
    bool paused = false;
    bool fullscreen = false;

    sf::ContextSettings settings;

    Entity player;

    sf::Texture playerTexture;

    std::vector<Bullet> bullets;

    Level level;

    State(sf::Texture& playerSheet)
        : player(playerTexture)
    {
        player.hitbox.setOrigin({hitboxSize.x/2,hitboxSize.y/2});
        player.hitbox.setFillColor(sf::Color::Blue);
        playerTexture = playerSheet;
        settings.antiAliasingLevel = 8;  
        player.pos = levelSize/2.f;
        player.hitbox.setPosition(player.pos);
    }

    void updatePlayer(float elapsed);
    void update(float elapsed, sf::RenderWindow& window);
    void drawFrame(sf::RenderWindow& window);
    void restart();
};

void State::updatePlayer(float elapsed){
    updateEntity(player, elapsed, level);

    quoteSpriteDirection(player,player.charSprite, player.gunSprite);

    quoteUpdateAnimation(player, elapsed);
    
    player.charSprite.setPosition({std::round(player.pos.x),std::round(player.pos.y)});
    gunUpdateSprite(player);
}

void State::update(float elapsed, sf::RenderWindow& window){ 
    if(paused) {
        return;
    }
    updatePlayer(elapsed);
    updateBullets(player, bullets, elapsed);
    updateCamera(window, player);
}

void State::drawFrame(sf::RenderWindow& window){ 
    //going for the background
    window.draw(level.decorationLevelSprites);
    window.draw(level.solidLevelSprites);
    //going for the background
    window.draw(player.hitbox);
    window.draw(player.gunSprite);
    window.draw(player.charSprite);
    drawBullets(window, bullets, playerTexture);
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

//Pressing a key
void handle (const sf::Event::KeyPressed& key, State& state, sf::RenderWindow& window){
    sf::View view;
    switch (key.scancode){

        //Settings hotkeys
        case sf::Keyboard::Scancode::F11 :
            state.fullscreen=!state.fullscreen;
            toggle_fullscreen(window, state.fullscreen, state.settings);
            break;
        case sf::Keyboard::Scancode::Space :
            state.paused = !state.paused;
            break;

        //Holding direction
        case sf::Keyboard::Scancode::Left :
        case sf::Keyboard::Scancode::A :
            state.player.inputLeft = true;
            state.player.facingRight = false;
            break;
        case sf::Keyboard::Scancode::D :
        case sf::Keyboard::Scancode::Right :
            state.player.inputRight = true;
            state.player.facingRight = true;
            break;
        case sf::Keyboard::Scancode::S :
        case sf::Keyboard::Scancode::Down :
            state.player.inputDown = true;
            break;
        case sf::Keyboard::Scancode::W :
        case sf::Keyboard::Scancode::Up :
            state.player.inputUp = true;  
            break;

        //Action buttons
        case sf::Keyboard::Scancode::J ://Shoot
            state.player.shooting = true;
            break;
        case sf::Keyboard::Scancode::K ://Jump
            state.player.jumped = true;
            state.player.holdingJump = true;
            break;
        case sf::Keyboard::Scancode::L ://Special
            state.player.special1 = true;
            break;
        case sf::Keyboard::Scancode::Semicolon ://Special 2
            state.player.special2 = true;
            break;
        
        //DELETE THIS, TRYING TO MOVE VIEWPORT WORLD AND WINDOW
        case sf::Keyboard::Scancode::U : 
            view = window.getView();
            view.setCenter({view.getCenter().x+1,view.getCenter().y+1});
            window.setView(view);
            break;
            
        default:
            break;
    }
}


//Releasing a key
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
            break;
        case sf::Keyboard::Scancode::W :
        case sf::Keyboard::Scancode::Up :
            state.player.inputUp = false;    
            break;

        //Action Buttons
        case sf::Keyboard::Scancode::J ://Jump
            state.player.shooting = false;
            break;
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

    //Load Textures
    sf::Texture playerTexture;
    if(!playerTexture.loadFromFile(quoteTexturePath)){
        std::cout << "Image Failed to load" << std::endl;
    }

    //set sound volume
    sounds::jumpSound.setVolume(20.f);
    sounds::walkSound.setVolume(20.f);
    sounds::PSSound.setVolume(5.f);

    //------------------------------------ background try ------------------------------------//
    // define the level with an array of tile indices
    std::vector<int> levelDeco, levelSolid;
    TileMap mapDeco, mapSolid;
    Level level;
    unsigned width, height;
    if(!loadCsvLevel("./../Cartella-risorse/Cave Story TileMaps/FirstCave_DecorationLayer.csv", levelDeco, width, height))
        return-1;
    level.decorationLevelIDs = levelDeco;
    if(!loadCsvLevel("./../Cartella-risorse/Cave Story TileMaps/FirstCave_SolidBlocksLayer.csv", levelSolid, width, height))
        return-1;
    level.solidLevelIDs = levelSolid;
    // create the tilemap from the level definition
    
    if (!mapDeco.load("./../Cartella-risorse/Cave Story - Backgrounds - First Cave.png", {16, 16}, levelDeco.data(), width, height))
        return -1;
    level.decorationLevelSprites = mapDeco;
    if (!mapSolid.load("./../Cartella-risorse/Cave Story - Backgrounds - First Cave.png", {16, 16}, levelSolid.data(), width, height))
        return -1;
    level.solidLevelSprites = mapSolid;
    level.width = width;
    level.height = height;
    levelSize = {width*16.f, height*16.f};
    //------------------------------------ background try ------------------------------------//

    State state(playerTexture);
    state.level = level;

    sf::RenderWindow window;

   
    
    toggle_fullscreen(window, state.fullscreen,state.settings);

    clock.restart();

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
        
        elapsed = std::min(clock.restart().asSeconds(),1/60.f);//prevents the game logic from advancing without updating during long loading times or lag
        window.clear (sf::Color::Black);
        state.update(elapsed, window);
        state.drawFrame(window);
        window.display();
        frameCounter = (frameCounter + 1)%max_frame_rate;
    }
}

//------------------------------------ Main ------------------------------------//

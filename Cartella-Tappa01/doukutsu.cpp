#include <SFML/Graphics.hpp>
#include "../Cartella-risorse/textures.hpp"
#include <iostream>
#include <cmath>
//////////////////////
// Initial defaults //
//////////////////////

// window
const char* window_title = "DoukutsuClone";
const int window_width = 800;
const int window_height = 600;
const float mult = 1;//increase size of objects and mantain ratio
const float max_frame_rate = 60;

/////////////
/// STATE ///
/////////////

struct State
{
    sf::Vector2i mouse_pos;
    bool pause = true;
    
    State(){};//constructor

    void update(float elapsed);
    void draw(sf::RenderWindow& window);
    void restart();
};

void State::draw(sf::RenderWindow& window){
    
};



////////////
// Events //
////////////

template <typename T> void handle (T& event, State& state){}

void handle_close (sf::RenderWindow& window){
    window.close();
};

void handle_resize (const sf::Event::Resized& resized, sf::RenderWindow& window){   // constrain aspect ratio and map always the same portion of the world
    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height);
    float new_aspect = static_cast<float>(resized.size.x)/static_cast<float>(resized.size.y);

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
};

void handle (const sf::Event::KeyPressed& key, State& state){
    switch (key.scancode)
    {
    case sf::Keyboard::Scancode::Space :
        state.pause = !state.pause;
        break;
    case sf::Keyboard::Scancode::Left :
        break;
    case sf::Keyboard::Scancode::Right :
        break;
    default:
        break;
    }
}

void handle (const sf::Event::FocusLost Focus, State& state){
    state.pause = true;
}

void handle (const sf::Event::KeyReleased Key, State& state){
}

//////////
// Loop //
//////////

int main(){
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title, sf::Style::Default);
    
    sf::VideoMode Desktop = sf::VideoMode::getDesktopMode();
    
    window.setFramerateLimit (max_frame_rate);
    window.setMinimumSize(window.getSize());
    window.setPosition({static_cast<int>(Desktop.size.x)/2 - window_width/2, static_cast<int>(Desktop.size.y)/2 - window_height/2});
    State state;

    //Loop
    while (window.isOpen())
    {
        // events
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); },
                             [&state](const auto& event) { handle (event, state); }//utilizza il polimorfismo per scegliere la handle giusta a seconda del tipo
        );
        window.clear (sf::Color::Black);
        //state.draw(window);
        window.display ();
    }
}

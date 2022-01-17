#include "game.h"

// Game functions

// Private functions
void Game::start(){
    // Initialize SFML
    window.create(sf::VideoMode(1280, 720), "Space Game");
    window.setFramerateLimit(120);
    
    hudCamera.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), (sf::Vector2f)window.getSize()));
    worldCamera.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), (sf::Vector2f)window.getSize()));

    // Initialize static classes
    Interface::Renderer::init(&window);
    Debug::init(&window);

    // Initialize game variables
    parallaxEffect.create(window.getSize().x, window.getSize().y, 10.f);
    planet = new Objects::Planet(engine, 0, 0, "./res/textures/planet.png");
    player.create(engine, 600, 100);
    ship = new Vehicles::Ship(engine, 900, 300);
}

void Game::event(){
    switch(mEvent.type){
    case sf::Event::Closed:
        window.close();
        break;

    case sf::Event::MouseWheelScrolled:
        worldCamera.zoom(mEvent.mouseWheelScroll.delta * -0.1f + 1.f);
        break;

    case sf::Event::KeyPressed:
        if(mEvent.key.code == sf::Keyboard::R){
            player.drive(player.isDriving() ? NULL : ship);
        }
        break;

    default:
        break;
    }
}

void Game::frame(){
    // Physics
    engine.update(deltaTime);
    player.update(deltaTime);
    ship->update(deltaTime);
    parallaxEffect.setCameraPosition(player.getPosition());

    // HUD rendering
    window.setView(hudCamera);
    window.draw(parallaxEffect);

    // World rendering
    worldCamera.setCenter(player.getPosition());
    window.setView(worldCamera);
    window.draw(*planet);
    window.draw(player);
    window.draw(*ship);
    window.draw(engine);
    planet->update(deltaTime);
}

void Game::end(){
    // Cleanup
    delete planet;
    delete ship;
}

// Constructors
Game::Game(){}
Game::~Game(){}

void Game::run(){
    // Starts the application
    start();

    while(window.isOpen()){
        deltaTime = deltaClock.restart().asSeconds();

        while(window.pollEvent(mEvent)){
            event();
        }

        window.clear();
        frame();
        window.display();
    }

    end();
}

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
    parallaxEffect.create(window.getSize().x, window.getSize().y, 64.f);
    planet = new Objects::Planet(engine, 0, 32 * 256 + 1000, 32.f, 256);
    player.create(engine, 100, -100);
    ship = new Vehicles::Ship(engine, 0, -100);

    Phys::RigidBody* r = Phys::RigidBody::createBoxRigidBody(-500, -100, 50, 50);
    //r->velocity.x = 500.f;
    r->mass = 10.f;
    engine.registerBody(r);
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
    // engine.update(deltaTime);
    player.update(deltaTime);
    ship->update(deltaTime);
    parallaxEffect.setCameraPosition(player.getPosition());

    // Player gravity
    sf::Vector2f planetToPlayer = Math::normalize(player.getPosition() - planet->getCenter());
    float plyRot = std::atan2(planetToPlayer.y, planetToPlayer.x) * (180 / 3.1415) + 90;
    worldCamera.setRotation(plyRot);
    player.getRigidBody()->force += planetToPlayer * -450.f * player.getRigidBody()->mass;
    
    sf::Vector2f planetToShip = Math::normalize(ship->getPosition() - planet->getCenter());
    ship->getRigidBody()->force += planetToShip * -450.f * ship->getRigidBody()->mass;

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        planet->setPixel(pos.x, pos.y, sf::Color::Red);
    } else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        planet->setPixel(pos.x, pos.y, sf::Color(0, 0, 0, 0));
    }

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
    engine.update(deltaTime);
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

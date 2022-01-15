#include <SFML/Graphics.hpp>

#include "phys/engine.h"
#include "util/debugger.h"

int main(int argc, char** argv){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Space Game");
    window.setFramerateLimit(120);
    
    Debug::init(&window);
    sf::Clock deltaClock;
    float deltaTime;
    std::string debugText;
    
    Phys::Engine engine;
    int c1 = engine.registerCollider(new Phys::BoxRigidBody(320, 250, 100, 50));
    int c2 = engine.registerCollider(new Phys::BoxRigidBody(450, 250, 50, 100));
    engine.getRigidBody(c2)->setSpringForce(0.f);

    while(window.isOpen()){
        // Get delta time
        deltaTime = deltaClock.restart().asSeconds();
        debugText  = "DeltaTime: " + std::to_string(deltaTime);
        debugText += "\nFPS: " + std::to_string((int)(1.f / deltaTime));

        // Handle events
        sf::Event e;
        while(window.pollEvent(e)){
            switch(e.type){
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            }
        }

        // Controls
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ engine.getRigidBody(c1)->acceleration.x = -10 * deltaTime; }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ engine.getRigidBody(c1)->acceleration.x = 10 * deltaTime; }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ engine.getRigidBody(c1)->acceleration.y = -10 * deltaTime; }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ engine.getRigidBody(c1)->acceleration.y = 10 * deltaTime; }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){ engine.getRigidBody(c1)->rotAcceleration = 5 * deltaTime; }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){ engine.getRigidBody(c1)->rotAcceleration = -5 * deltaTime; }

        // Physics
        window.clear();
        engine.update(deltaTime);

        // Rendering
        window.draw(engine);
        Debug::drawText(10, 10, debugText, 18u, sf::Color::Yellow);
        window.display();
    }

    return 0;
}
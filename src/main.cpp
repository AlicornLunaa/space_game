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
    int c1 = engine.registerCollider(new Phys::BoxCollider(320, 250, 100, 50));
    int c2 = engine.registerCollider(new Phys::BoxCollider(450, 250, 50, 100));

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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ engine.getCollider(c1)->move(-100 * deltaTime, 0); }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ engine.getCollider(c1)->move(100 * deltaTime, 0); }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ engine.getCollider(c1)->move(0, -100 * deltaTime); }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ engine.getCollider(c1)->move(0, 100 * deltaTime); }

        // Physics
        engine.update(deltaTime);

        // Rendering
        window.clear();
        window.draw(engine);
        Debug::drawText(10, 10, debugText, 18u, sf::Color::Yellow);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include "objects/ship.h"
#include "util/debugger.h"

int main(int argc, char** argv){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Space Game");
    window.setFramerateLimit(120);
    
    Debug::init(&window);
    sf::Clock deltaClock;
    float deltaTime;
    
    std::string debugText;

    Objects::Ship playerShip(10, 25, 4);
    playerShip.setPosition(500, 250);

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

        // Physics
        playerShip.update(deltaTime);

        // Rendering
        window.clear();
        window.draw(playerShip);
        Debug::drawText(10, 10, debugText, 16u, sf::Color::Yellow);
        Debug::drawLine(10, 10, 500, 500, sf::Color::Green);
        window.display();
    }

    return 0;
}
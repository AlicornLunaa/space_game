#include <SFML/Graphics.hpp>
#include "objects/ship.h"

int main(int argc, char** argv){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Space Game");
    window.setFramerateLimit(120);

    sf::Clock deltaClock;
    float deltaTime;

    sf::Font debugFont;
    sf::Text debugText;
    debugFont.loadFromFile("./res/fonts/arial.ttf");
    debugText.setFont(debugFont);
    debugText.setPosition(10, 10);

    Objects::Ship playerShip(10, 25, 4);

    while(window.isOpen()){
        // Get delta time
        deltaTime = deltaClock.restart().asSeconds();
        debugText.setString("DeltaTime: " + std::to_string(deltaTime));
        debugText.setString("FPS: " + std::to_string((int)(1.f / deltaTime)));

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
        window.display();
    }

    return 0;
}
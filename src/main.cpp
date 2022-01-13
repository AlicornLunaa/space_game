#include <SFML/Graphics.hpp>

#include "objects/player.h"
#include "objects/vehicle.h"
#include "util/debugger.h"

int main(int argc, char** argv){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Space Game");
    window.setFramerateLimit(120);
    
    Debug::init(&window);
    sf::Clock deltaClock;
    float deltaTime;
    
    std::string debugText;

    Objects::Player player(1280/2, 720/2);
    Vehicles::Ship shipVehicle(10, 25, 4);
    shipVehicle.setPosition(500, 250);

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

            case sf::Event::KeyPressed:
                if(e.key.code == sf::Keyboard::R){
                    if(player.isDriving()){
                        player.drive(nullptr);
                    } else {
                        player.drive(&shipVehicle);
                    }
                }
                break;

            default:
                break;
            }
        }

        // Physics
        player.update(deltaTime);

        // Rendering
        window.clear();
        player.render(&window);
        Debug::drawText(10, 10, debugText, 18u, sf::Color::Yellow);
        Debug::drawRect(42, 68, 20, 20, sf::Color::Red, false);
        window.display();
    }

    return 0;
}
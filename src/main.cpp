#include <SFML/Graphics.hpp>

#include "objects/parallax.h"
#include "objects/player.h"
#include "objects/vehicle.h"
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
    Phys::RigidBody rb(700, 300, 0); rb.addPoint(0, -1); rb.addPoint(-1, 1); rb.addPoint(1, 1); rb.scale(30, 40);
    engine.registerCollider(&rb);
    int c2 = engine.registerCollider(new Phys::BoxRigidBody(320, 600, 100, 100));
    int c3 = engine.registerCollider(new Phys::BoxRigidBody(460, 600, 100, 100));
    engine.registerCollider(new Phys::BoxRigidBody(450, 250, 500, 100));
    engine.getRigidBody(c2)->setSpringForce(0.f);
    engine.getRigidBody(c3)->setSpringForce(0.f);

    Objects::Parallax parallaxEffect(window.getSize().x, window.getSize().y, 8.f);
    Objects::Player player(engine, 700, 300);
    Vehicles::Ship ship(engine, 900, 300);

    sf::View hudCamera(sf::FloatRect(sf::Vector2f(0, 0), (sf::Vector2f)window.getSize()));
    sf::View worldCamera(player.getPosition(), (sf::Vector2f)window.getSize());

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

            case sf::Event::MouseWheelScrolled:
                worldCamera.zoom(e.mouseWheelScroll.delta * -0.1f + 1.f);
                break;

            case sf::Event::KeyPressed:
                if(e.key.code == sf::Keyboard::R){
                    player.drive(player.isDriving() ? NULL : &ship);
                }
                break;

            default:
                break;
            }
        }

        // Physics
        engine.update(deltaTime);
        player.update(deltaTime);
        ship.update(deltaTime);
        parallaxEffect.setCameraPosition(player.getPosition());

        // Hud drawing
        window.clear();
        window.setView(hudCamera);
        window.draw(parallaxEffect);
        Debug::drawText(10, 10, debugText, 18u, sf::Color::Yellow);

        // World drawing
        window.setView(worldCamera);
        worldCamera.setCenter(player.getPosition());
        window.draw(player);
        window.draw(ship);
        window.draw(engine);

        window.display();
    }

    return 0;
}
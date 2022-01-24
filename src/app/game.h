/**
 * @file game.h
 * @author Luna
 * @brief Contains the game data
 */
#pragma once
#include <SFML/Graphics.hpp>
#include "planetManager.h"
#include "../interface/ui.h"
#include "../phys/physics.h"
#include "../objects/objects.h"
#include "../util/debugger.h"

class Game {
private:
    // Rendering variables
    sf::RenderWindow window;
    sf::Event mEvent;
    sf::View hudCamera;
    sf::View worldCamera;

    // Physics variables
    Phys::Engine engine;
    sf::Clock deltaClock;
    float deltaTime;

    // Game variables
    Managers::PlanetManager planetManager;
    Objects::Parallax parallaxEffect;
    Objects::Player player;
    Vehicles::Ship* ship;

    // Functions
    void start();
    void event();
    void frame();
    void end();

public:
    // Constructors
    Game();
    ~Game();

    // Functions
    void run();
};
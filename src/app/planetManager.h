/**
 * @file planetManager.h
 * @author Luna
 * @brief The planet manager class will handle which planets are active and updating as well as the camera parent and gravitational parent
 */
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../objects/planet.h"
#include "../objects/player.h"

using namespace Objects;

namespace Managers {

    class PlanetManager : public sf::Drawable {
    private:
        // Variables
        const float gravitationalConstant = 500.f;
        std::vector<Planet*> planetList;

        sf::RectangleShape effectShape;

        // Functions
        sf::Vector2f gravityImpulse(Planet* p, Phys::RigidBody* rb);
        Planet* findClosestPlanet(sf::Vector2f v);

    public:
        // Constructors
        PlanetManager();
        ~PlanetManager();

        // Functions
        void init(sf::RenderWindow& window);
        void registerPlanet(Planet* p);
        void cleanupPlanets();
        void update(Phys::Engine& engine, float deltaTime);
        void drawEffects(sf::RenderTarget& target, Player& ply);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

};
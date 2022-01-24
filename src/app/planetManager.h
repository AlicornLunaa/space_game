/**
 * @file planetManager.h
 * @author Luna
 * @brief The planet manager class will handle which planets are active and updating as well as the camera parent and gravitational parent
 */
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../objects/planet.h"

using namespace Objects;

namespace Managers {

    class PlanetManager : public sf::Drawable {
    private:
        // Variables
        const float gravitationalConstant = 500.f;
        std::vector<Planet*> planetList;

        // Functions
        sf::Vector2f gravityImpulse(Planet* p, Phys::RigidBody* rb);

    public:
        // Constructors
        PlanetManager();
        ~PlanetManager();

        // Functions
        void registerPlanet(Planet* p);
        void update(Phys::Engine& engine, float deltaTime);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

};
#include "planetManager.h"
using namespace Managers;

// Constructors
PlanetManager::PlanetManager(){}
PlanetManager::~PlanetManager(){}

// Functions
Planet* PlanetManager::findClosestPlanet(sf::Vector2f v){
    // Find the closest planet to the point
    float minDistance = INFINITY;
    unsigned int closest = 0;

    for(unsigned int i = 1; i < planetList.size(); i++){
        float currentDistance = Math::distanceSquare(planetList[i]->getRigidBody()->getPosition(), v);
        
        if(currentDistance < minDistance){
            closest = i;
            minDistance = currentDistance;
        }
    }

    return planetList[closest];
}

void PlanetManager::init(sf::RenderWindow& window){
    // Iniitalize some variables
    effectShape.setSize((sf::Vector2f)window.getSize());
}

void PlanetManager::registerPlanet(Planet* p){ planetList.push_back(p); }

void PlanetManager::cleanupPlanets(){
    for(Planet* p : planetList){
        delete p;
    }
}

sf::Vector2f PlanetManager::gravityImpulse(Planet* p, Phys::RigidBody* rb){
    // Returns the calculation for the impulse for gravity
    Phys::RigidBody* pr = p->getRigidBody();
    sf::Vector2f dir = Math::normalize(pr->getPosition() - rb->getPosition());
    float radius = Math::distanceSquare(rb->getPosition(), pr->getPosition());
    float force = gravitationalConstant * ((pr->mass * rb->mass) / radius);

    return dir * force * rb->mass;
}

void PlanetManager::update(Phys::Engine& engine, float deltaTime){
    // Basic update
    for(Planet* p : planetList){
        p->update(deltaTime);
    }

    // Run each rigidbody
    for(unsigned int i = 0; i < engine.getBodyCount(); i++){
        // Variables needed
        Phys::RigidBody* rb = engine.getRigidBody(i);
        sf::Vector2f impulse;

        // Make sure body is actually rigid
        if(rb == NULL) continue;

        // Run through every planet and calculate new impulses for each object
        for(Planet* p : planetList){
            // Make sure the rigidbody is not the planet
            if(p->getRigidBody() == rb){
                impulse *= 0.f;
                break;
            }

            // Calculate impulse for this planet
            impulse += gravityImpulse(p, rb);
        }

        // Add impulse
        rb->applyImpulse(impulse * deltaTime);
    }
}

void PlanetManager::drawEffects(sf::RenderTarget& target, Player& ply){
    // Draw the HUD effects concerning planets
    /**
     * Drawing the atmosphere by getting the closest planet to the player
     * finding the distance to the surface and adjusting an effect strength
     */
    sf::Vector2f plyPos = ply.getRigidBody()->getPosition();
    Planet* closestPlanet = findClosestPlanet(plyPos);
    float radius = closestPlanet->getSize().x / 2.f * closestPlanet->getScale().x;
    float distanceToSurface = Math::distance(plyPos, closestPlanet->getRigidBody()->getPosition());
    distanceToSurface = distanceToSurface - radius;

    float colorScale = 1.f - std::min(std::max(distanceToSurface / radius * 2.f, 0.04f), 1.f);
    sf::Color renderColor = closestPlanet->getAtmosColor();
    renderColor.a = 255.f * colorScale;
    effectShape.setFillColor(renderColor);

    target.draw(effectShape);
}

void PlanetManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Run through every planet and draw then
    for(Planet* p : planetList){
        target.draw(*p);
    }
}
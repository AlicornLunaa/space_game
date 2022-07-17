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

    return (planetList.size() == 0) ? NULL : planetList[closest];
}

float PlanetManager::getAtmosDepth(Planet* p, sf::Vector2f v){
    // Get a scale for the depth of the atmosphere
    float radius = p->getSize().x / 2.f * p->getScale().x;
    float distanceToSurface = Math::distance(v, p->getRigidBody()->getPosition());
    distanceToSurface = distanceToSurface - radius;
    return (1.f - std::min(std::max(distanceToSurface / radius * 2.f, 0.04f), 1.f));
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

    return dir * force * rb->mass * 0.01f;
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
        float totalDrag = 0.f;

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
            impulse += gravityImpulse(p, rb);;

            // Calculate drag impulse
            totalDrag += getAtmosDepth(p, rb->getPosition());
        }

        // Add impulses
        rb->applyImpulse(impulse * deltaTime);
        rb->velocity *= 1.f - std::max(std::min(totalDrag * 0.02f, 1.f), 0.f);
    }
}

void PlanetManager::drawEffects(sf::RenderTarget& target, Player& ply){
    // Draw the HUD effects concerning planets
    // Error checking
    if(planetList.size() == 0) return;

    /**
     * Drawing the atmosphere by getting the closest planet to the player
     * finding the distance to the surface and adjusting an effect strength
     */
    sf::Vector2f plyPos = ply.getRigidBody()->getPosition();
    Planet* closestPlanet = findClosestPlanet(plyPos);
    float atmosDepth = getAtmosDepth(closestPlanet, plyPos);

    sf::Color renderColor = closestPlanet->getAtmosColor();
    renderColor.a = 255.f * atmosDepth;
    effectShape.setFillColor(renderColor);

    target.draw(effectShape);

    // closestPlanet->drawGround(target, ply);
}

void PlanetManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Run through every planet and draw then
    for(Planet* p : planetList){
        target.draw(*p);
    }
}
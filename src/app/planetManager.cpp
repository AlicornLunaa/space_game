#include "planetManager.h"
using namespace Managers;

// Constructors
PlanetManager::PlanetManager(){}

PlanetManager::~PlanetManager(){
    for(Planet* p : planetList){
        delete p;
    }
}

// Functions
void PlanetManager::registerPlanet(Planet* p){ planetList.push_back(p); }

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

void PlanetManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Run through every planet and draw then
    for(Planet* p : planetList){
        target.draw(*p);
    }
}
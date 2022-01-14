#include "engine.h"
using namespace Phys;

// Constructors
Engine::Engine(){
    // Initialize engine
    renderDebug = true; // Draw debug
}

Engine::~Engine(){}

// Functions
unsigned int Engine::registerCollider(Collider* c){
    colliders.push_back(c);
    return colliders.size() - 1;
}

bool Engine::unregisterCollider(unsigned int id){
    if(id >= colliders.size()) return false;

    colliders.erase(colliders.begin() + id);
    return true;
}

void Engine::collisionDetection(){
    // Check all the collisions
    for(unsigned int i = 0; i < colliders.size(); i++){

    }
}

void Engine::collisionResolution(){
    // Fix all the collisions


    collisions.empty();
}

void Engine::physicsUpdate(){

}

void Engine::update(float deltaTime){
    collisionDetection();
    collisionResolution();
    physicsUpdate();
}

void Engine::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!renderDebug) return;

    for(Collider* c : colliders){
        // Draw lines
        sf::Vector2f lastPoint = c->getPoint(0);

        for(unsigned int i = 1; i < c->getPointCount(); i++){
            sf::Vector2f p = c->getPoint(i);
            Debug::drawLine(lastPoint.x, lastPoint.y, p.x, p.y);
            lastPoint = p;
        }
    }
}
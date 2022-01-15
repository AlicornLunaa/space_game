#include "engine.h"
using namespace Phys;

// Constructors
Engine::Engine(){
    // Initialize engine
    renderDebug = true; // Draw debug
}

Engine::~Engine(){}

// Functions
bool Engine::collidesAABB(Collider* c1, Collider* c2){
    // Get position of each and check if they collide
    sf::Vector2f p1 = c1->getPosition();
    sf::Vector2f s1 = c1->getScale() * 1.5f;
    sf::Vector2f p2 = c2->getPosition();
    sf::Vector2f s2 = c2->getScale() * 1.5f;

    return (p1.x - s1.x <= p2.x + s2.x && p1.x + s1.x >= p2.x - s2.x && p1.y - s1.y <= p2.y + s2.y && p1.y + s1.y >= p2.y - s2.y);
}

bool Engine::collidesSAT(Collider* c1, Collider* c2){
    return true;
}

unsigned int Engine::registerCollider(Collider* c){
    colliders.push_back(c);
    return colliders.size() - 1;
}

bool Engine::unregisterCollider(unsigned int id){
    if(id >= colliders.size()) return false;

    colliders.erase(colliders.begin() + id);
    return true;
}

Collider* Engine::getCollider(unsigned int id){
    return colliders[id];
}

void Engine::collisionDetection(){
    // Check all the collisions
    for(unsigned int i = 0; i < colliders.size(); i++){
        for(unsigned int j = 0; j < colliders.size(); j++){
            // Broadphase
            if(i == j) continue;
            if(!collidesAABB(colliders[i], colliders[j])) continue;

            // Narrow phase
            if(!collidesSAT(colliders[i], colliders[j])) continue;

            // Collision occured, color it
            colliders[i]->setSleeping(false);
            colliders[j]->setSleeping(false);
        }
    }
}

void Engine::collisionResolution(){
    // Fix all the collisions
    for(CollisionData& c : collisions){

    }

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
        sf::Vector2f lastPoint = c->getPointGlobal(0);
        sf::Color col = c->isSleeping() ? sf::Color::Green : sf::Color::Yellow;

        for(unsigned int i = 1; i < c->getPointCount(); i++){
            sf::Vector2f p = c->getPointGlobal(i);
            Debug::drawLine(lastPoint.x, lastPoint.y, p.x, p.y, col);
            lastPoint = p;
        }
        
        Debug::drawLine(lastPoint.x, lastPoint.y, c->getPointGlobal(0).x, c->getPointGlobal(0).y, col);
        c->setSleeping(true);
    }
}
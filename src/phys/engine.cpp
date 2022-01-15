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
    // Define function
    auto check = [](Collider* c1, Collider* c2){
        // Loop through every edge
        for(unsigned int i = 0; i < c1->getPointCount(); i++){
            // Get the edge of the face
            sf::Vector2f edge = (i == c1->getPointCount() - 1) ? (c1->getPointGlobal(0) - c1->getPointGlobal(i)) : (c1->getPointGlobal(i + 1) - c1->getPointGlobal(i));
            sf::Vector2f normal = Math::perpendicular(Math::normalize(edge));

            // Edge data
            float min1 = INFINITY;
            float max1 = -INFINITY;
            float min2 = INFINITY;
            float max2 = -INFINITY;

            // Run SAT calculate on edge
            for(unsigned int j = 0; j < c1->getPointCount(); j++){
                // Project point
                float proj = Math::dot(c1->getPointGlobal(j), normal);
                min1 = std::min(proj, min1);
                max1 = std::max(proj, max1);
            }
            
            for(unsigned int j = 0; j < c2->getPointCount(); j++){
                // Project point
                float proj = Math::dot(c2->getPointGlobal(j), normal);
                min2 = std::min(proj, min2);
                max2 = std::max(proj, max2);
            }

            // Check collision
            if(!(max2 >= min1 && max1 >= min2)) return false;
        }

        return true;
    };

    return check(c1, c2) && check(c2, c1);
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
        for(unsigned int j = i + 1; j < colliders.size(); j++){
            // Broadphase
            if(!collidesAABB(colliders[i], colliders[j])) continue;

            // Narrow phase
            if(!collidesSAT(colliders[i], colliders[j])) continue;

            // Collision occured, record it
            collisions.push_back({ colliders[i], colliders[j], sf::Vector2f(0, 1) });
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
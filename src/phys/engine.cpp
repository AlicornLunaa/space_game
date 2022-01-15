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

bool Engine::collidesDIAG(Collider* c1, Collider* c2){
    // Extract data
    CollisionData data1 = { c1, c2, sf::Vector2f(0, 1), sf::Vector2f(0, 0), 0 };
    CollisionData data2 = { c2, c1, sf::Vector2f(0, 1), sf::Vector2f(0, 0), 0 };

    // Define function
    auto check = [](Collider* c1, Collider* c2, CollisionData& data, float adj){
        // Loop through every diagonal
        for(unsigned int i = 0; i < c1->getPointCount(); i++){
            // Get the edge of the face
            sf::Vector2f start1 = c1->getPosition();
            sf::Vector2f end1 = c1->getPointGlobal(i);
            sf::Vector2f displacement(0, 0);
            
            // Get edge of other faces
            for(unsigned int j = 0; j < c2->getPointCount(); j++){
                // Edges
                sf::Vector2f start2 = c2->getPointGlobal(j);
                sf::Vector2f end2 = c2->getPointGlobal((j + 1) % c2->getPointCount());

                float h = (end2.x - start2.x) * (start1.y - end1.y) - (start1.x - end1.x) * (end2.y - start2.y);
                float t1 = ((start2.y - end2.y) * (start1.x - start2.x) + (end2.x - start2.x) * (start1.y - start2.y)) / h;
                float t2 = ((start1.y - end1.y) * (start1.x - start2.x) + (end1.x - start1.x) * (start1.y - start2.y)) / h;

                // Check collision
                if(t1 >= 0.f && t1 < 1.f && t2 >= 0.f && t2 < 1.f){
                    data.normal = Math::perpendicular(Math::normalize(end2 - start2));
                    displacement += (1.f - t1) * (end1 - start1);
                    data.contactPoint = i;
                    Debug::drawLine(start1.x, start1.y, end1.x, end1.y);
                    Debug::drawLine(start1.x, start1.y, start1.x + displacement.x * 50, start1.y + displacement.y * 50, sf::Color::Red);
                }
            }

            c1->move(displacement * adj);
        }

        return data.normal.x != 0 || data.normal.y != 0;
    };

    bool collided = check(c1, c2, data1, -1.1f) | check(c2, c1, data1, -1.1f);
    
    if(collided||true){
        collisions.push_back(data1);
    }

    return collided;
}

bool Engine::collidesSAT(Collider* c1, Collider* c2){
    // Store collision
    CollisionData data = { c1, c2, sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0 };

    // Define function
    auto check = [](Collider* c1, Collider* c2, CollisionData& data){
        // Prepare for getting data
        sf::Vector2f shortestDir(0, 0);
        float overlap = INFINITY;

        // Loop through every edge
        for(unsigned int i = 0; i < c1->getPointCount(); i++){
            // Get the edge of the face
            sf::Vector2f p1 = c1->getPointGlobal(i);
            sf::Vector2f p2 = c1->getPointGlobal((i + 1) % c1->getPointCount());
            sf::Vector2f mid = (p1 + p2) / 2.f;
            sf::Vector2f edge = p2 - p1;
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

            // Get overlap data to respond
            float currentOverlap = max1 - min2;
            if(currentOverlap < overlap){
                shortestDir = normal;
                overlap = currentOverlap;
                data.contactPoint = i;
            }

            // Check collision
            if(!(max2 >= min1 && max1 >= min2)) return false;
        }
        
        // Save collision data
        data.normal = shortestDir;
        data.displacement = shortestDir * overlap;

        return true;
    };

    bool collided = check(c1, c2, data) && check(c2, c1, data);
    
    if(collided){
        collisions.push_back(data);
    }

    return collided;
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

RigidBody* Engine::getRigidBody(unsigned int id){
    return dynamic_cast<RigidBody*>(colliders[id]);
}

void Engine::collisionDetection(){
    // Check all the collisions
    for(unsigned int i = 0; i < colliders.size(); i++){
        for(unsigned int j = i + 1; j < colliders.size(); j++){
            // Broadphase
            //if(!collidesAABB(colliders[i], colliders[j])) continue;

            // Narrow phase
            if(!collidesSAT(colliders[i], colliders[j])) continue;
        }
    }
}

void Engine::collisionResolution(){
    // Fix all the collisions
    for(CollisionData& c : collisions){
        // Solve collision
        c.c1->move(c.displacement);

        // Check if the collider is also a rigidbody
        RigidBody* r1 = dynamic_cast<RigidBody*>(c.c1);
        RigidBody* r2 = dynamic_cast<RigidBody*>(c.c2);

        // Update velocities
        if(r1 != NULL){ r1->velocity = Math::reflect(r1->velocity, c.normal); }
        if(r2 != NULL){ r2->velocity = Math::reflect(r2->velocity, c.normal); }
    }

    collisions.clear();
}

void Engine::physicsUpdate(){
    // Update each body
    for(Collider* c : colliders){
        // Update only rigidbodies
        RigidBody* rb = dynamic_cast<RigidBody*>(c);
        if(rb == NULL) continue;

        // Update position and other variables
        rb->velocity += rb->acceleration;
        rb->rotVelocity += rb->rotAcceleration;
        rb->move(rb->velocity);
        rb->rotate(rb->rotVelocity);
        rb->acceleration *= 0.f;
        rb->rotAcceleration *= 0.f;
    }
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
        sf::Color col = c->isSleeping() ? sf::Color::Green : sf::Color::Yellow;

        for(unsigned int i = 0; i < c->getPointCount(); i++){
            sf::Vector2f p1 = c->getPointGlobal(i);
            sf::Vector2f p2 = c->getPointGlobal((i + 1) % c->getPointCount());
            Debug::drawLine(p1, p2, col);
        }
        
        c->setSleeping(true);
    }
}
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
    CollisionData data1 = { c1, c2, sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0 };
    CollisionData data2 = { c2, c1, sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0 };

    // Define function
    auto check = [](Collider* c1, Collider* c2, CollisionData& data){
        // Loop through every diagonal
        for(unsigned int i = 0; i < c1->getPointCount(); i++){
            // Get the edge of the face
            sf::Vector2f start1 = c1->getPosition();
            sf::Vector2f end1 = c1->getPointGlobal(i);
            
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
                    data.displacement += (1.f - t1) * data.normal;
                    data.contactPoint = i;
                }
            }
        }

        return data.normal.x != 0 || data.normal.y != 0;
    };

    bool collided = check(c1, c2, data1) | check(c2, c1, data2);
    
    if(collided){
        collisions.push_back(data1);
        collisions.push_back(data2);
    }

    return collided;
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

RigidBody* Engine::getRigidBody(unsigned int id){
    return dynamic_cast<RigidBody*>(colliders[id]);
}

void Engine::collisionDetection(){
    // Check all the collisions
    for(unsigned int i = 0; i < colliders.size(); i++){
        for(unsigned int j = i + 1; j < colliders.size(); j++){
            // Broadphase
            if(!collidesAABB(colliders[i], colliders[j])) continue;

            // Narrow phase
            if(!collidesDIAG(colliders[i], colliders[j])) continue;
        }
    }
}

void Engine::collisionResolution(){
    // Fix all the collisions
    for(CollisionData& c : collisions){
        // Solve collision
        c.c1->move(c.displacement * -0.5f * c.c1->getSpringForce());
        c.c2->move(c.displacement * 0.5f * c.c2->getSpringForce());

        // Check if the collider is also a rigidbody
        RigidBody* r1 = dynamic_cast<RigidBody*>(c.c1);
        RigidBody* r2 = dynamic_cast<RigidBody*>(c.c2);

        // Update velocities
        if(r1 != NULL){
            r1->velocity *= 0.f;
        }

        if(r2 != NULL){
            r2->velocity *= 0.f;
        }
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
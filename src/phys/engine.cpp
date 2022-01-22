#include "engine.h"
using namespace Phys;

// Constructors
Engine::Engine(){
    // Initialize engine
    renderDebug = true; // Draw debug
}

Engine::~Engine(){}

// Functions
bool Engine::collidesAABB(CollisionBody* c1, CollisionBody* c2){
    // Get position of each and check if they collide
    sf::Vector2f p1 = c1->getPosition();
    sf::Vector2f s1 = c1->getScale() * 1.5f;
    sf::Vector2f p2 = c2->getPosition();
    sf::Vector2f s2 = c2->getScale() * 1.5f;

    return (p1.x - s1.x <= p2.x + s2.x && p1.x + s1.x >= p2.x - s2.x && p1.y - s1.y <= p2.y + s2.y && p1.y + s1.y >= p2.y - s2.y);
}

bool Engine::collidesSAT(CollisionBody* body1, CollisionBody* body2, int id1, int id2){
    // Store collision
    CollisionData data = { body1, body2, sf::Vector2f(0, 0), INFINITY, 0 };

    // Define function
    auto check = [](CollisionBody* b1, CollisionBody* b2, int id1, int id2, CollisionData& data, float scalar){
        // Get colliders
        Collider& c1 = b1->getCollider(id1);
        Collider& c2 = b2->getCollider(id2);

        // Loop through every edge
        for(unsigned int i = 0; i < c1.getPointCount(); i++){
            // Get the edge of the face
            sf::Vector2f p1 = b1->getPointOnCollider(id1, i);
            sf::Vector2f p2 = b1->getPointOnCollider(id1, (i + 1) % c1.getPointCount());
            sf::Vector2f edge = p2 - p1;
            sf::Vector2f normal = Math::perpendicular(Math::normalize(edge));

            // Edge data
            float min1 = INFINITY;
            float max1 = -INFINITY;
            float min2 = INFINITY;
            float max2 = -INFINITY;

            // Run SAT calculate on edge
            for(unsigned int j = 0; j < c1.getPointCount(); j++){
                // Project point
                float proj = Math::dot(b1->getPointOnCollider(id1, j), normal);
                min1 = std::min(proj, min1);
                max1 = std::max(proj, max1);
            }
            
            for(unsigned int j = 0; j < c2.getPointCount(); j++){
                // Project point
                float proj = Math::dot(b2->getPointOnCollider(id2, j), normal);
                min2 = std::min(proj, min2);
                max2 = std::max(proj, max2);
            }

            // Get overlap data to respond
            float currentOverlap = std::max(max2, min2) - std::min(max1, min1);
            if(currentOverlap < data.displacement){
                data.normal = normal * scalar;
                data.displacement = currentOverlap;
                data.contactPoint = i;
            }

            // Check collision
            if(!(max2 >= min1 && max1 >= min2)) return false;
        }

        return true;
    };

    // Make sure there are enough bodies
    if(body1->getCollidersSize() == 0 || body2->getCollidersSize() == 0) return false;

    // Commence check
    bool collided = check(body1, body2, id1, id2, data, -1) && check(body2, body1, id2, id1, data, 1);
    
    if(collided){
        collisions.push_back(data);
    }

    return collided;
}

unsigned int Engine::registerBody(CollisionBody* c){
    bodies.push_back(c);
    return bodies.size() - 1;
}

bool Engine::unregisterBody(unsigned int id){
    if(id >= bodies.size()) return false;

    bodies.erase(bodies.begin() + id);
    return true;
}

CollisionBody* Engine::getBody(unsigned int id){
    return bodies[id];
}

RigidBody* Engine::getRigidBody(unsigned int id){
    return dynamic_cast<RigidBody*>(bodies[id]);
}

void Engine::collisionDetection(){
    // Check all the collisions
    for(unsigned int i = 0; i < bodies.size(); i++){
        for(unsigned int j = i + 1; j < bodies.size(); j++){
            // Broadphase
            if(!getBody(i)->enabled || !getBody(j)->enabled) continue;
            //if(!collidesAABB(getBody(i), getBody(j))) continue;

            // Narrow phase
            // Test every collider
            for(unsigned int k = 0; k < getBody(i)->getCollidersSize(); k++){
                for(unsigned int l = 0; l < getBody(j)->getCollidersSize(); l++){
                    collidesSAT(getBody(i), getBody(j), k, l);
                }
            }
        }
    }
}

void Engine::collisionResolution(){
    // Fix all the collisions
    for(CollisionData& data : collisions){
        // Solve collision
        if(!data.c1->mStatic && !data.c2->mStatic){
            data.c1->move(data.normal * data.displacement * -0.5f);
            data.c2->move(data.normal * data.displacement * 0.5f);
        } else if(data.c1->mStatic){
            data.c2->move(data.normal * data.displacement * 1.f);
        } else {
            data.c1->move(data.normal * data.displacement * -1.f);
        }

        // Check if the collider is also a rigidbody
        RigidBody* r1 = dynamic_cast<RigidBody*>(data.c1);
        RigidBody* r2 = dynamic_cast<RigidBody*>(data.c2);

        // Update velocities
        if(r1 != NULL){
            r1->velocity = Math::reflect(r1->velocity, data.normal) * 0.5f;
        }

        if(r2 != NULL){
            r2->velocity = Math::reflect(r2->velocity, data.normal) * 0.5f;
        }
    }

    collisions.clear();
}

void Engine::physicsUpdate(){
    // Update each body
    for(CollisionBody* b : bodies){
        // Update only rigidbodies
        RigidBody* rb = dynamic_cast<RigidBody*>(b);
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

    for(CollisionBody* b : bodies){
        for(unsigned int i = 0; i < b->getCollidersSize(); i++){
            // Get current collider
            Collider& c = b->getCollider(i);

            // Draw lines
            for(unsigned int j = 0; j < c.getPointCount(); j++){
                sf::Vector2f p1 = b->getPointOnCollider(i, j);
                sf::Vector2f p2 = b->getPointOnCollider(i, (j + 1) % c.getPointCount());
                Interface::Renderer::drawLine(p1, p2, sf::Color::Green);
            }
        }
    }
}
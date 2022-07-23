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
    CollisionData data = { body1, body2, sf::Vector2f(0, 0), INFINITY };

    // Define functions
    auto check = [](CollisionBody* b1, CollisionBody* b2, int id1, int id2, CollisionData& data, float scalar){
        // Get colliders
        Collider& c1 = b1->getCollider(id1);
        Collider& c2 = b2->getCollider(id2);
        std::stack<sf::Vector2f> possibleAxises;
        sf::Vector2f center1 = b1->getTransform().transformPoint(c1.getPosition());
        sf::Vector2f center2 = b2->getTransform().transformPoint(c2.getPosition());
        sf::Vector2f direction = (center2 - center1);
        sf::Vector2f normal;
        std::vector<sf::Vector2f> contacts;
        float penetration = INFINITY;

        // Get every possible edge to check
        for(unsigned int i = 0; i < c1.getPointCount(); i++){
            sf::Vector2f p1 = b1->getPointOnCollider(id1, i);
            sf::Vector2f p2 = b1->getPointOnCollider(id1, (i + 1) % c1.getPointCount());
            sf::Vector2f perp = Math::perpendicular(p2 - p1);
            possibleAxises.push(perp);
        }
        
        for(unsigned int i = 0; i < c2.getPointCount(); i++){
            sf::Vector2f p1 = b2->getPointOnCollider(id2, i);
            sf::Vector2f p2 = b2->getPointOnCollider(id2, (i + 1) % c2.getPointCount());
            sf::Vector2f perp = Math::perpendicular(p2 - p1);
            possibleAxises.push(perp);
        }

        // Try every perpendicular edge
        while(possibleAxises.size() > 0){
            // Project every vertex from the shapes onto the lines
            sf::Vector2f edge = possibleAxises.top();
            possibleAxises.pop();

            // Edge data
            sf::Vector2f min1;
            sf::Vector2f max1;
            sf::Vector2f min2;
            sf::Vector2f max2;
            float a = INFINITY;
            float b = -INFINITY;
            float c = INFINITY;
            float d = -INFINITY;

            // Body 1
            for(unsigned int j = 0; j < c1.getPointCount(); j++){
                sf::Vector2f p = b1->getPointOnCollider(id1, j);
                float proj = Math::dot(p, edge);

                // Get min and maxs
                if(proj < a){
                    min1 = p;
                    a = proj;
                }
                
                if(proj > b){
                    max1 = p;
                    b = proj;
                }
            }

            // Body 2
            for(unsigned int j = 0; j < c2.getPointCount(); j++){
                sf::Vector2f p = b2->getPointOnCollider(id2, j);
                float proj = Math::dot(p, edge);

                // Get min and maxs
                if(proj < c){
                    min2 = p;
                    c = proj;
                }
                
                if(proj > d){
                    max2 = p;
                    d = proj;
                }
            }

            // Final calculation
            if(!((a < d && a > c) || (c < b && c > a))){
                return false;
            }

            float edgePenetration = std::min(d - a, b - c);
            if(edgePenetration < penetration){
                penetration = edgePenetration;
                normal = edge;
            }
        }

        penetration /= Math::magnitude(normal);
        normal = Math::normalize(normal);

        if(Math::dot(direction, normal) < 0.f){
            normal *= -1.f;
        }

        for(unsigned int i = 0; i < c1.getPointCount(); i++){
            sf::Vector2f p = b1->getPointOnCollider(id1, i);
            
            if(b2->contains(p)){
                Interface::Renderer::drawPoint(p, 2.f, sf::Color::Cyan);
                contacts.push_back(p);
            }
        }

        // for(unsigned int i = 0; i < c2.getPointCount(); i++){
        //     sf::Vector2f p = b2->getPointOnCollider(id2, i);
            
        //     if(b1->contains(p)){
        //         contacts.push_back(p);
        //     }
        // }

        for(sf::Vector2f p : contacts){
            data.contactPoint += p;
        }

        data.normal = normal;
        data.displacement = penetration;
        data.contactPoint /= (float)contacts.size();

        Interface::Renderer::drawLine(b1->getCenter(), b1->getCenter() + normal * -penetration);
        Interface::Renderer::drawPoint(data.contactPoint);

        return true;
    };

    // Make sure there are enough bodies
    if(body1->getCollidersSize() == 0 || body2->getCollidersSize() == 0) return false;

    // Commence check
    bool collided = check(body1, body2, id1, id2, data, -1);
    
    if(collided){
        collisions.push_back(data);

        Interface::Renderer::drawLine(data.contactPoint, data.contactPoint + data.normal * -data.displacement, sf::Color::Yellow);
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

unsigned int Engine::getBodyCount(){ return bodies.size(); }

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

void Engine::collisionResolution(float deltaTime){
    // Fix all the collisions
    for(CollisionData& data : collisions){
        // Solve collision
        Interface::Renderer::drawText(10, 50, std::to_string(data.displacement));
        Interface::Renderer::drawPoint(data.c1->getPosition());

        float slop = 0.005f;
        float percent = 0.9f;
        sf::Vector2f correction = (std::max(data.displacement - slop, 0.f)) * data.normal * percent;

        if(!data.c1->mStatic && !data.c2->mStatic){
            data.c1->move(-correction);
            data.c2->move(correction);
        } else if(data.c1->mStatic){
            data.c2->move(correction);
        } else {
            data.c1->move(-correction);
        }

        // Check if the collider is also a rigidbody
        RigidBody* r1 = dynamic_cast<RigidBody*>(data.c1);
        RigidBody* r2 = dynamic_cast<RigidBody*>(data.c2);

        // Update velocities
        if(r1 != NULL && r2 != NULL){
            // Velocity impulse
            sf::Vector2f relativeVelocity = r2->velocity - r1->velocity;
            float velocityProj = Math::dot(relativeVelocity, data.normal);
            float restitution = std::min(r1->elasticity, r2->elasticity);
            float massLoss = (1.f / r1->mass + 1.f / r2->mass);
            float scale = (-(1 + restitution) * velocityProj) / massLoss;
            sf::Vector2f impulse = data.normal * scale;

            if(velocityProj > 0) continue;
            if(!r1->mStatic){ r1->applyImpulse(-impulse); }
            if(!r2->mStatic){ r2->applyImpulse(impulse); }

            // Contact based impulses
            for(unsigned int i = 0; i < 1/*data.contactPoints.size()*/; i++){
                // Rotational impulse
                // Get radius for the point being hit
                sf::Vector2f contact = data.contactPoint;
                sf::Vector2f radius1 = contact - r1->getCenter();
                sf::Vector2f radius2 = contact - r2->getCenter();

                // Both valid
                float rotationLoss = (std::pow(Math::cross(radius1, data.normal).z, 2) * (1.f / r1->inertia) + std::pow(Math::cross(radius2, data.normal).z, 2) * (1.f / r2->inertia));
                sf::Vector2f rv = relativeVelocity + Math::cross(r2->rotVelocity * (3.1415f / 180), radius2) - Math::cross(r1->rotVelocity * (3.1415f / 180), radius1);
                velocityProj = Math::dot(rv, data.normal);
                scale = (-(1 + restitution) * velocityProj) / (massLoss + rotationLoss) / 1/*data.contactPoints.size()*/;
                sf::Vector2f impulse = data.normal * scale;

                Interface::Renderer::drawPoint(contact);
                Interface::Renderer::drawLine(contact, contact + impulse * 10.f);

                if(velocityProj > 0) continue;
                if(!r1->mStatic){ r1->applyImpulse(-impulse, radius1); }
                if(!r2->mStatic){ r2->applyImpulse(impulse, radius2); }

                // Friction impulse
                // rv = relativeVelocity + Math::cross(r2->rotVelocity * (3.1415f / 180), radius2) - Math::cross(r1->rotVelocity * (3.1415f / 180), radius1);
                // sf::Vector2f t = rv - (data.normal * Math::dot(rv, data.normal));
                // float jt = -Math::dot(rv, t);
                // jt /= (massLoss + rotationLoss);
                // jt /= data.contactPoints.size();

                // if(jt == 0.f) continue;
                // impulse = (std::abs(jt) < scale * std::sqrt(r1->staticFriction * r2->staticFriction)) ? (t * jt) : (t * -scale * std::sqrt(r1->dynamicFriction * r2->dynamicFriction));
                // if(!r1->mStatic){ r1->applyImpulse(-impulse, radius1); }
                // if(!r2->mStatic){ r2->applyImpulse(impulse, radius2); }
            }
        }
    }

    collisions.clear();
}

void Engine::physicsUpdate(float deltaTime){
    // Update each body
    for(CollisionBody* b : bodies){
        // Update only rigidbodies
        RigidBody* rb = dynamic_cast<RigidBody*>(b);
        if(rb == NULL) continue;

        // Update body dynamics
        rb->velocity += rb->force / rb->mass * deltaTime;
        rb->rotVelocity += rb->torque / rb->mass * deltaTime;

        rb->move(rb->velocity * deltaTime);
        rb->rotate(rb->rotVelocity * deltaTime);
        
        rb->force *= 0.f;
        rb->torque *= 0.f;
    }
}

void Engine::update(float deltaTime){
    collisionDetection();
    // collisionResolution(deltaTime);
    physicsUpdate(deltaTime);
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
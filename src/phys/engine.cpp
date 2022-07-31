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

    // Basic SAT implementation
    auto check = [](CollisionBody* b1, CollisionBody* b2, int id1, int id2, CollisionData& data, float scalar){
        // SAT algorithm
        // Get colliders
        Collider& c1 = b1->getCollider(id1);
        Collider& c2 = b2->getCollider(id2);
        std::stack<sf::Vector2f> possibleAxises;
        sf::Vector2f center1 = b1->getTransform().transformPoint(c1.getPosition());
        sf::Vector2f center2 = b2->getTransform().transformPoint(c2.getPosition());
        sf::Vector2f direction = (center2 - center1);
        sf::Vector2f normal;
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

        data.normal = normal;
        data.displacement = penetration;

        Interface::Renderer::drawLine(b1->getCenter(), b1->getCenter() + normal * -penetration);

        return true;
    };

    // Incident and reference clipping to get the contact points
    auto getManifold = [](CollisionBody* b1, CollisionBody* b2, int id1, int id2, sf::Vector2f normal, float penetration){
        // Basic vars
        Collider& c1 = b1->getCollider(id1);
        Collider& c2 = b2->getCollider(id2);

        // Start by finding the significant vertex, the point furthest along the normal
        sf::Vector2f vertex1, vertex2;
        unsigned int index1, index2 = 0;
        float maxProj = -INFINITY;

        for(unsigned int i = 0; i < c1.getPointCount(); i++){
            sf::Vector2f point = b1->getPointOnCollider(id1, i);
            float proj = Math::dot(normal, point);

            if(proj >= maxProj){
                vertex1 = point;
                index1 = i;
                maxProj = proj;
            }
        }
        
        maxProj = -INFINITY;
        for(unsigned int i = 0; i < c2.getPointCount(); i++){
            sf::Vector2f point = b2->getPointOnCollider(id2, i);
            float proj = Math::dot(-normal, point);

            if(proj >= maxProj){
                vertex2 = point;
                index2 = i;
                maxProj = proj;
            }
        }

        // Find the two faces involved with contact
        std::pair<Face, Face> possibleFaces1 = {
            Face(b1, id1, (index1 - 1) % c1.getPointCount(), index1),
            Face(b1, id1, index1, (index1 + 1) % c1.getPointCount())
        };
        std::pair<Face, Face> possibleFaces2 = {
            Face(b2, id2, (index2 - 1) % c2.getPointCount(), index2),
            Face(b2, id2, index2, (index2 + 1) % c2.getPointCount())
        };
        std::pair<Face, Face> significantFaces;
        float collisionSlope = normal.y / normal.x;

        // Decide the first of the two faces by checking which is more parallel to the normal
        if(std::abs(possibleFaces1.first.getSlope() - collisionSlope) <= std::abs(possibleFaces1.second.getSlope() - collisionSlope)){
            // The first face is more parallel
            significantFaces.first = possibleFaces1.first;
        } else {
            // The second face is more parallel
            significantFaces.first = possibleFaces1.second;
        }

        // Decide the second of the two faces by checking which is more parallel to the normal
        if(std::abs(possibleFaces2.first.getSlope() - collisionSlope) <= std::abs(possibleFaces2.second.getSlope() - collisionSlope)){
            // The first face is more parallel
            significantFaces.second = possibleFaces2.first;
        } else {
            // The second face is more parallel
            significantFaces.second = possibleFaces2.second;
        }

        // Classify which face is the incident and which is the reference by calculating which of significantFaces are more parallel to the normal
        Face referenceFace = significantFaces.first;
        Face incidentFace = significantFaces.second;
        if(std::abs(significantFaces.first.getSlope() - collisionSlope) < std::abs(significantFaces.second.getSlope() - collisionSlope)){
            // The first face is more parallel, its the reference
            referenceFace = significantFaces.second;
            incidentFace = significantFaces.first;
        }
        
        // Adjacent faces to the reference are required for the algorithm, first is counter clockwise, second clockwise
        std::pair<Face, Face> adjacentFaces = referenceFace.getAdjacents();

        // Debug
        Interface::Renderer::drawLine(referenceFace.start, referenceFace.end, sf::Color::Blue);
        Interface::Renderer::drawLine(adjacentFaces.first.start, adjacentFaces.first.end, sf::Color::Yellow);
        Interface::Renderer::drawLine(adjacentFaces.second.start, adjacentFaces.second.end, sf::Color::Yellow);
        Interface::Renderer::drawLine(incidentFace.start, incidentFace.end, sf::Color::Red);
        Interface::Renderer::drawPoint(incidentFace.start, 2.f, sf::Color::Red);
        Interface::Renderer::drawPoint(incidentFace.end, 2.f, sf::Color::Red);

        Interface::Renderer::drawText(referenceFace.start.x, referenceFace.start.y, std::to_string(referenceFace.startIndex));
        Interface::Renderer::drawText(referenceFace.end.x, referenceFace.end.y, std::to_string(referenceFace.endIndex));

        Interface::Renderer::drawText(adjacentFaces.first.end.x, adjacentFaces.first.end.y, std::to_string(adjacentFaces.first.endIndex), 24u, sf::Color::Red);
        Interface::Renderer::drawText(adjacentFaces.second.end.x, adjacentFaces.second.end.y, std::to_string(adjacentFaces.second.endIndex), 24u, sf::Color::Red);

        Interface::Renderer::drawPoint(adjacentFaces.first.start, 2.f, sf::Color::Cyan);
        Interface::Renderer::drawPoint(adjacentFaces.first.end, 2.f, sf::Color::Cyan);
        Interface::Renderer::drawPoint(adjacentFaces.second.start, 2.f, sf::Color::Magenta);
        Interface::Renderer::drawPoint(adjacentFaces.second.end, 2.f, sf::Color::Magenta);
    };

    // Make sure there are enough bodies
    if(body1->getCollidersSize() == 0 || body2->getCollidersSize() == 0) return false;

    // Commence final check
    if(check(body1, body2, id1, id2, data, -1)){
        collisions.push_back(data);

        getManifold(body1, body2, id1, id2, data.normal, data.displacement);

        return true;
    }

    return false;
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
            for(unsigned int i = 0; i < data.contactPoints.size(); i++){
                // Rotational impulse
                // Get radius for the point being hit
                sf::Vector2f contact = data.contactPoints[i];
                sf::Vector2f radius1 = contact - r1->getCenter();
                sf::Vector2f radius2 = contact - r2->getCenter();

                // Both valid
                float rotationLoss = (std::pow(Math::cross(radius1, data.normal).z, 2) * (1.f / r1->inertia) + std::pow(Math::cross(radius2, data.normal).z, 2) * (1.f / r2->inertia));
                sf::Vector2f rv = relativeVelocity + Math::cross(r2->rotVelocity * (3.1415f / 180), radius2) - Math::cross(r1->rotVelocity * (3.1415f / 180), radius1);
                velocityProj = Math::dot(rv, data.normal);
                scale = (-(1 + restitution) * velocityProj) / (massLoss + rotationLoss) / data.contactPoints.size();
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
#include "collisionbody.h"
using namespace Phys;

// CollisionBody class
// Constructors
CollisionBody::CollisionBody(float x, float y, float rotation){
    // Initialize the body
    setPosition(x, y);
    setRotation(rotation);

    enabled = true;
    mStatic = false;
}

CollisionBody::~CollisionBody(){}

// Functions
unsigned int CollisionBody::getCollidersSize(){ return colliders.size(); }
Collider& CollisionBody::getCollider(unsigned int id){ return colliders[id]; }
void CollisionBody::addCollider(Collider c){ colliders.push_back(c); }
void CollisionBody::clearColliders(){ colliders.clear(); }
void CollisionBody::removeCollider(unsigned int id){ colliders.erase(colliders.begin() + id); }

sf::Vector2f CollisionBody::getPointOnCollider(unsigned int collider, unsigned int point){
    // Get position at collider and then transform it by the parent
    sf::Vector2f p = getCollider(collider).getPoint(point);
    return getTransform().transformPoint(p);
}

sf::Vector2f CollisionBody::getCenter(){
    return getPosition();
}

sf::Vector2f CollisionBody::getUp(){ return sf::Vector2f(std::cos((getRotation() - 90) * (3.1415 / 180)), std::sin((getRotation() - 90) * (3.1415 / 180))); }
sf::Vector2f CollisionBody::getRight(){ return sf::Vector2f(std::cos(getRotation() * (3.1415 / 180)), std::sin(getRotation() * (3.1415 / 180))); }

bool CollisionBody::contains(sf::Vector2f point){
    auto getAngle = [](float a, float b, float c){ return std::acos((a - b - c) / (-2 * b * c)); };

    for(Collider& c : colliders){
        float sum = 0.f;

        for(unsigned int i = 0; i < c.getPointCount(); i++){
            // Get two endpoints for a triangle
            sf::Vector2f p1 = getTransform().transformPoint(c.getPoint(i));
            sf::Vector2f p2 = getTransform().transformPoint(c.getPoint((i + 1) % c.getPointCount()));

            // Law of cosines requires the angle being found to be across of the wall
            // Triangle will be formed from point to p1 to p2 and back to point
            /*
                    * Point
                     / \
                c   /   \  b
                   /     \
                  /       \
              p2 *----a----* p1
            */
            float a = Math::distanceSquare(p1, p2);
            float b = Math::distanceSquare(p2, point);
            float c = Math::distanceSquare(point, p1);
            sum += getAngle(a, b, c);
        }

        // If the point is inside this polygon, the sum will be greater than 2pi
        if(sum >= 3.141592654 * 2){
            return true;
        }
    }

    return false;
}

// RigidBody class
// Constructors
RigidBody::RigidBody(float x, float y, float rotation) : CollisionBody(x, y, rotation){
    // Initialize physics properties
    velocity = sf::Vector2f(0.f, 0.f);
    force = sf::Vector2f(0.f, 0.f);
    rotVelocity = 0.f;
    torque = 0.f;
    mass = 1.f;
    inertia = 10.f;
    elasticity = 0.2f;
    staticFriction = 0.009f;
    dynamicFriction = 0.006f;
}

RigidBody::~RigidBody(){}

// Functions
void RigidBody::applyImpulse(sf::Vector2f impulse){
    velocity += (1.f / mass) * impulse;
}

void RigidBody::applyImpulse(sf::Vector2f impulse, sf::Vector2f contactVec){
    applyImpulse(impulse);
    rotVelocity += (1.f / inertia) * Math::cross(contactVec, impulse).z;
}

// Static functions
RigidBody* RigidBody::createBoxRigidBody(float x, float y, float w, float h, float rotation){
    // Create new body
    RigidBody* rb = new RigidBody(x, y, rotation);
    rb->addCollider(Collider::createBoxCollider(0.f, 0.f, w, h, 0.f));

    return rb;
}

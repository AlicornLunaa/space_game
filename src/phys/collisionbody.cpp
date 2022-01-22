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

sf::Vector2f CollisionBody::getUp(){ return sf::Vector2f(std::cos((getRotation() - 90) * (3.1415 / 180)), std::sin((getRotation() - 90) * (3.1415 / 180))); }
sf::Vector2f CollisionBody::getRight(){ return sf::Vector2f(std::cos(getRotation() * (3.1415 / 180)), std::sin(getRotation() * (3.1415 / 180))); }

// RigidBody class
// Constructors
RigidBody::RigidBody(float x, float y, float rotation) : CollisionBody(x, y, rotation){
    // Initialize physics properties
    velocity = sf::Vector2f(0.f, 0.f);
    acceleration = sf::Vector2f(0.f, 0.f);
    rotVelocity = 0.f;
    rotAcceleration = 0.f;
    mass = 1.f;
}

RigidBody::~RigidBody(){}

// Static functions
RigidBody* RigidBody::createBoxRigidBody(float x, float y, float w, float h, float rotation){
    // Create new body
    RigidBody* rb = new RigidBody(x, y, rotation);
    rb->addCollider(Collider::createBoxCollider(0.f, 0.f, w, h, 0.f));

    return rb;
}

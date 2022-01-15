#include "rigidbody.h"
using namespace Phys;

// Base class
RigidBody::RigidBody(float x, float y, float rotation) : Collider(x, y, rotation){
    velocity = sf::Vector2f(0.f, 0.f);
    acceleration = sf::Vector2f(0.f, 0.f);
    rotVelocity = 0.f;
    rotAcceleration = 0.f;
}

RigidBody::~RigidBody(){}


// Box class
BoxRigidBody::BoxRigidBody(float x, float y, float w, float h, float rotation) : RigidBody(x, y, rotation){
    // Create points
    setScale(sf::Vector2f(w / 2, h / 2));
    addPoint(-1,  1);
    addPoint( 1,  1);
    addPoint( 1, -1);
    addPoint(-1, -1);
}

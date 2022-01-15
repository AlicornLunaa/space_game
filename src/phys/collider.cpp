#include "collider.h"
using namespace Phys;

// Base class
Collider::Collider(float x, float y, float rotation){
    setPosition(x, y);
    setRotation(rotation);
    sleeping = true;
    springForce = 50.f;
}

Collider::~Collider(){}

bool Collider::isSleeping(){ return sleeping; }
void Collider::setSleeping(bool s){ sleeping = s; }
float Collider::getSpringForce(){ return springForce; }
void Collider::setSpringForce(float s){ springForce = s; }
void Collider::addPoint(float x, float y){ points.push_back(sf::Vector2f(x, y)); }
sf::Vector2f Collider::getPoint(unsigned int i){ return points[i]; }
unsigned int Collider::getPointCount(){ return points.size(); }

sf::Vector2f Collider::getPointGlobal(unsigned int i){
    sf::Transform t = getTransform();
    return t.transformPoint(getPoint(i));
}


// Box class
BoxCollider::BoxCollider(float x, float y, float w, float h, float rotation) : Collider(x, y, rotation){
    // Create points
    setScale(sf::Vector2f(w / 2, h / 2));
    addPoint(-1,  1);
    addPoint( 1,  1);
    addPoint( 1, -1);
    addPoint(-1, -1);
}
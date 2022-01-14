#include "collider.h"
using namespace Phys;

// Base class
Collider::Collider(float x, float y, float rotation){
    setPosition(x, y);
    setRotation(rotation);
}

Collider::~Collider(){}

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
    addPoint(-1 - w/2,  1 + h/2);
    addPoint( 1 + w/2,  1 + h/2);
    addPoint( 1 + w/2, -1 - h/2);
    addPoint(-1 - w/2, -1 - h/2);
}
#include "colliders.h"
using namespace Phys;

// Collider class
// Constructors
Collider::Collider(float x, float y, float rotation){ create(x, y, rotation); }
Collider::Collider(){}
Collider::~Collider(){}

void Collider::create(float x, float y, float rotation){
    setPosition(x, y);
    setRotation(rotation);
}

// Functions
void Collider::addPoint(float x, float y){ points.push_back(sf::Vector2f(x, y)); }
void Collider::clearPoints(){ points.clear(); }

sf::Vector2f Collider::getPoint(unsigned int i){ return getTransform().transformPoint(points[i]); }
unsigned int Collider::getPointCount(){ return points.size(); }

sf::Vector2f Collider::getUp(){ return sf::Vector2f(std::cos((getRotation() - 90) * (3.1415 / 180)), std::sin((getRotation() - 90) * (3.1415 / 180))); }
sf::Vector2f Collider::getRight(){ return sf::Vector2f(std::cos(getRotation() * (3.1415 / 180)), std::sin(getRotation() * (3.1415 / 180))); }

// Static functions
Collider Collider::createBoxCollider(float x, float y, float w, float h, float rotation){
    // Create points
    Collider c(x, y, rotation);
    c.setScale(sf::Vector2f(w / 2, h / 2));
    c.addPoint(-1,  1);
    c.addPoint( 1,  1);
    c.addPoint( 1, -1);
    c.addPoint(-1, -1);
    
    return c;
}
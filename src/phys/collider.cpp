#include "collider.h"
using namespace Phys;

Collider::Collider(float x, float y, float rotation){
    position.x = x;
    position.y = y;
    this->rotation = rotation;
}

Collider::~Collider(){}

void Collider::addPoint(sf::Vector2f v){ points.push_back(v); }
sf::Vector2f Collider::getPoint(unsigned int i){ return points[i]; }
unsigned int Collider::getPointCount(){ return points.size(); }
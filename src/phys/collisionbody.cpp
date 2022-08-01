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
    auto getAngle = [](float a, float b, float c){ return std::acos((b + c - a) / (2 * b * c)); };
    auto inside = [](sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f p){
        unsigned int count = 0;

        if((p.y < v1.y != p.y < v2.y) && (p.x < (v2.x - v1.x) * (p.y - v1.y) / (v2.y - v1.y) + v1.x)){
           count++; 
        }

        return count % 2 != 0;
    };

    for(Collider& c : colliders){
        float sum = 0.f;

        for(unsigned int i = 0; i < c.getPointCount(); i++){
            // Get two endpoints for a triangle
            sf::Vector2f p1 = getTransform().transformPoint(c.getPoint(i));
            sf::Vector2f p2 = getTransform().transformPoint(c.getPoint((i + 1) % c.getPointCount()));
            
            if(inside(p1, p2, point)){
                Interface::Renderer::drawPoint(point, 2.f, sf::Color::Cyan);
            } else {
                Interface::Renderer::drawPoint(point, 2.f, sf::Color::Red);
            }
        }

        // If the point is inside this polygon, the sum will be greater than 2pi
        if(sum >= 3.14159265358979323846 * 2){
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

// Face structure
Face::Face(CollisionBody* body, int collider, int startIndex, int endIndex){
    this->body = body;
    this->collider = collider;
    this->startIndex = startIndex;
    this->endIndex = endIndex;

    if(startIndex > endIndex){
        this->startIndex = endIndex;
        this->endIndex = startIndex;
    }
    
    start = body->getPointOnCollider(collider, this->startIndex);
    end = body->getPointOnCollider(collider, this->endIndex);

    if(this->endIndex - this->startIndex > 1){
        adjacentStart = (this->startIndex + 1) % body->getCollider(collider).getPointCount();
        adjacentEnd = (this->endIndex - 1) % body->getCollider(collider).getPointCount();
    } else {
        adjacentStart = (this->startIndex - 1) % body->getCollider(collider).getPointCount();
        adjacentEnd = (this->endIndex + 1) % body->getCollider(collider).getPointCount();
    }
}
Face::Face(CollisionBody* body, int collider, sf::Vector2f start, sf::Vector2f end, int startIndex, int endIndex, int adjacentStart, int adjacentEnd){
    this->body = body;
    this->collider = collider;
    this->start = start;
    this->end = end;
    this->startIndex = startIndex;
    this->endIndex = endIndex;
    this->adjacentStart = adjacentStart;
    this->adjacentEnd = adjacentEnd;
}
Face::Face(CollisionBody* body, int collider, sf::Vector2f start, sf::Vector2f end, int startIndex, int endIndex){
    this->body = body;
    this->collider = collider;
    this->start = start;
    this->end = end;
    this->startIndex = startIndex;
    this->endIndex = endIndex;
}
Face::Face(CollisionBody* body, int collider, sf::Vector2f start, sf::Vector2f end){
    this->body = body;
    this->collider = collider;
    this->start = start;
    this->end = end;
    this->startIndex = -1;
    this->endIndex = -1;
}
Face::Face(){
    start = sf::Vector2f(0, 0);
    end = sf::Vector2f(0, 0);
    this->startIndex = -1;
    this->endIndex = -1;
}

sf::Vector2f Face::getCenter(){ return (start + end) / 2.f; }
sf::Vector2f Face::getNormal(){
    sf::Vector2f normal = Math::normalize(Math::perpendicular(end - start));
    sf::Vector2f pos = body->getTransform().transformPoint(body->getCollider(collider).getPosition());

    if(Math::dot(getCenter() - pos, normal) < 0.f){
        normal *= -1.f;
    }

    return -normal;
}
float Face::getSlope(){ return (end.y - start.y) / (end.x - start.x); }
std::pair<Face, Face> Face::getAdjacents(){
    return {
        Face(body, collider, start, body->getPointOnCollider(collider, adjacentStart), startIndex, adjacentStart),
        Face(body, collider, end, body->getPointOnCollider(collider, adjacentEnd), endIndex, adjacentEnd)
    };
}
sf::Vector2f Face::project(Face face, sf::Vector2f point){
    /* Equations
     * x = (-m2 * x2 + y2 + m1 * x1 - y1) / (m1 - m2)
     * y = m1(x - x1) + y1
     */
    // Projected point
    float m1 = getSlope();
    float m2 = face.getSlope();
    float x = ((-m2 * face.start.x) + face.start.y + (m1 * start.x) - start.y) / (m1 - m2);
    float y = (m1 * (x - start.x)) + start.y;

    // Edge-case for a slope being vertical, aka infinite
    Interface::Renderer::drawPlane((this->start + this->end) / 2.f, this->getNormal(), sf::Color(200, 50, 50, 50));

    // Check clip edge using inequality
    if((start.x >= end.x && point.y <= (m2 * (point.x - face.start.x)) + face.start.y) || (start.x <= end.x && point.y >= (m2 * (point.x - face.start.x)) + face.start.y)){
        return sf::Vector2f(x, y);
    }

    Interface::Renderer::drawText(start.x, start.y, "A");
    return point;
}
void Face::draw(sf::Color color){
    Interface::Renderer::drawLine(start, end, color);
    Interface::Renderer::drawPoint(start, 2.f, color);
    Interface::Renderer::drawPoint(end, 2.f, color);
}
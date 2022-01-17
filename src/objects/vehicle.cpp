#include "vehicle.h"
using namespace Vehicles;

// Base class
Vehicle::Vehicle(Phys::Engine& engine, float x, float y, unsigned int width, unsigned int height, unsigned int scale){
    // Initialize a ship
    body.create(width, height, sf::Color::White);
    updateTexture();

    setPosition(x, y);
    setSize(sf::Vector2f(width, height) * (float)scale);
    setOrigin(sf::Vector2f(width, height) * (float)scale * 0.5f);

    rigidbody = new Phys::BoxRigidBody(x, y, width, height, 0.f);
    engine.registerCollider(rigidbody);
}

Vehicle::~Vehicle(){
    delete rigidbody;
}

Phys::RigidBody* Vehicle::getRigidBody(){ return rigidbody; }

void Vehicle::updateTexture(){
    // Reset the texture
    tex.loadFromImage(body);
    setTexture(&tex);
}

// Ship vehicle
Ship::Ship(Phys::Engine& engine, float x, float y) : Vehicle(engine, x, y, 32, 64, 1){
    // Initialize a ship
    body.loadFromFile("./res/textures/ship.png");
    updateTexture();
}

Ship::~Ship(){}

void Ship::updateKeys(float deltaTime){
    // Controls
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ rigidbody->acceleration = rigidbody->getUp() * 10.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ rigidbody->acceleration = rigidbody->getUp() * -10.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ rigidbody->rotAcceleration = -2.5f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ rigidbody->rotAcceleration = 2.5f * deltaTime; }
}

void Ship::update(float deltaTime){
    // Update physics
    setPosition(rigidbody->getPosition());
    setRotation(rigidbody->getRotation());
}
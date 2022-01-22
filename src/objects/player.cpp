#include "player.h"
using namespace Objects;

// Constructors
Player::Player(Phys::Engine& engine, float x, float y){ create(engine, x, y); }
Player::Player(){}
Player::~Player(){}

void Player::create(Phys::Engine& engine, float x, float y){
    // Initialize the player object
    tex.loadFromFile("./res/textures/player.png");
    setTexture(&tex);

    setPosition(x, y);
    setSize(sf::Vector2f(16, 32));
    setOrigin(getSize() * 0.5f);

    rigidbody = Phys::RigidBody::createBoxRigidBody(x, y, 16, 32, 0.f);
    engine.registerBody(rigidbody);

    driving = false;
    vehicle = NULL;
}

// Private functions
void Player::updateKeys(float deltaTime){
    // Controls
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ rigidbody->acceleration += rigidbody->getRight() * -8.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ rigidbody->acceleration += rigidbody->getRight() * 8.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ rigidbody->acceleration += rigidbody->getUp() * 14.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ rigidbody->acceleration += rigidbody->getUp() * -14.f * deltaTime; }
}

// Vehicle functions
void Player::drive(Vehicles::Vehicle* v){
    driving = !(v == NULL); 
    vehicle = v;

    if(!driving){
        rigidbody->setRotation(0);
    }
}

bool Player::isDriving(){ return driving; }
Vehicles::Vehicle* Player::getVehicle(){ return vehicle; }

Phys::RigidBody* Player::getRigidBody(){ return rigidbody; }

// Physics functions
void Player::update(float deltaTime){
    // Parent the vehicle to the player if driving
    rigidbody->enabled = true;
    
    if(isDriving()){
        Vehicles::Vehicle* v = getVehicle();
        v->updateKeys(deltaTime);

        rigidbody->setPosition(v->getPosition());
        rigidbody->setRotation(v->getRotation());
        rigidbody->enabled = false;
    }

    updateKeys(deltaTime);
    setPosition(rigidbody->getPosition());
    setRotation(rigidbody->getRotation());

    rigidbody->velocity *= 0.99f;
}

// Rendering functions
void Player::render(sf::RenderTarget* target){
    // Draw the player
    target->draw(*this);

    // Draw ship if the player is in one
    if(isDriving()){
        target->draw(*getVehicle());
    }
}
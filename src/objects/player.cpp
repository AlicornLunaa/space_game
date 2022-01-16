#include "player.h"
using namespace Objects;

// Constructors
Player::Player(Phys::Engine& engine, float x, float y){
    // Initialize the player object
    tex.loadFromFile("./res/textures/player.png");
    setTexture(&tex);

    setPosition(x, y);
    setSize(sf::Vector2f(16, 32));
    setOrigin(getSize() * 0.5f);

    rigidbody = new Phys::BoxRigidBody(x, y, 16, 32, 0.f);
    engine.registerCollider(rigidbody);

    driving = false;
    vehicle = NULL;
}

Player::~Player(){
    delete rigidbody;
}

// Private functions
void Player::updateKeys(float deltaTime){
    // Controls
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ rigidbody->acceleration = rigidbody->getRight() * -5.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ rigidbody->acceleration = rigidbody->getRight() * 5.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ rigidbody->acceleration = rigidbody->getUp() * 5.f * deltaTime; }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ rigidbody->acceleration = rigidbody->getUp() * -5.f * deltaTime; }
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

// Physics functions
void Player::update(float deltaTime){
    // Parent the vehicle to the player if driving
    rigidbody->setEnabled(true);
    
    if(isDriving()){
        Vehicles::Vehicle* v = getVehicle();
        v->updateKeys(deltaTime);

        rigidbody->setPosition(v->getPosition());
        rigidbody->setRotation(v->getRotation());
        rigidbody->setEnabled(false);
    }

    rigidbody->acceleration.y += 9.8 * deltaTime;

    updateKeys(deltaTime);
    setPosition(rigidbody->getPosition());
    setRotation(rigidbody->getRotation());
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
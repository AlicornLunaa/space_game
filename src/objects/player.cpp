#include "player.h"
using namespace Objects;

// Constructors
Player::Player(float x, float y){
    // Initialize the player object
    setPosition(x, y);
    setSize(sf::Vector2f(20, 20));
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color::Cyan);

    vehicle = nullptr;
}

Player::~Player(){}

// Vehicle functions
void Player::drive(Vehicles::Vehicle* v){ vehicle = v; }
bool Player::isDriving(){ return (vehicle != nullptr); }
Vehicles::Vehicle* Player::getVehicle(){ return vehicle; }

// Physics functions
void Player::update(float deltaTime){
    // Parent the vehicle to the player if driving
    if(isDriving()){
        Vehicles::Vehicle* v = getVehicle();

        v->update(deltaTime);
        setPosition(v->getPosition());
        setRotation(v->getRotation());
    }
}

// Rendering functions
void Player::render(sf::RenderTarget* target){
    // Draw ship if the player is in one
    if(isDriving()){
        target->draw(*getVehicle());
    }

    // Draw the player
    target->draw(*this);
}
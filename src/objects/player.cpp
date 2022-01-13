#include "player.h"
using namespace Objects;

// Constructors
Player::Player(float x, float y){
    // Initialize the player object
    setPosition(x, y);
    setSize(sf::Vector2f(20, 40));
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color::Cyan);

    texture = new sf::Texture();
    texture->loadFromFile("./res/textures/player.png");
    setTexture(texture);

    velocity = sf::Vector2f(0, 0);
    rotVelocity = 0.f;
    vehicle = nullptr;
}

Player::~Player(){
    delete texture;
}

// Private functions
void Player::updateKeys(float deltaTime){    
    // Controls
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        rotVelocity += -5 * deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        rotVelocity += 5 * deltaTime;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        velocity.x += 5 * deltaTime * std::sin(getRotation() * (3.1415 / 180));
        velocity.y -= 5 * deltaTime * std::cos(getRotation() * (3.1415 / 180));
    }
}

void Player::updateTransform(){
    // Update the position and rotation
    setPosition(getPosition() + velocity);
    setRotation(getRotation() + rotVelocity);
}

// Vehicle functions
void Player::drive(Vehicles::Vehicle* v){ vehicle = v; }
bool Player::isDriving(){ return (vehicle != nullptr); }
Vehicles::Vehicle* Player::getVehicle(){ return vehicle; }

// Physics functions
void Player::update(float deltaTime){
    // Parent the vehicle to the player if driving
    if(isDriving() && false){
        Vehicles::Vehicle* v = getVehicle();

        v->update(deltaTime);
        setPosition(v->getPosition());
        setRotation(v->getRotation());
    }

    updateKeys(deltaTime);
    updateTransform();
}

// Rendering functions
void Player::render(sf::RenderTarget* target){
    // Draw the player
    target->draw(*this);

    // Draw ship if the player is in one
    if(isDriving() && false){
        target->draw(*getVehicle());
    }
}
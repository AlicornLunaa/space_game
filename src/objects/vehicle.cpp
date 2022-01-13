#include "vehicle.h"
using namespace Vehicles;

// Base class
Vehicle::Vehicle(unsigned int width, unsigned int height, unsigned int scale){
    // Initialize a ship
    setOrigin(sf::Vector2f(width, height) * (float)scale * 0.5f);
    setSize(sf::Vector2f(width, height) * (float)scale);

    body.create(width, height, sf::Color::White);

    updateTexture();
    setTexture(&texture);
    setTextureRect(sf::IntRect(0, 0, width, height));
}

Vehicle::~Vehicle(){}

void Vehicle::updateTexture(){
    // Reset the texture
    texture.loadFromImage(body);
}


// Ship vehicle
Ship::Ship(unsigned int width, unsigned int height, unsigned int scale) : Vehicle(width, height, scale){
    // Initialize a ship
}

Ship::~Ship(){}

void Ship::update(float deltaTime){
    // Update physics
    
}
#include "ship.h"
using namespace Objects;

Ship::Ship(unsigned int width, unsigned int height, unsigned int scale){
    // Initialize a ship
    setOrigin(sf::Vector2f(width, height) * (float)scale * 0.5f);
    setSize(sf::Vector2f(width, height) * (float)scale);

    body.create(width, height, sf::Color::White);

    updateTexture();
    setTexture(&texture);
    setTextureRect(sf::IntRect(0, 0, width, height));
}

Ship::~Ship(){}

void Ship::updateTexture(){
    // Reset the texture
    texture.loadFromImage(body);
}

void Ship::update(float deltaTime){
    // Update physics

}
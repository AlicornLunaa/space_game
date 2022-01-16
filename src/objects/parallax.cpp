#include "parallax.h"
using namespace Objects;

Parallax::Parallax(unsigned int width, unsigned int height, float scale){ create(width, height, scale); }
Parallax::Parallax(){}
Parallax::~Parallax(){}

void Parallax::create(unsigned int width, unsigned int height, float scale){
    // Load texture
    tex.loadFromFile("./res/textures/skybox.png");
    tex.setRepeated(true);

    // Initialize shape
    setPosition(0, 0);
    setSize(sf::Vector2f(width, height));
    setTexture(&tex);
    setTextureRect(sf::IntRect(0, 0, 1280, 720));

    // Initialize effect
    pos = sf::Vector2f(0, 0);
    this->scale = scale;
}

void Parallax::setCameraPosition(sf::Vector2f p){
    // Save position
    pos = p;

    // Set the rectangle for the parallax
    setTextureRect(sf::IntRect((sf::Vector2i)(pos / scale), (sf::Vector2i)tex.getSize()));
}

void Parallax::setScale(float s){ scale = s; }
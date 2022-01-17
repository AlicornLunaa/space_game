#include "renderer.h"
using namespace Interface;

Renderer::Renderer(){}
Renderer::~Renderer(){}

sf::RenderWindow* Renderer::window = nullptr;
sf::Font Renderer::font;
sf::Text Renderer::text;
sf::RectangleShape Renderer::rect;
sf::CircleShape Renderer::circ;

void Renderer::init(sf::RenderWindow* window){
    // Load fonts and objects
    Renderer::window = window;

    font.loadFromFile("./res/fonts/arial.ttf");
    text.setFont(font);
}

void Renderer::drawText(float x, float y, std::string t, unsigned int fontSize, sf::Color textColor){
    // Make sure window exists
    assert(window != nullptr);

    // Set text properties
    text.setPosition(x, y);
    text.setCharacterSize(fontSize);
    text.setFillColor(textColor);
    text.setString(t);

    window->draw(text);
}

void Renderer::drawRect(float x, float y, float width, float height, sf::Color color, bool outline, float rotation){
    // Make sure window exists
    assert(window != nullptr);

    // Set rectangle properties
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(width, height));
    rect.setRotation(rotation);

    if(outline){
        rect.setOutlineColor(color);
        rect.setOutlineThickness(1.f);
        rect.setFillColor(sf::Color(0, 0, 0, 0));
    } else {
        rect.setOutlineThickness(0.f);
        rect.setFillColor(color);
    }

    window->draw(rect);
}

void Renderer::drawLine(float x1, float y1, float x2, float y2, sf::Color color){
    // Make sure window exists
    assert(window != nullptr);

    // Get line properties
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x1, y1), color),
        sf::Vertex(sf::Vector2f(x2, y2), color)
    };

    window->draw(line, 2, sf::Lines);
}

void Renderer::drawLine(sf::Vector2f v1, sf::Vector2f v2, sf::Color color){
    drawLine(v1.x, v1.y, v2.x, v2.y, color);
}

void Renderer::drawPoint(float x, float y, float radius, sf::Color color){
    // Make sure window exists
    assert(window != nullptr);

    // Get circle propertires
    circ.setRadius(radius);
    circ.setOrigin(radius, radius);
    circ.setPosition(x, y);
    circ.setFillColor(color);

    window->draw(circ);
}

void Renderer::drawPoint(sf::Vector2f v, float radius, sf::Color color){
    drawPoint(v.x, v.y, radius, color);
}
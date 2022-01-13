#include "debugger.h"

Debug::Debug(){}
Debug::~Debug(){}

sf::RenderWindow* Debug::window = nullptr;
sf::Font Debug::debugFont;
sf::Text Debug::debugText;
sf::RectangleShape Debug::debugRect;

void Debug::init(sf::RenderWindow* window){
    // Load fonts and objects
    std::cout << "Initialized debugger\n"; 

    Debug::window = window;

    debugFont.loadFromFile("./res/fonts/arial.ttf");
    debugText.setFont(debugFont);
}

void Debug::drawText(float x, float y, std::string text, unsigned int fontSize, sf::Color textColor){
    // Make sure window exists
    assert(window != nullptr);

    // Set text properties
    debugText.setPosition(x, y);
    debugText.setCharacterSize(fontSize);
    debugText.setFillColor(textColor);
    debugText.setString(text);

    window->draw(debugText);
}

void Debug::drawRect(float x, float y, float width, float height, sf::Color color){
    // Make sure window exists
    assert(window != nullptr);

    // Set rectangle properties
    debugRect.setPosition(x, y);
    debugRect.setSize(sf::Vector2f(width, height));
    debugRect.setFillColor(color);

    window->draw(debugRect);
}

void Debug::drawLine(float x1, float y1, float x2, float y2, sf::Color color){
    // Make sure window exists
    assert(window != nullptr);

    // Get line properties
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x1, y1), color),
        sf::Vertex(sf::Vector2f(x2, y2), color)
    };

    window->draw(line, 2, sf::Lines);
}
/**
 * @file renderer.h
 * @author Luna
 * @brief This class holds functions to draw things to the screen without constructing SFML objects every time
 */
#pragma once
#include <cmath>
#include <assert.h>
#include <SFML/Graphics.hpp>
#include "../util/vec_math.h"

namespace Interface {
    
    class Renderer {
    private:
        // Private constructors to stop construction
        Renderer();
        ~Renderer();
        
        // Variables
        static sf::RenderWindow* window;
        static sf::Font font;
        static sf::Text text;
        static sf::RectangleShape rect;
        static sf::CircleShape circ;

    public:
        /**
         * @brief Initializes the debugger with objects
         */
        static void init(sf::RenderWindow* window);

        /**
         * @brief Draws text to the given position using the given style
         * 
         * @param x X position of the text bound to the top-left
         * @param y Y position of the text bound to the top-right
         * @param t The text being drawn to the screen
         * @param fontSize The size of the text in font points
         * @param textColor The color of the text to use
         */
        static void drawText(float x, float y, std::string t, unsigned int fontSize = 24u, sf::Color textColor = sf::Color::White);

        /**
         * @brief Draws a rectangle to the screen given the position and size
         * 
         * @param x X position of the rectangle bound to the top-left
         * @param y Y position of the rectangle bound to the top-right
         * @param width The width of the rectangle being drawn
         * @param height The height of the rectangle being drawn
         * @param color The color to draw the rectangle with
         */
        static void drawRect(float x, float y, float width, float height, sf::Color color = sf::Color::White, bool outline = false, float rotation = 0.f);

        /**
         * @brief Draws a line from point A to point B using the given color
         * 
         * @param x1 X position of the first point of the line
         * @param y1 Y position of the first point of the line
         * @param x2 X position of the second point of the line
         * @param y2 Y Position of the second point of the line
         * @param color The color to draw the line with
         */
        static void drawLine(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::White);
        static void drawLine(sf::Vector2f v1, sf::Vector2f v2, sf::Color color = sf::Color::White);

        /**
         * @brief Draws a point at the position given
         * 
         * @param x X position of the point
         * @param y Y position of the point
         * @param radius The radius of the point to draw
         * @param color The color of the point
         */
        static void drawPoint(float x, float y, float radius = 2.f, sf::Color color = sf::Color::White);
        static void drawPoint(sf::Vector2f v, float radius = 2.f, sf::Color color = sf::Color::White);

        /**
         * @brief Draws a plane across the screen
         * 
         * @param point 
         * @param direction 
         * @param color 
         */
        static void drawPlane(sf::Vector2f point, sf::Vector2f direction, sf::Color color = sf::Color::White);
    };

};
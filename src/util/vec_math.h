/**
 * @file vec_math.h
 * @author Luna
 * @brief Mathematical functions for sfml vectors
 */
#pragma once
#include <cmath>
#include <SFML/System.hpp>

namespace Math {

    /**
     * @brief Returns the dot product of v1 and v2
     * @param v1 
     * @param v2 
     * @return float 
     */
    float dot(sf::Vector2f v1, sf::Vector2f v2);

    /**
     * @brief Returns the perpendicular vector to v
     * @param v 
     * @return sf::Vector2f 
     */
    sf::Vector2f perpendicular(sf::Vector2f v);

    /**
     * @brief Returns a magnitude for the vector v
     * @param v 
     * @return float 
     */
    float magnitude(sf::Vector2f v);

    /**
     * @brief Returns normalized version of the vector v
     * @param v 
     * @return sf::Vector2f 
     */
    sf::Vector2f normalize(sf::Vector2f v);

    /**
     * @brief Returns a vector reflected over the given vector
     * @param v Vector getting reflected
     * @param n Normal
     * @return sf::Vector2f 
     */
    sf::Vector2f reflect(sf::Vector2f v, sf::Vector2f n);

    /**
     * @brief Returns the distance of v1 to v2 squared
     * @param v1 
     * @param v2 
     * @return float 
     */
    float distanceSquare(sf::Vector2f v1, sf::Vector2f v2);
    float distance(sf::Vector2f v1, sf::Vector2f v2);

    /**
     * @brief Calculates the cross product
     * @param v1 
     * @param v2 
     * @return sf::Vector3f 
     */
    sf::Vector3f cross(sf::Vector2f v1, sf::Vector2f v2);
    sf::Vector2f cross(float a, sf::Vector2f v);
    
    /**
     * @brief Returns true or false depending on if the point is behind the line
     * @param edge The slope
     * @param point Point to test
     * @param flip Reverse slope
     * @return bool
     */
    bool clipPoint(sf::Vector2f edgeStart, sf::Vector2f edgeEnd, sf::Vector2f point, bool flip);
}
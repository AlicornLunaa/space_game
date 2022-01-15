/**
 * @file vec_math.h
 * @author Luna
 * @brief Mathematical functions for sfml vectors
 */
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

}
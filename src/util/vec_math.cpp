#include "vec_math.h"

float Math::dot(sf::Vector2f v1, sf::Vector2f v2){
    return v1.x * v2.x + v1.y * v2.y;
}

sf::Vector2f Math::perpendicular(sf::Vector2f v){
    return sf::Vector2f(v.y, -v.x);
}

float Math::magnitude(sf::Vector2f v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Math::normalize(sf::Vector2f v){
    return v / magnitude(v);
}
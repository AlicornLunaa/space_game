#include "vec_math.h"

float Math::dot(sf::Vector2f v1, sf::Vector2f v2){
    return v1.x * v2.x + v1.y * v2.y;
}

sf::Vector2f Math::perpendicular(sf::Vector2f v){
    return sf::Vector2f(v.y, v.x) * -1.f;
}
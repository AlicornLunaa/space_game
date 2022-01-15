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

sf::Vector2f Math::reflect(sf::Vector2f v, sf::Vector2f n){
    sf::Vector2f norm = normalize(n);
    sf::Vector2f out = 2 * dot(v, norm) * norm - v;
    return out;
}
#include "vec_math.h"

float Math::dot(sf::Vector2f v1, sf::Vector2f v2){
    return v1.x * v2.x + v1.y * v2.y;
}

sf::Vector2f Math::perpendicular(sf::Vector2f v){
    return sf::Vector2f(-v.y, v.x);
}

float Math::magnitude(sf::Vector2f v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Math::normalize(sf::Vector2f v){
    float mag = magnitude(v);
    return (mag == 0) ? sf::Vector2f(0.f, 0.f) : v / mag;
}

sf::Vector2f Math::reflect(sf::Vector2f v, sf::Vector2f n){
    sf::Vector2f norm = normalize(n);
    sf::Vector2f out = 2 * dot(v, norm) * norm - v;

    // Error checking
    if(norm.x == 0 && norm.y == 0){
        out = v;
    } else if(std::abs(norm.x) == 1){
        out = v;
        out.x *= -1.f;
    } else if(std::abs(norm.y) == 1){
        out = v;
        out.y *= -1.f;
    }

    return out;
}

float Math::distanceSquare(sf::Vector2f v1, sf::Vector2f v2){
    sf::Vector2f v = v2 - v1;
    return v.x * v.x + v.y * v.y;
}

float Math::distance(sf::Vector2f v1, sf::Vector2f v2){
    return std::sqrt(distanceSquare(v1, v2));
}

sf::Vector3f Math::cross(sf::Vector2f v1, sf::Vector2f v2){
    return sf::Vector3f(0, 0, v1.x * v2.y - v1.y * v2.x);
}

sf::Vector2f Math::cross(float a, sf::Vector2f v){
    return { -a * v.y, a * v.x };
}

bool Math::clipPoint(sf::Vector2f edgeStart, sf::Vector2f edgeEnd, sf::Vector2f point, bool flip){
    // Clips a point along an edge
    float slope = (edgeEnd.y - edgeStart.y) / (edgeEnd.x - edgeStart.x);
    float product = (slope * (point.x - edgeStart.x)) + edgeStart.y;
    return (flip ? (product - point.y) <= 0 : (product - point.y) >= 0);
}
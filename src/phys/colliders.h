/**
 * @file collider.h
 * @author Luna
 * @brief A collider contains the data for the collision bounds
 */
#pragma once
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>

namespace Phys {

    class Collider : public sf::Transformable {
    private:
        // Variables
        std::vector<sf::Vector2f> points;

    public:
        // Constructors
        Collider(float x, float y, float rotation);
        Collider();
        ~Collider();
        void create(float x, float y, float rotation);
        
        // Functions
        void addPoint(float x, float y);
        void clearPoints();
        sf::Vector2f getPoint(unsigned int i);
        unsigned int getPointCount();
        sf::Vector2f getUp();
        sf::Vector2f getRight();

        // Static functions
        static Collider createBoxCollider(float x, float y, float w, float h, float rotation = 0.f);
    };

};
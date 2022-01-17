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
        std::vector<sf::Vector2f> points;
        bool sleeping;
        bool enabled;
        bool mStatic;

    public:
        Collider(float x, float y, float rotation);
        ~Collider();
        
        bool isSleeping();
        void setSleeping(bool s);
        bool isEnabled();
        void setEnabled(bool e);
        bool isStatic();
        void setStatic(bool s);
        void addPoint(float x, float y);
        void clearPoints();
        sf::Vector2f getPoint(unsigned int i);
        sf::Vector2f getPointGlobal(unsigned int i);
        unsigned int getPointCount();
        sf::Vector2f getUp();
        sf::Vector2f getRight();
    };

    class BoxCollider : public Collider {
    public:
        BoxCollider(float x, float y, float w, float h, float rotation = 0.f);
    };

    struct CollisionData {
        Collider* c1;
        Collider* c2;
        sf::Vector2f normal;
        float displacement;
        unsigned int contactPoint;
    };

};
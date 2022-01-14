/**
 * @file collider.h
 * @author Luna
 * @brief A collider contains the data for the collision bounds
 */
#include <vector>
#include <SFML/System.hpp>

namespace Phys {

    class Collider {
    private:
        std::vector<sf::Vector2f> points;
        sf::Vector2f position;
        float rotation;

    public:
        Collider(float x, float y, float rotation);
        ~Collider();

        void addPoint(sf::Vector2f v);
        sf::Vector2f getPoint(unsigned int i);
        unsigned int getPointCount();
    };

    struct CollisionData {
        Collider* c1;
        Collider* c2;
        sf::Vector2f normal;
    };

};
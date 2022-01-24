#include "planet.h"
#include "../util/earcut.hpp"
using namespace Objects;

// Functions for when on planet


// Functions for when in space
void Planet::reloadTexture(){
    planetTexture.loadFromImage(planetData);
    setTexture(&planetTexture);
}

void Planet::calculateMesh(){
    // Get data
    sf::Vector2u size = planetData.getSize();
    std::vector<sf::Vector2f> points;

    // Start marching squares from top-left down iteration path
    for(unsigned int x = 0; x < size.x; x++){
        for(unsigned int y = 0; y < size.y; y++){
            // Get current pixel
            sf::Color pixel = planetData.getPixel(x, y);

            // Check neighbors
            unsigned int threshold = 0;
            unsigned int state = 0;
            unsigned int sides = 0;
            bool edge = ((x == 0) || (x == size.x - 1) || (y == 0) || (y == size.y - 1));
            bool left = edge || (planetData.getPixel(x - 1, y).a > threshold);
            bool right = edge || (planetData.getPixel(x + 1, y).a > threshold);
            bool top = edge || (planetData.getPixel(x, y - 1).a > threshold);
            bool bottom = edge || (planetData.getPixel(x, y + 1).a > threshold);
            bool selfValid = pixel.a > threshold;

            if(top){
                // 0 and 1 get activated
                state |= (1UL << 0);
                state |= (1UL << 1);
                sides++;
            }

            if(right){
                // 1 and 2 get activated
                state |= (1UL << 1);
                state |= (1UL << 2);
                sides++;
            }

            if(bottom){
                // 2 and 3 get activated
                state |= (1UL << 2);
                state |= (1UL << 3);
                sides++;
            }

            if(left){
                // 3 and 0 get activated
                state |= (1UL << 3);
                state |= (1UL << 0);
                sides++;
            }

            // Outline the shape if its not surrounded on every side and has at least one neighbor
            if(selfValid && sides <= 3){
                points.push_back(sf::Vector2f(x + 0.5f, y + 0.5f));
            }
        }
    }

    cleanupMesh(points);
    convertToCollider(points);
}

void Planet::convertToCollider(std::vector<sf::Vector2f>& points){
    // Convert to triangles to use with the collision detection
    std::vector<std::vector<std::array<float, 2>>> polygon;
    polygon.push_back({}); // First array is the polygon

    for(sf::Vector2f p : points){
        polygon.back().push_back({ p.x, p.y });
    }

    // Remove original colliders
    sf::Vector2f size = (sf::Vector2f)planetTexture.getSize();
    rigidbody->clearColliders();

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    for(unsigned int i = 0; i < indices.size(); i += 3){
        sf::Vector2f p1 = points[indices[i]];
        sf::Vector2f p2 = points[indices[i + 1]];
        sf::Vector2f p3 = points[indices[i + 2]];

        // Get mesh data
        sf::Vector2f midPoint = (p1 + p2 + p3) / 3.f;
        p1 -= midPoint;
        p2 -= midPoint;
        p3 -= midPoint;
        
        // Add new collider for each triangle
        Phys::Collider c(midPoint.x, midPoint.y, 0.f);
        c.setScale(size);
        c.addPoint(p1.x / size.x - 0.5f, p1.y / size.y - 0.5f);
        c.addPoint(p2.x / size.x - 0.5f, p2.y / size.y - 0.5f);
        c.addPoint(p3.x / size.x - 0.5f, p3.y / size.y - 0.5f);
        rigidbody->addCollider(c);
    }
}

void Planet::cleanupMesh(std::vector<sf::Vector2f>& points){
    // Run nearest neighbor algorithm
    if(points.size() == 0) return;
    sf::Vector2 start = points[0];
    sf::Vector2f current = start;
    unsigned int index = 1;
    int nextIndex = -1;
    float lastDist = INFINITY;
    sf::Vector2f next = points[1];
    std::vector<sf::Vector2f> hull = { start };

    while(points.size() > 0){
        // Find closest
        sf::Vector2f checking = points[index];
        float dist = Math::distanceSquare(current, checking);

        // Point is closer, save it
        if(dist < lastDist){
            next = checking;
            nextIndex = index;
            lastDist = dist;
        }
        
        index = (index + 1) % points.size();

        if(index == 0){
            // Exit condition
            if(next == start){
                break;
            }

            // Add the closest to the hull
            hull.push_back(next);
            current = next;
            index = 1;
            points.erase(points.begin() + nextIndex);
            next = start;
            lastDist = INFINITY;
        }
    }

    // Remove colinear points
    for(unsigned int i = 0; i < hull.size(); i++){
        // Get slopes
        sf::Vector2f start = hull[i];
        sf::Vector2f mid = hull[(i + 1) % hull.size()];
        sf::Vector2f end = hull[(i + 2) % hull.size()];

        if((start.x - mid.x) * (start.y - mid.y) == (mid.x - end.x) * (mid.y - end.y)){
            hull.erase(hull.begin() + ((i + 1) % hull.size()));
            i--;
        }
    }

    points = hull;
}

sf::Vector2f Planet::getCenter(){ return getPosition(); }

sf::Color Planet::getAtmosColor(){ return atmosphereColor; }

Phys::RigidBody* Planet::getRigidBody(){ return rigidbody; }

void Planet::update(float deltaTime){
    // Update everything on the planet
    setPosition(rigidbody->getPosition());
    setRotation(rigidbody->getRotation());
}

// Constructors
Planet::Planet(Phys::Engine& engine, float x, float y, float scale, unsigned int radius){ create(engine, x, y, scale, radius); }
Planet::Planet(Phys::Engine& engine, float x, float y, float scale, std::string path){ create(engine, x, y, scale, path); }
Planet::Planet(){}
Planet::~Planet(){}

void Planet::create(Phys::Engine& engine, float x, float y, float scale, unsigned int radius){
    // Create a default planet with the given radius
    planetData.create(radius * 2, radius * 2, sf::Color(67, 220, 67));

    for(int mx = 0; mx < (int)radius * 2; mx++) for(int my = 0; my < (int)radius * 2; my++){
        sf::Vector2f v(mx - (int)radius, my - (int)radius);
        
        if(v.x * v.x + v.y * v.y > (int)(radius * radius)){
            planetData.setPixel(mx, my, sf::Color(0, 0, 0, 0));
        }
    }

    reloadTexture();

    setPosition(x, y);
    setScale(scale, scale);
    setSize((sf::Vector2f)planetData.getSize());
    setOrigin(getSize() * 0.5f);

    atmosphereColor = sf::Color(135 * 0.9f, 206 * 0.9f, 235 * 0.9f);

    rigidbody = new Phys::RigidBody(x, y, 0.f);
    rigidbody->setScale(getScale());
    rigidbody->mStatic = true;
    rigidbody->mass = 10000000.f;
    rigidbody->inertia = 1000000.f;
    calculateMesh();

    engine.registerBody(rigidbody);
}

void Planet::create(Phys::Engine& engine, float x, float y, float scale, std::string path){
    // Create a planet from the given texture
    create(engine, x, y, scale, 64.f);
    planetData.loadFromFile(path);
    reloadTexture();
}

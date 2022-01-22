#include "planet.h"
#include "../util/earcut.hpp"
using namespace Objects;

void Planet::reloadTexture(){
    tex.loadFromImage(planetData);
    setTexture(&tex);
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
            }

            if(right){
                // 1 and 2 get activated
                state |= (1UL << 1);
                state |= (1UL << 2);
            }

            if(bottom){
                // 2 and 3 get activated
                state |= (1UL << 2);
                state |= (1UL << 3);
            }

            if(left){
                // 3 and 0 get activated
                state |= (1UL << 3);
                state |= (1UL << 0);
            }

            // Outline the shape if its not surrounded on every side and has at least one neighbor
            if(selfValid && state != 15 && state != 0){
                points.push_back(sf::Vector2f(x + 0.5f, y + 0.5f));
            }
        }
    }
    
    // Draw shape
    for(unsigned int i = 0; i < points.size(); i++){
        sf::Vector2f p1 = points[i];
        Interface::Renderer::drawPoint(p1 * getScale().x + getPosition(), 1.f, sf::Color::Yellow);
    }

    cleanupMesh(points);
    //convertMesh(points);
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
    rigidbody->clearColliders();

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    for(unsigned int i = 0; i < indices.size(); i += 3){
        sf::Vector2f p1 = points[indices[i]];
        sf::Vector2f p2 = points[indices[i + 1]];
        sf::Vector2f p3 = points[indices[i + 2]];
        
        // Add new collider for each triangle
        Phys::Collider c;
        c.addPoint(p1.x, p1.y);
        c.addPoint(p2.x, p2.y);
        c.addPoint(p3.x, p3.y);
        rigidbody->addCollider(c);
    }
}

void Planet::convertMesh(std::vector<sf::Vector2f>& points){
    // Run a jarvis march to get the hull
    sf::Vector2f leftMost = points[0];
    sf::Vector2f currentVertex = leftMost;
    unsigned int startIndex = 2;
    unsigned int index = 2;
    int nextIndex = -1;
    sf::Vector2f nextVertex = points[1];
    std::vector<sf::Vector2f> hull = { currentVertex };
    
    // Find the leftmost point
    for(unsigned int i = 1; i < points.size(); i++){
        if(points[i].x < leftMost.x){
            // Closer
            leftMost = points[i];
            currentVertex = leftMost;
            startIndex = (i + 2) % points.size();
            index = startIndex;
            nextVertex = points[(i + 1) % points.size()];
        }
    }

    // Start the march
    while(true){
        sf::Vector2f checking = points[index];
        float crossProduct = Math::cross(nextVertex - currentVertex, checking - currentVertex).z;

        if(crossProduct < 0){
            nextVertex = checking;
            nextIndex = index;
        }
        
        index = (index + 1) % points.size();

        if(index == startIndex){
            if(nextVertex == leftMost){
                break;
            }

            hull.push_back(nextVertex);
            currentVertex = nextVertex;
            index = startIndex;
            points.erase(points.begin() + nextIndex);
            nextVertex = leftMost;
        }
    }

    // Draw hull
    for(unsigned int i = 0; i < hull.size(); i++){
        sf::Vector2f p1 = hull[i];
        sf::Vector2f p2 = hull[(i + 1) % hull.size()];
        Interface::Renderer::drawLine(getPosition() + p1 * getScale().x, getPosition() + p2 * getScale().x, sf::Color::Yellow);
    }

    points = hull;

    // Create collision mesh
    rigidbody->clearColliders();
    Phys::Collider c;
    for(sf::Vector2f p1 : points){
        c.addPoint(p1.x, p1.y);
    }
    rigidbody->addCollider(c);
}

void Planet::cleanupMesh(std::vector<sf::Vector2f>& points){
    // Run nearest neighbor algorithm
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

    // Draw hull
    for(unsigned int i = 0; i < hull.size(); i++){
        sf::Vector2f p1 = hull[i];
        Interface::Renderer::drawPoint(p1 * getScale().x + getPosition(), 2.f, sf::Color::Red);
    }

    points = hull;
}

Planet::Planet(Phys::Engine& engine, float x, float y, float scale, unsigned int radius){ create(engine, x, y, scale, radius); }
Planet::Planet(Phys::Engine& engine, float x, float y, float scale, std::string path){ create(engine, x, y, scale, path); }
Planet::Planet(){}
Planet::~Planet(){}

void Planet::create(Phys::Engine& engine, float x, float y, float scale, unsigned int radius){
    // Create a default planet with the given radius
    planetData.create(radius * 2, radius * 2, sf::Color(0, 0, 0, 0));
    reloadTexture();

    setPosition(x, y);
    setScale(scale, scale);
    setSize(sf::Vector2f(radius, radius) * 2.f);

    rigidbody = new Phys::RigidBody(x, y, 0.f);
    rigidbody->setScale(getScale());
    rigidbody->mStatic = true;
    calculateMesh();

    engine.registerBody(rigidbody);
}

void Planet::create(Phys::Engine& engine, float x, float y, float scale, std::string path){
    // Create a planet from the given texture
    planetData.loadFromFile(path);
    reloadTexture();

    setPosition(x, y);
    setScale(scale, scale);
    setSize((sf::Vector2f)planetData.getSize());

    rigidbody = new Phys::RigidBody(x, y, 0.f);
    rigidbody->setScale(getScale());
    rigidbody->mStatic = true;
    calculateMesh();
    
    engine.registerBody(rigidbody);
}

sf::Vector2f Planet::getCenter(){
    return getPosition() + getSize() * (getScale().x / 2);
}

sf::Color Planet::getPixel(float x, float y){
    // Convert given position to image-local
    sf::Vector2i pos = (sf::Vector2i)getInverseTransform().transformPoint(x, y);

    // Bounds check it
    if(pos.x < 0 || pos.x > getSize().x || pos.y < 0 || pos.y > getSize().y) return sf::Color(0, 0, 0, 0);

    // Return the color
    return planetData.getPixel(pos.x, pos.y);
}

void Planet::setPixel(float x, float y, sf::Color color){
    // Convert given position to image-local
    sf::Vector2i pos = (sf::Vector2i)getInverseTransform().transformPoint(x, y);

    // Bounds check it
    if(pos.x < 0 || pos.x > getSize().x || pos.y < 0 || pos.y > getSize().y) return;

    // Select it
    planetData.setPixel(pos.x, pos.y, color);
    reloadTexture();
    calculateMesh();
}

void Planet::update(float deltaTime){
    // Update everything on the planet
    calculateMesh();
    setPosition(rigidbody->getPosition());
    setRotation(rigidbody->getRotation());
}
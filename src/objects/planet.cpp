#include "planet.h"
using namespace Objects;

void Planet::reloadTexture(){
    tex.loadFromImage(planetData);
    setTexture(&tex);
}

void Planet::calculateMesh(){
    /* 1         2
        *-------*
        |       |
        |       |
        *-------*
       8         4
    */
    // Get data
    sf::Vector2u size = planetData.getSize();
    sf::Vector2f parentTopLeft = getPosition();
    std::vector<sf::Vector2f> points;
    std::vector<double> pointsRaw;

    // Start marching squares from top-left down iteration path
    for(unsigned int x = 0; x < size.x; x++){
        for(unsigned int y = 0; y < size.y; y++){
            // Get current pixel
            sf::Color pixel = planetData.getPixel(x, y);

            // Check neighbors
            unsigned int threshold = 0;
            unsigned int state = 0;
            bool edge = (x == 0) || (x == size.x - 1) || (y == 0) || (y == size.y - 1) || (pixel.a <= threshold);
            bool left = edge || (planetData.getPixel(x - 1, y).a <= threshold);
            bool right = edge || (planetData.getPixel(x + 1, y).a <= threshold);
            bool top = edge || (planetData.getPixel(x, y - 1).a <= threshold);
            bool bottom = edge || (planetData.getPixel(x, y + 1).a <= threshold);
            bool topLeft = edge || (planetData.getPixel(x - 1, y - 1).a <= threshold);
            bool topRight = edge || (planetData.getPixel(x + 1, y - 1).a <= threshold);
            bool bottomLeft = edge || (planetData.getPixel(x - 1, y + 1).a <= threshold);
            bool bottomRight = edge || (planetData.getPixel(x + 1, y + 1).a <= threshold);

            if(top){
                // 0 and 1 get activated
                state |= (1UL << 0);
                state |= (1UL << 1);
            } else {
                if(topLeft){ state |= (1UL << 0); }
                if(topRight){ state |= (1UL << 1); }
            }

            if(right){
                // 1 and 2 get activated
                state |= (1UL << 1);
                state |= (1UL << 2);
            } else {
                if(topRight){ state |= (1UL << 1); }
                if(bottomRight){ state |= (1UL << 2); }
            }

            if(bottom){
                // 2 and 3 get activated
                state |= (1UL << 2);
                state |= (1UL << 3);
            } else {
                if(bottomRight){ state |= (1UL << 2); }
                if(bottomLeft){ state |= (1UL << 3); }
            }

            if(left){
                // 3 and 0 get activated
                state |= (1UL << 3);
                state |= (1UL << 0);
            } else {
                if(bottomLeft){ state |= (1UL << 3); }
                if(topLeft){ state |= (1UL << 0); }
            }

            // Outline the shape if its not surrounded on every side and has at least one neighbor
            switch(state){
            case 0: // None
                break;
            case 1: // Top left
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, y));
                points.push_back(sf::Vector2f(x, (y + y - 1) / 2.f));
                break;
            case 2: // Top right
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, y));
                points.push_back(sf::Vector2f(x, (y + y - 1) / 2.f));
                break;
            case 3: // Top
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, y));
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, y));
                break;
            case 4: // Bottom right
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, y));
                points.push_back(sf::Vector2f(x, (y + y + 1) / 2.f));
                break;
            case 5: // Double backslash
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, (y + y - 1) / 2.f));
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, (y + y + 1) / 2.f));
                break;
            case 6: // Right
                points.push_back(sf::Vector2f(x, (y + y - 1) / 2.f));
                points.push_back(sf::Vector2f(x, (y + y + 1) / 2.f));
                break;
            case 7: // Bottom backslash
                points.push_back(sf::Vector2f(x, (y + y + 1) / 2.f));
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, y));
                break;
            case 8: // Bottom left
                points.push_back(sf::Vector2f(x, (y + y + 1) / 2.f));
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, y));
                break;
            case 9: // Left
                points.push_back(sf::Vector2f(x, (y + y - 1) / 2.f));
                points.push_back(sf::Vector2f(x, (y + y + 1) / 2.f));
                break;
            case 10: // Double forwardslash
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, (y + y + 1) / 2.f));
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, (y + y - 1) / 2.f));
                break;
            case 11: // Bottom forwardslash
                points.push_back(sf::Vector2f(x, (y + y + 1) / 2.f));
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, y));
                break;
            case 12: // Bottom
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, y));
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, y));
                break;
            case 13: // Top backslash
                points.push_back(sf::Vector2f((x + x + 1) / 2.f, y));
                points.push_back(sf::Vector2f(x, (y + y - 1) / 2.f));
                break;
            case 14: // Top forwardslash
                points.push_back(sf::Vector2f((x + x - 1) / 2.f, y));
                points.push_back(sf::Vector2f(x, (y + y - 1) / 2.f));
                break;
            case 15: // Filled
                break;
            }
        }
    }


    cleanupMesh(points);
    // Outline drawing for testing
    for(unsigned i = 0; i < points.size(); i++){
        sf::Vector2f p1 = points[i];
        sf::Vector2f p2 = points[(i + 1) % points.size()];
        Interface::Renderer::drawLine(parentTopLeft + p1 * getScale().x, parentTopLeft + p2 * getScale().y, sf::Color::Red);
    }
}

std::vector<sf::Vector2f> Planet::triangulate(std::vector<double>& points){
    // Convert to triangles to use with the collision detection
    std::vector<sf::Vector2f> out;
    delaunator::Delaunator d(points);

    for(unsigned int i = 0; i < d.triangles.size(); i++){
        // Convert data
        out.push_back(sf::Vector2f(d.coords[2 * d.triangles[i]], d.coords[2 * d.triangles[i] + 1]));
        out.push_back(sf::Vector2f(d.coords[2 * d.triangles[i + 1]], d.coords[2 * d.triangles[i + 1] + 1]));
        out.push_back(sf::Vector2f(d.coords[2 * d.triangles[i + 2]], d.coords[2 * d.triangles[i + 2] + 1]));

        // Draw lines
        Interface::Renderer::drawLine(out[i * 3], out[i * 3 + 1]);
        Interface::Renderer::drawLine(out[i * 3 + 1], out[i * 3 + 2]);
        Interface::Renderer::drawLine(out[i * 3 + 2], out[i * 3]);
    }

    return out;
}

void Planet::cleanupMesh(std::vector<sf::Vector2f>& points){
    // Remove duplicate points
    int current = points.size() - 1;
    while(current >= 0){
        for(unsigned int i = 0; i < points.size(); i++){
            if(points[i] == points[current]){
                points.erase(points.begin() + i);

                i--;
                current--;
            }
        }

        current--;
    }

    // Convert into one line
    std::vector<sf::Vector2f> hull = { points[0] };
    unsigned int index = 0;

    // Loop through every line segment
    while(points.size() != 0){
        sf::Vector2f p1 = points[index];
        float minDistance = INFINITY;

        // Find the other line segment that starts the closest to the first lines end
        for(unsigned int j = 0; j < points.size(); j += 2){
            sf::Vector2f p2 = points[j];

            // Find next link
            float currentDistance = Math::distanceSquare(p1, p2);
            if(currentDistance < minDistance && currentDistance != 0 && index != j && index + 1 != j){
                index = j;
                minDistance = currentDistance;
            }
            bool a = false;
        }

        // Add point
        hull.push_back(points[index]);
        points.erase(points.begin());
        

        // Exit condition
        if(index == 0)
            break;

        index--;
    }

    points = hull;
}

Planet::Planet(Phys::Engine& engine, float x, float y, unsigned int radius){ create(engine, x, y, radius); }
Planet::Planet(Phys::Engine& engine, float x, float y, std::string path){ create(engine, x, y, path); }
Planet::Planet(){}
Planet::~Planet(){}

void Planet::create(Phys::Engine& engine, float x, float y, unsigned int radius){
    // Create a default planet with the given radius
    planetData.create(radius * 2, radius * 2, sf::Color(0, 100, 0));
    reloadTexture();

    setPosition(x, y);
    setScale(4.f, 4.f);
    setSize(sf::Vector2f(radius, radius) * 2.f);

    rigidbody = new Phys::RigidBody(x, y, 0.f);
    rigidbody->setScale(getScale());
    calculateMesh();
    engine.registerCollider(rigidbody);
}

void Planet::create(Phys::Engine& engine, float x, float y, std::string path){
    // Create a planet from the given texture
    planetData.loadFromFile(path);
    reloadTexture();

    setPosition(x, y);
    setScale(4.f, 4.f);
    setSize((sf::Vector2f)planetData.getSize());

    rigidbody = new Phys::RigidBody(x, y, 0.f);
    rigidbody->setScale(getScale());
    calculateMesh();
    engine.registerCollider(rigidbody);
}

void Planet::update(float deltaTime){
    // Update everything on the planet
    calculateMesh();
    setPosition(rigidbody->getPosition());
    setRotation(rigidbody->getRotation());
}
/**
 * @file planet.h
 * @author Luna
 * @brief Controls the planet physics
 */
#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../interface/renderer.h"
#include "../phys/physics.h"

namespace Objects {

    class Planet : public sf::RectangleShape {
    private:
        // Members when on planet


        // Members when in space
        sf::Image planetData; // Run an algorithm to convert the blocks in the world to a texture
        sf::Texture planetTexture;
        sf::Color atmosphereColor; // Color of the atmospheric effect
        Phys::RigidBody* rigidbody;
        
        float atmosDensity;

        /** @brief Updates SFML texture based on the image data */
        void reloadTexture();

        /** @brief Runs through an algorithm to update the collision mesh for the texture */
        void calculateMesh();
        void convertToCollider(std::vector<sf::Vector2f>& points);
        void cleanupMesh(std::vector<sf::Vector2f>& points);

    public:
        // Constructors
        Planet(Phys::Engine& engine, float x, float y, float scale, unsigned int radius);
        Planet(Phys::Engine& engine, float x, float y, float scale, std::string path);
        Planet();
        ~Planet();

        /**
         * @brief Creates a new planet object
         * @param engine 
         * @param x 
         * @param y 
         * @param radius 
         */
        void create(Phys::Engine& engine, float x, float y, float scale, unsigned int radius);
        void create(Phys::Engine& engine, float x, float y, float scale, std::string path);

        // Members when on planet
        

        // Members when in space
        sf::Vector2f getCenter();
        sf::Color getAtmosColor();
        Phys::RigidBody* getRigidBody();
        void update(float deltaTime);
    };

};
/**
 * @file planet.h
 * @author Luna
 * @brief Controls different planet objects
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
        sf::Image planetData;
        sf::Texture tex;
        Phys::RigidBody* rigidbody;

        /**
         * @brief Updates SFML texture based on the image data
         */
        void reloadTexture();

        /**
         * @brief Runs through an algorithm to update the collision mesh for the texture
         */
        void calculateMesh();

        /**
         * @brief Converts the mesh into triangles
         * @param points 
         */
        void convertToCollider(std::vector<sf::Vector2f>& points);

        /**
         * @brief Convert to convex hull
         * @param points 
         */
        void convertMesh(std::vector<sf::Vector2f>& points);

        /**
         * @brief Cleans the mesh
         * @param points 
         */
        void cleanupMesh(std::vector<sf::Vector2f>& points);

    public:
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

        /**
         * @brief Creates a new planet object based on the image
         * @param engine 
         * @param x 
         * @param y 
         * @param path 
         */
        void create(Phys::Engine& engine, float x, float y, float scale, std::string path);

        /**
         * @brief Get the center of the planet
         * @return sf::Vector2f 
         */
        sf::Vector2f getCenter();

        /**
         * @brief Returns the pixel at the location given
         * @param x 
         * @param y 
         * @return sf::Color 
         */
        sf::Color getPixel(float x, float y);

        /**
         * @brief Sets the pixel at the position to a specific color
         * @param x 
         * @param y 
         * @param color 
         */
        void setPixel(float x, float y, sf::Color color);

        /**
         * @brief Updates the planet
         * @param deltaTime 
         */
        void update(float deltaTime);
    };

};
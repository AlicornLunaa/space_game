/**
 * @file panel.h
 * @author Luna
 * @brief Contains the different objects to draw
 */
#include <SFML/Graphics.hpp>
#include "renderer.h"

namespace Interface {

    class Panel : public sf::Drawable {
    private:
        float x;
        float y;
        float width;
        float height;

    public:
        Panel(float x, float y, float width, float height);
        ~Panel();

        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };

};
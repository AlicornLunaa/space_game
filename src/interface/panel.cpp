#include "panel.h"
using namespace Interface;

Panel::Panel(float x, float y, float width, float height){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}
Panel::~Panel(){}
void Panel::draw(sf::RenderTarget &target, sf::RenderStates states) const {}
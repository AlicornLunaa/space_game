#include <SFML/Graphics.hpp>

int main(int argc, char** argv){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Space Game");

    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            switch(e.type){
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            }
        }

        window.clear();

        window.display();
    }

    return 0;
}
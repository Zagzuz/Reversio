#include <SFML/Window.hpp>

#include "Hex.h"

int main()
{
    rev::Hex h(0, -1, 1), j(-3, 3, 0);
    rev::Hex::distance(h, j);
    
    sf::Window window(sf::VideoMode(800, 600), sf::String("Reversio"));
    sf::Event event;
    // while there are pending events...
    while (true)
    {
        window.pollEvent(event);
        // check the type of the event...
        switch (event.type)
        {
            // window closed
        case sf::Event::Closed:
            window.close();
            break;

            // key pressed
        case sf::Event::KeyPressed:
            break; 

            // we don't process other types of events
        default:
            break;
        }
    }
	return 0;
}

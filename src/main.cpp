#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_set>

#include "hex.h"
#include "layout.h"

int main()
{
    using namespace rev;

    sf::VideoMode video_mode(800, 600);
    sf::RenderWindow window(video_mode, "Reversio");
    sf::Event event;

    Point<int> layout_size(30, 30);
    Point<int> layout_origin(50, 50);
    Layout<int> layout(pointy_orientation(), layout_size, layout_origin);
    std::unordered_set<Hex, HexHash> hexes;
    for (int q = 0; q < 7; ++q)
    {
        for (int r = 0; r < 7; ++r)
        {
            if (q + r < 3 || q + r > 9) continue;
            hexes.emplace(q, r);
        }
    }

    while (window.isOpen())
    {
        window.pollEvent(event);
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            break; 

        default:
            break;
        }

        for (const Hex& hex : hexes)
        {
            for (auto&& corner : hex.polygon_corners(layout))
            {
                sf::Vertex point({ corner.x, corner.y }, sf::Color::White);
                window.draw(&point, 1, sf::Points);
            }
        }

        window.display();
    }
	return 0;
}

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

#include "hex.h"
#include "layout.h"

sf::ConvexShape create_polygon(const rev::Layout<rev::Hex::crd_t>& layout, const rev::Hex& hex)
{
    sf::ConvexShape polygon(6);
    auto corners = hex.polygon_corners(layout);
    for (decltype(corners)::size_type i = 0; i < corners.size(); ++i)
    {
        polygon.setPoint(i, sf::Vector2f(corners[i].x, corners[i].y));
    }
    polygon.setOutlineColor(sf::Color::Black);
    polygon.setOutlineThickness(1);
    return polygon;
}

int main()
{
    using namespace rev;

    sf::VideoMode video_mode(800, 600);
    sf::RenderWindow window(video_mode, "Reversio");
    window.setVerticalSyncEnabled(true);
    sf::Event event;

    Point<int> layout_size(20, 20);
    Point<int> layout_origin(0, 0);
    Layout<int> layout(pointy_orientation(), layout_size, layout_origin);

    std::unordered_map<Hex, sf::ConvexShape, HexHash> polygon_map;

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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            Hex hex = Hex::from_pixel(layout, { mouse_pos.x, mouse_pos.y });
            polygon_map.emplace(hex, create_polygon(layout, hex));
        }

        for (auto&& [hex, poly] : polygon_map)
        {
            window.draw(poly);
        }

        window.display();
    }

	return 0;
}

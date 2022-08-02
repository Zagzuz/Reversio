#include <SFML/Graphics.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "hex.h"
#include "layout.h"

template <class T>
sf::ConvexShape create_polygon(const rev::Layout& layout,
                               const rev::Hex<T>& hex,
                               const sf::Color& fill_color = sf::Color::White) {
  sf::ConvexShape polygon(6);
  auto corners = hex.polygon_corners(layout);
  for (decltype(corners)::size_type i = 0; i < corners.size(); ++i) {
    polygon.setPoint(i, sf::Vector2f(corners[i]));
  }
  polygon.setOutlineColor(sf::Color::Black);
  polygon.setFillColor(fill_color);
  polygon.setOutlineThickness(1);
  return polygon;
}

namespace logging = boost::log;

int main() {
  using namespace rev;

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);

  sf::VideoMode video_mode(600, 600);
  sf::RenderWindow window(video_mode, "Reversio");
  window.setFramerateLimit(60);
  sf::Event event;
  auto mouse_pos = [&window]() { return sf::Mouse::getPosition(window); };

  sf::Font arial_font;
  arial_font.loadFromFile("res/Hack-Regular.ttf");
  sf::Text text;
  text.setFont(arial_font);
  sf::Texture Y;
  Y.loadFromFile("res/Y.png");

  Point<unsigned int> layout_size(60, 60);
  Point<unsigned int> layout_origin(0, 0);
  Layout layout(pointy_orientation(), layout_size, layout_origin);

  using poly_map_t = IntHex;
  std::unordered_map<poly_map_t, sf::ConvexShape, HexHash<poly_map_t::crd_t>>
      polygon_map;

  std::unique_ptr<IntHex> ih;
  std::unique_ptr<FracHex> fh;

  while (window.isOpen()) {
    window.pollEvent(event);
    if (event.type == sf::Event::Closed) {
      window.close();
    } else if (event.type == sf::Event::Resized) {
      sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
      window.setView(sf::View(visibleArea));
    } else if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        auto hex = round_cube(from_pixel(layout, mouse_pos()));
        auto poly = create_polygon(layout, hex);
        poly.setTexture(&Y);
        auto&& [it, b] = polygon_map.emplace(hex, poly);
        if (b)
          BOOST_LOG_TRIVIAL(debug)
              << "(hex, poly) pair emplaced at (" << hex.q() << ", " << hex.r()
              << ", " << hex.s() << ')';
      } else if (event.mouseButton.button == sf::Mouse::Right) {
        auto hex = round_cube(from_pixel(layout, mouse_pos()));
        auto it = polygon_map.find(hex);
        if (it != polygon_map.end()) {
          polygon_map.erase(it);
          BOOST_LOG_TRIVIAL(debug) << "(hex, poly) pair erased at (" << hex.q()
                                   << ", " << hex.r() << ", " << hex.s() << ')';
        }
      }
    } else if (event.type == sf::Event::MouseMoved) {
      ih.reset(&round_cube(from_pixel(layout, mouse_pos())));
      fh.reset(&from_pixel(layout, mouse_pos()));
      std::ostringstream ostr;
      ostr << '(' << mouse_pos().x << ", " << mouse_pos().y << ")\n"
           << '(' << fh->q() << ", " << fh->r() << ", " << fh->s() << ")\n"
           << '(' << ih->q() << ", " << ih->r() << ", " << ih->s() << ')';
      text.setString(ostr.str());
    }

    window.clear();

    for (auto&& [hex, poly] : polygon_map) {
      window.draw(poly);
    }

    if (mouse_pos().x > 0 && mouse_pos().x < window.getView().getSize().x &&
        mouse_pos().y > 0 && mouse_pos().y < window.getView().getSize().y) {
      if (ih)
        window.draw(create_polygon(layout, *ih, sf::Color::Red));
      window.draw(text);
    }

    window.display();

    (void)ih.release();
    (void)fh.release();
  }

  return 0;
}

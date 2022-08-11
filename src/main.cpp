#include <SFML/Graphics.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <iostream>
#include <string>

#include "hex.h"
#include "layout.h"
#include "polymap.h"

using namespace rev;
using namespace sf;
using namespace std;

namespace logging = boost::log;

int main() {
  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::debug);

  VideoMode video_mode(800, 600);
  RenderWindow window(video_mode, "Reversio");
  window.setFramerateLimit(60);

  auto mouse_pos = [&window]() { return Mouse::getPosition(window); };
  auto gmouse_pos = [&window, &mouse_pos]() {
    return window.mapPixelToCoords(mouse_pos());
  };

  Layout layout{
      .orientation = Orientation::pointy(), .size = {16, 12}, .origin = {0, 0}};

  View view{window.getDefaultView()};

  Polymap<40, 30> map{layout};

  Text mouse_position;
  Font font;
  font.loadFromFile("C:\\Windows\\Fonts\\Hack-Regular.ttf");

  while (window.isOpen()) {
    Event event;
    window.pollEvent(event);

    if (event.type == Event::Closed) {
      window.close();
    } else if (event.type == Event::Resized) {
      FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width),
                            static_cast<float>(event.size.height));
      window.setView(View(visibleArea));
    }

    if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
      view.move(0, 10);
    } else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
      view.move(0, -10);
    } else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
      view.move(-10, 0);
    } else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
      view.move(10, 0);
    }

    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
      map.place<GrassPoly>(IntPt{gmouse_pos()});
    } else if (Mouse::isButtonPressed(Mouse::Button::Right)) {
      map.place<LavaPoly>(IntPt{gmouse_pos()});
    } else if (Keyboard::isKeyPressed(Keyboard::Delete)) {
      map.remove(IntPt{gmouse_pos()});
    }

    auto axial{hex_from_pixel_rounded<int>(layout, mouse_pos())};
    string mp{string{"("} + to_string(gmouse_pos().x) + ", " +
              to_string(gmouse_pos().y) + ") = (" + to_string(axial.q()) +
              ", " + to_string(axial.r()) + ")"};
    mouse_position.setString(mp);
    mouse_position.setFont(font);

    window.clear();

    window.setView(view);

    window.draw(map);
    window.draw(mouse_position);

    window.display();
  }

  return 0;
}

#ifndef POLY_H
#define POLY_H

#include "hex.h"
#include "layout.h"
#include "point.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <type_traits>
#include <array>

namespace rev {
class AbstractPoly : public sf::Drawable {
 public:
  using sf::Drawable::draw;
};

template <class Derived>
class Poly : public AbstractPoly, public IntHex {
 private:
  Derived& actual() {
    static_assert(std::is_base_of_v<Poly<Derived>, Derived>);
    return *static_cast<Derived*>(this);
  }

 public:
  Poly(const Layout& layout, const IntPt& position) noexcept
      : m_layout{layout},
        IntHex{hex_from_pixel_rounded(layout, position)},
        m_poly{polygon_corners(layout)} {
    decorate();
  }

  Poly(const Layout& layout, int q, int r) noexcept
      : m_layout{layout}, IntHex{q, r}, m_poly{polygon_corners(layout)} {
    decorate();
  }

  void decorate() { actual().decorate(); }

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override {
    target.draw(m_poly, states);
  }

 private:
  class PolyShape : public sf::ConvexShape {
   public:
    PolyShape(const std::array<IntPt, 6>& corners) : sf::ConvexShape{6} {
      for (size_t i = 0; i < 6; ++i) {
        setPoint(i, sf::Vector2f{corners[i]});
      }
    }
  };

 protected:
  const Layout& m_layout;
  PolyShape m_poly;
};

class GrassPoly : public Poly<GrassPoly> {
 public:
  using Poly<GrassPoly>::Poly;

 public:
  void decorate() { 
    m_poly.setOutlineThickness(1);
    m_poly.setFillColor(sf::Color::Green);
  }
};

class MountainPoly : public Poly<MountainPoly> {
 public:
  using Poly<MountainPoly>::Poly;

 public:
  void decorate() {
    m_poly.setOutlineThickness(1);
    m_poly.setFillColor(sf::Color::Color(120, 120, 120));
  }
};

class LavaPoly : public Poly<LavaPoly> {
 public:
  using Poly<LavaPoly>::Poly;

 public:
  void decorate() { 
    m_poly.setOutlineThickness(1);
    m_poly.setFillColor(sf::Color::Red);
  }
};

}  // namespace rev

#endif

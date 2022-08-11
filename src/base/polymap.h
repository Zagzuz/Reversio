#ifndef POLYMAP_H
#define POLYMAP_H

#include "point.h"
#include "poly.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>
#include <type_traits>
#include <unordered_set>

namespace rev {

template <int Q, int R, Offset Offtype = Offset::EVEN>
class Polymap : public sf::Drawable {
 private:
  using pixel_t = int;

 public:
  explicit Polymap(const Layout& layout) noexcept : m_layout{layout} {};
  explicit Polymap(Layout&& layout) noexcept : m_layout{std::move(layout)} {};

  template <class PolyTy>
  requires std::is_base_of_v<AbstractPoly, PolyTy>
  bool place(const Point<pixel_t>& position) noexcept {
    auto [q, r] = axial_to_offset<Offtype>(
        pixel_to_axial<pixel_t>(m_layout, position));
    [[unlikely]] if (!offset_in_bounds(q, r)) {
      BOOST_LOG_TRIVIAL(error)
          << "(" << position.x << ", " << position.y << ") = hex (" << q << ", "
          << r << ") is not in bounds";
      return false;
    }
    m_map[q][r] = std::make_unique<PolyTy>(m_layout, position);
    return true;
  }

  [[nodiscard]] bool contains(const Point<pixel_t>& position) noexcept {
    auto [q, r] =
        axial_to_offset<Offtype>(pixel_to_axial<pixel_t>(m_layout, position));
    return offset_in_bounds(q, r) && m_map[q, r];
  }

  void remove(const Point<pixel_t>& position) noexcept {
    auto [q, r] =
        axial_to_offset<Offtype>(pixel_to_axial<pixel_t>(m_layout, position));
    [[unlikely]] if (!offset_in_bounds(q, r)) {
      BOOST_LOG_TRIVIAL(error)
          << "(" << position.x << ", " << position.y << ") = hex (" << q << ", "
          << r << ") is not in bounds";
      return;
    }
    if (!m_map[q][r]) {
      BOOST_LOG_TRIVIAL(error)
          << "there is no poly to remove at (" << position.x << ", "
          << position.y << ") = hex (" << q << ", " << r << ")";
      return;
    }
    m_map[q][r].reset(nullptr);
    BOOST_LOG_TRIVIAL(debug)
        << "Removed poly at (" << position.x << ", " << position.y
        << ") = hex (" << q << ", " << r << ")";
  }

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override {
    for (const auto& col : m_map) {
      for (const auto& poly_ptr : col) {
        if (poly_ptr) {
          target.draw(*poly_ptr.get(), states);
        }
      }
    }
  }

 private:
  [[nodiscard]] static constexpr bool offset_in_bounds(int q, int r) noexcept {
    return q >= 0 && q < Q && r >= 0 && r < R;
  }

 private:
  Layout m_layout;
  std::unique_ptr<AbstractPoly> m_map[Q][R];
};
}  // namespace rev

#endif

#ifndef HEX_H
#define HEX_H

#include "layout.h"
#include "nof.h"
#include "point.h"

#include <boost/functional/hash.hpp>
#include <boost/log/trivial.hpp>

#include <array>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace rev {
enum class Offset { ODD = -1, EVEN = 1 };

template <class T>
struct Hex {
 public:
  using crd_t = T;

 public:
  constexpr explicit Hex(crd_t q, crd_t r) noexcept : m_q(q), m_r(r) {}

  Hex(crd_t q, crd_t r, crd_t s) : m_q(q), m_r(r) {
    if (auto res{Nof(q) + r + s}; !res.has_value() || res.value() != 0) {
      throw std::invalid_argument("Hex coordinate constraint violation");
    }
  }

  Hex(const Hex&) noexcept = default;
  Hex(Hex&&) noexcept = default;

  [[nodiscard]] crd_t q() const noexcept { return m_q; }
  [[nodiscard]] crd_t r() const noexcept { return m_r; }
  [[nodiscard]] crd_t s() const noexcept { return -m_q - m_r; }

  template <class PxlTy>
  [[nodiscard]] Point<PxlTy> to_pixel(const Layout& layout) const noexcept {
    auto x{(layout.orientation.f0 * q() + layout.orientation.f1 * r()) *
           layout.size.x};
    auto y{(layout.orientation.f2 * q() + layout.orientation.f3 * r()) *
           layout.size.y};
    return {static_cast<crd_t>(x + layout.origin.x),
            static_cast<crd_t>(y + layout.origin.y)};
  }

  [[nodiscard]] std::array<Point<crd_t>, 6> polygon_corners(
      const Layout& layout) const noexcept {
    auto center{to_pixel<crd_t>(layout)};
    return {
        center + corner_offset(layout, 0), center + corner_offset(layout, 1),
        center + corner_offset(layout, 2), center + corner_offset(layout, 3),
        center + corner_offset(layout, 4), center + corner_offset(layout, 5)};
  }

  template <int HexDir>
  [[nodiscard]] Hex neighbor() const {
    return *this + direction(HexDir);
  }

  template <int HexDir>
  [[nodiscard]] static const Hex& direction() {
    static_assert(HexDir >= 0 && HexDir < 6);
    static Hex dirs[6]{Hex{1, 0, -1}, Hex{1, -1, 0}, Hex{0, -1, 1},
                       Hex{-1, 0, 1}, Hex{-1, 1, 0}, Hex{0, 1, -1}};
    return dirs[HexDir];
  }

  [[nodiscard]] static crd_t distance(const Hex& lhs, const Hex& rhs) noexcept {
    return std::max(
        std::abs(lhs.s() - rhs.s()),
        std::max(std::abs(lhs.q() - rhs.q()), std::abs(lhs.r() - rhs.r())));
  }

  [[nodiscard]] static Point<crd_t> corner_offset(const Layout& layout,
                                                  int corner) noexcept {
    constexpr long double PI{3.14159265358979323846};
    const auto angle{2 * PI * (layout.orientation.start_angle + corner) / 6};
    return {static_cast<crd_t>(layout.size.x * std::cos(angle)),
            static_cast<crd_t>(layout.size.y * std::sin(angle))};
  }

  [[nodiscard]] auto operator<=>(const Hex<T>&) const = default;

 private:
  const crd_t m_q, m_r;
};

template <class T>
std::size_t hash_value(const Hex<T>& p) {
  std::size_t seed{0};
  boost::hash_combine(seed, p.q());
  boost::hash_combine(seed, p.r());
  return seed;
}

template <class PxlTy>
[[nodiscard]] Point<PxlTy> pixel_to_axial(const Layout& layout,
                                          const Point<PxlTy>& p) {
  auto ptx{static_cast<double>(p.x - layout.origin.x) / layout.size.x};
  auto pty{static_cast<double>(p.y - layout.origin.y) / layout.size.y};
  auto q{static_cast<double>(layout.orientation.b0 * ptx +
                             layout.orientation.b1 * pty)};
  auto r{static_cast<double>(layout.orientation.b2 * ptx +
                             layout.orientation.b3 * pty)};
  auto s{-q - r};
  auto q2{static_cast<PxlTy>(std::lround(q))};
  auto r2{static_cast<PxlTy>(std::lround(r))};
  auto s2{static_cast<PxlTy>(std::lround(s))};
  double q_diff{std::abs(q2 - q)};
  double r_diff{std::abs(r2 - r)};
  double s_diff{std::abs(s2 - s)};
  if (q_diff > r_diff && q_diff > s_diff) {
    q2 = -r2 - s2;
  } else if (r_diff > s_diff) {
    r2 = -q2 - s2;
  } else {
    s2 = -q2 - r2;
  }
  return Point<PxlTy>{q2, r2};
}

[[nodiscard]] inline Hex<int> round_cube(Hex<double> hex) noexcept {
  auto q2{static_cast<int>(std::lround(hex.q()))};
  auto r2{static_cast<int>(std::lround(hex.r()))};
  auto s2{static_cast<int>(std::lround(hex.s()))};
  double q_diff{std::abs(q2 - hex.q())};
  double r_diff{std::abs(r2 - hex.r())};
  double s_diff{std::abs(s2 - hex.s())};
  if (q_diff > r_diff && q_diff > s_diff) {
    q2 = -r2 - s2;
  } else if (r_diff > s_diff) {
    r2 = -q2 - s2;
  } else {
    s2 = -q2 - r2;
  }
  return {q2, r2, s2};
}

template <class PxlTy>
[[nodiscard]] Hex<double> hex_from_pixel(const Layout& layout,
                                         const Point<PxlTy>& p) noexcept {
  static_assert(std::is_signed_v<PxlTy>, "Possible overflow next line");
  auto ptx{static_cast<double>(p.x - layout.origin.x) / layout.size.x};
  auto pty{static_cast<double>(p.y - layout.origin.y) / layout.size.y};
  return Hex{static_cast<double>(layout.orientation.b0 * ptx +
                                 layout.orientation.b1 * pty),
             static_cast<double>(layout.orientation.b2 * ptx +
                                 layout.orientation.b3 * pty)};
}

template <class PxlTy>
[[nodiscard]] Hex<int> hex_from_pixel_rounded(const Layout& layout,
                                              const Point<PxlTy>& p) noexcept {
  return round_cube(hex_from_pixel<PxlTy>(layout, p));
}

template <Offset offset_type>
[[nodiscard]] constexpr IntPt axial_to_offset(IntPt p) {
  return IntPt{p.x + (p.y + static_cast<int>(offset_type) * (p.y & 1)) / 2,
               p.y};
}

template <class T>
requires std::is_integral_v<T>
[[nodiscard]] std::vector<Hex<T>> line(const Hex<T>& h1, const Hex<T>& h2) {
  const auto dst{Hex<T>::distance(h1, h2)};
  auto lerp{
      [](double a, double b, double t) -> double { return a + (b - a) * t; }};
  auto cube_lerp{
      [&lerp](const Hex<T>& a, const Hex<T>& b, double t) -> Hex<double> {
        return Hex<double>{lerp(a.q(), b.q(), t), lerp(a.r(), b.r(), t),
                           lerp(a.s(), b.s(), t)};
      }};
  std::vector<Hex<T>> hexes_on_the_line;
  hexes_on_the_line.reserve(static_cast<size_t>(dst));
  for (T i = 0; i < dst; ++i) {
    hexes_on_the_line.emplace_back(
        round_cube(cube_lerp(h1, h2, 1.0 / dst * i)));
  }
  return hexes_on_the_line;
}

template <class T>
[[nodiscard]] Hex<T> operator+(const Hex<T>& lhs, const Hex<T>& rhs) {
  auto x{Nof(lhs.q()) + rhs.q()};
  auto y{Nof(lhs.r()) + rhs.r()};
  auto z{Nof(lhs.s()) + rhs.s()};
  if (!x.has_value() || !y.has_value() || !z.has_value()) {
    BOOST_LOG_TRIVIAL(error)
        << "Hex coordinates overflow occured while executing "
           "Hex("
        << lhs.q() << ';' << lhs.r() << ';' << lhs.s()
        << ") + "
           "Hex("
        << rhs.q() << ';' << rhs.r() << ';' << rhs.s() << ")\n";
    return lhs;
  }
  return Hex{x.get(), y.get(), z.get()};
}

template <class T>
[[nodiscard]] Hex<T> operator-(const Hex<T>& lhs, const Hex<T>& rhs) {
  auto x{Nof(lhs.q()) - rhs.q()};
  auto y{Nof(lhs.r()) - rhs.r()};
  auto z{Nof(lhs.s()) - rhs.s()};
  if (!x.has_value() || !y.has_value() || !z.has_value()) {
    BOOST_LOG_TRIVIAL(error)
        << "Hex coordinates overflow occured while executing "
           "Hex("
        << lhs.q() << ';' << lhs.r() << ';' << lhs.s()
        << ") - "
           "Hex("
        << rhs.q() << ';' << rhs.r() << ';' << rhs.s() << ")\n";
    return lhs;
  }
  return Hex{x.get(), y.get(), z.get()};
}

template <class T>
[[nodiscard]] Hex<T> operator*(const Hex<T>& lhs, const Hex<T>& rhs) {
  auto x{Nof(lhs.q()) * rhs.q()};
  auto y{Nof(lhs.r()) * rhs.r()};
  auto z{Nof(lhs.s()) * rhs.s()};
  if (!x.has_value() || !y.has_value() || !z.has_value()) {
    BOOST_LOG_TRIVIAL(error)
        << "Hex coordinates overflow occured while executing "
           "Hex("
        << lhs.q() << ';' << lhs.r() << ';' << lhs.s()
        << ") * "
           "Hex("
        << rhs.q() << ';' << rhs.r() << ';' << rhs.s() << ")\n";
    return lhs;
  }
  return Hex{x.get(), y.get(), z.get()};
}

using IntHex = Hex<int>;
using FracHex = Hex<double>;
}  // namespace rev

#endif  // HEX_H

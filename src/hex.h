#ifndef HEX_H
#define HEX_H

#define _USE_MATH_DEFINES

#include "nof.h"
#include "layout.h"
#include "point.h"

#include <cmath>
#include <array>
#include <cassert>
#include <stdexcept>
#include <boost/log/trivial.hpp>

namespace rev
{
    struct Hex
    {
    public:
        using crd_t = int;
        Hex(crd_t, crd_t);
        Hex(crd_t, crd_t, crd_t);
        crd_t q() const noexcept { return q_; }
        crd_t r() const noexcept { return r_; }
        crd_t s() const noexcept { return -q_ - r_; }
        crd_t length() const noexcept;
        static Hex::crd_t distance(const Hex& lhs, const Hex& rhs) noexcept;
        static const Hex& direction(int dir);
        Hex neighbor(int dir) const;
        Point<crd_t> to_pixel(const Layout<crd_t>& layout) const noexcept;
        static Hex from_pixel(const Layout<crd_t>& layout, const Point<crd_t>& p);
        static Point<crd_t> corner_offset(const Layout<crd_t>& layout, int corner);
        std::array<Point<crd_t>, 6> polygon_corners(const Layout<Hex::crd_t>& layout) const;
    private:
        const crd_t q_, r_;
    };
    
    Hex operator+(const Hex& lhs, const Hex& rhs);
    Hex operator-(const Hex& lhs, const Hex& rhs);
    Hex operator*(const Hex& lhs, const Hex& rhs);
    bool operator==(const Hex& lhs, const Hex& rhs) noexcept;
    
    struct HexHash
    {
        std::size_t operator()(const Hex& h) const
        {
            return std::hash<Hex::crd_t>()(h.q()) << 2 ^
                   std::hash<Hex::crd_t>()(h.r()) << 1 ^
                   std::hash<Hex::crd_t>()(h.s());
        }
    };
} // namespace rev

#endif // HEX_H

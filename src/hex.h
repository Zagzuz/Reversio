#ifndef HEX_H
#define HEX_H

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
        Hex(crd_t, crd_t, crd_t);
        crd_t q() const noexcept { return q_; }
        crd_t r() const noexcept { return r_; }
        crd_t s() const noexcept { return -q_ - r_; }
        crd_t length() const noexcept;
        static Hex::crd_t distance(const Hex& lhs, const Hex& rhs) noexcept;
        static const Hex& direction(int dir);
        Hex neighbor(int dir) const;
        Point<crd_t> to_pixel(const Layout<crd_t>& layout) const noexcept;
    private:
        const crd_t q_, r_;
    };
    
    Hex operator+(const Hex& lhs, const Hex& rhs);
    Hex operator-(const Hex& lhs, const Hex& rhs);
    Hex operator*(const Hex& lhs, const Hex& rhs);
    bool operator==(const Hex& lhs, const Hex& rhs) noexcept;
} // namespace rev

#endif // HEX_H
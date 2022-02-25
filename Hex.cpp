#include "Hex.h"

#include <cmath>

namespace rev
{
    Hex::Hex(Hex::crd_t q, Hex::crd_t r, Hex::crd_t s) : q_(q), r_(r)
    {
        assert(q + r + s == 0);
    }

    Hex::crd_t Hex::length() const
    {
        return static_cast<Hex::crd_t>((abs(q()) + abs(r()) + abs(s())) / 2);
    }

    Hex::crd_t Hex::distance(const Hex& lhs, const Hex& rhs)
    {
        return static_cast<Hex::crd_t>((abs(lhs.q() - rhs.q()) +
                                        abs(lhs.r() - rhs.r()) +
                                        abs(lhs.s() - rhs.s())) / 2);
    }

    const Hex& Hex::direction(int dir)
    {
        assert(dir >= 0 && dir < 6);
        static std::array<Hex, 6> dirs = { Hex(1, 0, -1), Hex(1, -1, 0),
            Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1) };
        return dirs[dir];
    }

    Hex Hex::neighbor(int dir) const
    {
        return *this + direction(dir);
    }

    Hex operator+(const Hex& lhs, const Hex& rhs)
    {
        return Hex(lhs.q() + rhs.q(), 
                   lhs.r() + rhs.r(),
                   lhs.s() + rhs.s());
    }

    Hex operator-(const Hex& lhs, const Hex& rhs)
    {
        return Hex(lhs.q() - rhs.q(),
                   lhs.r() - rhs.r(),
                   lhs.s() - rhs.s());
    }

    Hex operator*(const Hex& lhs, const Hex& rhs)
    {
        return Hex(lhs.q() * rhs.q(),
                   lhs.r() * rhs.r(),
                   lhs.s() * rhs.s());
    }

    bool operator==(const Hex& lhs, const Hex& rhs) noexcept
    {
        return lhs.q() == rhs.q() &&
               lhs.r() == rhs.r() &&
               lhs.s() == rhs.s();
    }
} // namespace rev
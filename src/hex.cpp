#include "Hex.h"

namespace rev
{
    Hex::Hex(Hex::crd_t q, Hex::crd_t r, Hex::crd_t s) : q_(q), r_(r)
    {
        if (auto res = Nof(q) + r + s; !res.has_value() || res.get() != 0)
        {
            throw std::invalid_argument("Hex coordinate constraint violation");
        }
    }

    Hex::crd_t Hex::length() const noexcept
    {
        return std::abs(q() / 2) + std::abs(r() / 2) + std::abs(s() / 2);
    }

    Hex::crd_t Hex::distance(const Hex& lhs, const Hex& rhs) noexcept
    {
        return std::abs(lhs.q() / 2 - rhs.q() / 2) +
               std::abs(lhs.r() / 2 - rhs.r() / 2) +
               std::abs(lhs.s() / 2 - rhs.s() / 2);
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
        auto x = Nof(lhs.q()) + rhs.q();
        auto y = Nof(lhs.r()) + rhs.r();
        auto z = Nof(lhs.s()) + rhs.s();
        if (!x.has_value() || !y.has_value() || !z.has_value())
        {
            BOOST_LOG_TRIVIAL(error) << 
                "Hex coordinates overflow occured while executing "
                "Hex(" << lhs.q() << ';' << lhs.r() << ';' << lhs.s() << ") + "
                "Hex(" << rhs.q() << ';' << rhs.r() << ';' << rhs.s() << ")\n";
            return lhs;
        }
        return Hex(x.get(), y.get(), z.get());
    }

    Hex operator-(const Hex& lhs, const Hex& rhs)
    {
        auto x = Nof(lhs.q()) - rhs.q();
        auto y = Nof(lhs.r()) - rhs.r();
        auto z = Nof(lhs.s()) - rhs.s();
        if (!x.has_value() || !y.has_value() || !z.has_value())
        {
            BOOST_LOG_TRIVIAL(error) <<
                "Hex coordinates overflow occured while executing "
                "Hex(" << lhs.q() << ';' << lhs.r() << ';' << lhs.s() << ") - "
                "Hex(" << rhs.q() << ';' << rhs.r() << ';' << rhs.s() << ")\n";
            return lhs;
        }
        return Hex(x.get(), y.get(), z.get());
    }

    Hex operator*(const Hex& lhs, const Hex& rhs)
    {
        auto x = Nof(lhs.q()) * rhs.q();
        auto y = Nof(lhs.r()) * rhs.r();
        auto z = Nof(lhs.s()) * rhs.s();
        if (!x.has_value() || !y.has_value() || !z.has_value())
        {
            BOOST_LOG_TRIVIAL(error) <<
                "Hex coordinates overflow occured while executing "
                "Hex(" << lhs.q() << ';' << lhs.r() << ';' << lhs.s() << ") * "
                "Hex(" << rhs.q() << ';' << rhs.r() << ';' << rhs.s() << ")\n";
            return lhs;
        }
        return Hex(x.get(), y.get(), z.get());
    }

    bool operator==(const Hex& lhs, const Hex& rhs) noexcept
    {
        return lhs.q() == rhs.q() &&
               lhs.r() == rhs.r() &&
               lhs.s() == rhs.s();
    }
} // namespace rev
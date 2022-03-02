#include "hex.h"

namespace rev
{
    Hex::Hex(Hex::crd_t q, Hex::crd_t r) : q_(q), r_(r) {}

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

    Point<Hex::crd_t> Hex::to_pixel(const Layout<crd_t>& layout) const noexcept
    {
        double x = (layout.orientation.f0 * q() + layout.orientation.f1 * r()) * layout.size.x;
        double y = (layout.orientation.f2 * q() + layout.orientation.f3 * r()) * layout.size.y;
        return Point<crd_t>(x + layout.origin.x, y + layout.origin.y);
    }

    Hex Hex::from_pixel(const Layout<crd_t>& layout, const Point<crd_t>& p)
    {
        Point<crd_t> pt = {
            (p.x - layout.origin.x) / layout.size.x,
            (p.y - layout.origin.y) / layout.size.y
        };
        double q = layout.orientation.b0 * pt.x + layout.orientation.b1 * pt.y;
        double r = layout.orientation.b2 * pt.x + layout.orientation.b3 * pt.y;
        double s = -q - r;
        Hex::crd_t q2 = static_cast<Hex::crd_t>(std::round(q));
        Hex::crd_t r2 = static_cast<Hex::crd_t>(std::round(r));
        Hex::crd_t s2 = static_cast<Hex::crd_t>(std::round(s));
        double q_diff = std::abs(q2 - q);
        double r_diff = std::abs(r2 - r);
        double s_diff = std::abs(s2 - s);
        if (q_diff > r_diff && q_diff > s_diff)
            q2 = -r2 - s2;
        else if (r_diff > s_diff)
            r2 = -q2 - s2;
        else
            s2 = -q2 - r2;
        return Hex(q2, r2, s2);
    }

    Point<Hex::crd_t> Hex::corner_offset(const Layout<crd_t>& layout, int corner)
    {
        double angle = 2.0 * M_PI * (layout.orientation.start_angle + corner) / 6;
        return Point<crd_t>(layout.size.x * cos(angle), layout.size.y * sin(angle));
    }

    std::array<Point<Hex::crd_t>, 6> Hex::polygon_corners(const Layout<Hex::crd_t>& layout) const
    {
        Point<crd_t> center = this->to_pixel(layout);
        std::array<Point<crd_t>, 6> corners = {
            center + corner_offset(layout, 0),
            center + corner_offset(layout, 1),
            center + corner_offset(layout, 2),
            center + corner_offset(layout, 3),
            center + corner_offset(layout, 4),
            center + corner_offset(layout, 5),
        };
        return corners;
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

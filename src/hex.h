#ifndef HEX_H
#define HEX_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "nof.h"
#include "layout.h"
#include "point.h"

#include <cmath>
#include <array>
#include <utility>
#include <cassert>
#include <stdexcept>
#include <boost/log/trivial.hpp>

namespace rev
{
    template <class T>
    struct Hex
    {
    public:
        using crd_t = T;
        Hex(crd_t, crd_t);
        Hex(crd_t, crd_t, crd_t);
        Hex(const Hex& rhs) noexcept;
        Hex(Hex&& rhs) noexcept;
        crd_t q() const noexcept { return q_; }
        crd_t r() const noexcept { return r_; }
        crd_t s() const noexcept { return -q_ - r_; }
        crd_t length() const noexcept;
        static crd_t distance(const Hex& lhs, const Hex& rhs) noexcept;
        static const Hex& direction(int dir);
        Hex neighbor(int dir) const;
        Point<crd_t> to_pixel(const Layout& layout) const noexcept;
        static Point<crd_t> corner_offset(const Layout& layout, int corner);
        std::array<Point<crd_t>, 6> polygon_corners(const Layout& layout) const;
        static Hex<crd_t> from_pixel(const Layout& layout, const Point<crd_t>& p) = delete;
    private:
        const crd_t q_, r_;
    };

    template <class T>
    Hex<T>::Hex(crd_t q, crd_t r) : q_(q), r_(r) {}

    template <class T>
    Hex<T>::Hex(crd_t q, crd_t r, crd_t s) : q_(q), r_(r)
    {
        if (auto res = Nof(q) + r + s; !res.has_value() || res.get() != 0)
        {
            throw std::invalid_argument("Hex coordinate constraint violation");
        }
    }

    template <class T>
    Hex<T>::Hex(const Hex& rhs) noexcept : q_(rhs.q_), r_(rhs.r_) {}

    template<class T>
    Hex<T>::Hex(Hex&& rhs) noexcept : q_(std::move(rhs.q_)), r_(std::move(rhs.r_)) {}

    template <class T>
    typename Hex<T>::crd_t Hex<T>::length() const noexcept
    {
        return std::abs(q() / 2) + std::abs(r() / 2) + std::abs(s() / 2);
    }

    template <class T>
    typename Hex<T>::crd_t Hex<T>::distance(const Hex<T>& lhs, const Hex<T>& rhs) noexcept
    {
        return std::abs(lhs.q() / 2 - rhs.q() / 2) +
            std::abs(lhs.r() / 2 - rhs.r() / 2) +
            std::abs(lhs.s() / 2 - rhs.s() / 2);
    }

    template <class T>
    Point<typename Hex<T>::crd_t> Hex<T>::to_pixel(const Layout& layout) const noexcept
    {
        auto x = (layout.orientation.f0 * q() + layout.orientation.f1 * r()) * layout.size.x;
        auto y = (layout.orientation.f2 * q() + layout.orientation.f3 * r()) * layout.size.y;
        return {
            static_cast<crd_t>(x + layout.origin.x),
            static_cast<crd_t>(y + layout.origin.y)
        };
    }

    template <class T>
    std::array<Point<T>, 6> Hex<T>::polygon_corners(const Layout& layout) const
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

    template <class T>
    const Hex<T>& Hex<T>::direction(int dir)
    {
        assert(dir >= 0 && dir < 6);
        static std::array<Hex, 6> dirs = { Hex(1, 0, -1), Hex(1, -1, 0),
            Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1) };
        return dirs[dir];
    }

    template <class T>
    Hex<T> Hex<T>::neighbor(int dir) const
    {
        return *this + direction(dir);
    }

    template <class T>
    struct HexHash
    {
        std::size_t operator()(const Hex<T>& h) const
        {
            return std::hash<Hex<T>::crd_t>()(h.q()) << 2 ^
                   std::hash<Hex<T>::crd_t>()(h.r()) << 1 ^
                   std::hash<Hex<T>::crd_t>()(h.s());
        }
    };

    inline Hex<double> from_pixel(const Layout& layout, const Point<int>& p)
    {
        double ptx = static_cast<double>(p.x - layout.origin.x) / layout.size.x;
        double pty = static_cast<double>(p.y - layout.origin.y) / layout.size.y;
        return { 
            static_cast<double>(layout.orientation.b0 * ptx + layout.orientation.b1 * pty),
            static_cast<double>(layout.orientation.b2 * ptx + layout.orientation.b3 * pty) 
        };
    }

    inline Hex<int> round_cube(Hex<double> hex)
    {
        int q2 = static_cast<int>(std::lround(hex.q()));
        int r2 = static_cast<int>(std::lround(hex.r()));
        int s2 = static_cast<int>(std::lround(hex.s()));
        double q_diff = std::abs(q2 - hex.q());
        double r_diff = std::abs(r2 - hex.r());
        double s_diff = std::abs(s2 - hex.s());
        if (q_diff > r_diff && q_diff > s_diff)
            q2 = -r2 - s2;
        else if (r_diff > s_diff)
            r2 = -q2 - s2;
        else
            s2 = -q2 - r2;
        return { q2, r2, s2 };
    }

    template <>
    Hex<int> Hex<int>::from_pixel(const Layout& layout, const Point<crd_t>& p)
    {
        return round_cube(rev::from_pixel(layout, p));
    }

    template<class T>
    Point<typename Hex<T>::crd_t> Hex<T>::corner_offset(const Layout& layout, int corner)
    {
        auto angle = 2 * M_PI * (layout.orientation.start_angle + corner) / 6;
        return {
            static_cast<crd_t>(layout.size.x * std::cos(angle)),
            static_cast<crd_t>(layout.size.y * std::sin(angle))
        };
    }

    template <class T>
    Hex<T> operator+(const Hex<T>& lhs, const Hex<T>& rhs)
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

    template <class T>
    Hex<T> operator-(const Hex<T>& lhs, const Hex<T>& rhs)
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

    template <class T>
    Hex<T> operator*(const Hex<T>& lhs, const Hex<T>& rhs)
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

    template <class T>
    bool operator==(const Hex<T>& lhs, const Hex<T>& rhs) noexcept
    {
        return lhs.q() == rhs.q() &&
            lhs.r() == rhs.r() &&
            lhs.s() == rhs.s();
    }

    using IntHex = Hex<int>;
    using FracHex = Hex<double>;
} // namespace rev

#endif // HEX_H

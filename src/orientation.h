#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <cmath>
#include <utility>

namespace rev
{
    struct Orientation
    {
        Orientation(double f0, double f1, double f2, double f3,
                    double b0, double b1, double b2, double b3,
                    double start_angle) noexcept;
        Orientation(const Orientation& x) noexcept;
        Orientation& operator=(Orientation x) noexcept;
        const double f0, f1, f2, f3;
        const double b0, b1, b2, b3;
        const double start_angle; // in multiples of 60°
    };

    inline Orientation pointy_orientation() noexcept
    {
        return {
            std::sqrt(3.0), std::sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
            std::sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
            0.5
        };
    }

    inline Orientation flat_orientation() noexcept
    {
        return {
            3.0 / 2.0, 0.0, std::sqrt(3.0) / 2.0, std::sqrt(3.0),
            2.0 / 3.0, 0.0, -1.0 / 3.0, std::sqrt(3.0) / 3.0,
            0.0
        };
    }
} // namespace rev

#endif // ORIENTATION_H

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <cmath>
#include <utility>

namespace rev
{
    struct Orientation
    {
        using value_type = double;
        Orientation(value_type f0, value_type f1, value_type f2, value_type f3,
                    value_type b0, value_type b1, value_type b2, value_type b3,
                    value_type start_angle) noexcept;
        Orientation(const Orientation& x) noexcept;
        Orientation& operator=(Orientation x) noexcept;
        const value_type f0, f1, f2, f3;
        const value_type b0, b1, b2, b3;
        const value_type start_angle;
    };

    inline Orientation pointy_orientation() noexcept
    {
        return {
            std::sqrt(3), std::sqrt(3) / 2, 0.0, 3.0 / 2,
            std::sqrt(3) / 3, -1.0 / 3, 0, 2.0 / 3.0,
            0.5
        };
    }

    inline Orientation flat_orientation() noexcept
    {
        return {
            3.0 / 2.0, 0.0, std::sqrt(3) / 2.0, std::sqrt(3),
            2.0 / 3.0, 0.0, -1.0 / 3.0, std::sqrt(3) / 3.0,
            0.0
        };
    }
} // namespace rev

#endif // ORIENTATION_H

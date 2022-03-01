#ifndef ORIENTATION_H
#define ORIENTATION_H

namespace rev
{
    struct Orientation
    {
        Orientation(double f0, double f1, double f2, double f3,
                    double b0, double b1, double b2, double b3,
                    double start_angle) :
            f0(f0), f1(f1), f2(f2), f3(f3),
            b0(b0), b1(b1), b2(b2), b3(b3),
            start_angle(start_angle) {}
        const double f0, f1, f2, f3;
        const double b0, b1, b2, b3;
        const double start_angle; // in multiples of 60�
    };

    static inline Orientation layout_pointy = {
        std::sqrt(3.0), std::sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
        std::sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
        0.5
    };
    
    static inline Orientation layout_flat = {
        3.0 / 2.0, 0.0, std::sqrt(3.0) / 2.0, std::sqrt(3.0),
        2.0 / 3.0, 0.0, -1.0 / 3.0, std::sqrt(3.0) / 3.0,
        0.0
    };
}

#endif // ORIENTATION_H
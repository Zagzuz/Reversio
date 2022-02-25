#ifndef HEX_H
#define HEX_H

#include <cassert>
#include <array>

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
        crd_t length() const;
        static Hex::crd_t distance(const Hex& lhs, const Hex& rhs);
        static const Hex& direction(int dir);
        Hex neighbor(int dir) const;
    private:
        const crd_t q_, r_;
    };
    
    Hex operator+(const Hex& lhs, const Hex& rhs);
    Hex operator-(const Hex& lhs, const Hex& rhs);
    Hex operator*(const Hex& lhs, const Hex& rhs);
    bool operator==(const Hex& lhs, const Hex& rhs) noexcept;
} // namespace rev

#endif // HEX_H
#include "orientation.h"

namespace rev {
Orientation::Orientation(value_type f0,
                         value_type f1,
                         value_type f2,
                         value_type f3,
                         value_type b0,
                         value_type b1,
                         value_type b2,
                         value_type b3,
                         value_type start_angle) noexcept
    : f0(f0),
      f1(f1),
      f2(f2),
      f3(f3),
      b0(b0),
      b1(b1),
      b2(b2),
      b3(b3),
      start_angle(start_angle) {}

Orientation::Orientation(const Orientation& x) noexcept
    : f0(x.f0),
      f1(x.f1),
      f2(x.f2),
      f3(x.f3),
      b0(x.b0),
      b1(x.b1),
      b2(x.b2),
      b3(x.b3),
      start_angle(x.start_angle) {}

Orientation& Orientation::operator=(Orientation x) noexcept {
  std::swap(*this, x);
  return *this;
}
}  // namespace rev

#ifndef LAYOUT_H
#define LAYOUT_H

#include "orientation.hpp"
#include "point.h"

namespace rev {
struct Layout {
  using size_type = unsigned int;
  Layout(Orientation orientation,
         Point<size_type> size,
         Point<size_type> origin = {0, 0}) noexcept
      : orientation(orientation), size(size), origin(origin) {}
  const Orientation orientation;
  const Point<size_type> size, origin;
};
}  // namespace rev

#endif  // LAYOUT_H
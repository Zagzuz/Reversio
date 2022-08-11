#ifndef LAYOUT_H
#define LAYOUT_H

#include "orientation.h"
#include "point.h"

namespace rev {
struct Layout {
 public:
  using size_type = int;

 public:
  const Orientation orientation;
  const Point<size_type> size, origin;
};
}  // namespace rev

#endif  // LAYOUT_H
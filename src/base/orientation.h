#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <cmath>

namespace rev {
/**
 * @brief A helper structure holding parameters defining how the hexagon will
 * be located on the screen
 * @remark More specifically, parameters define how the conversion is made
 * between hex coordinates and screen coordinates and vice versa
 */
struct Orientation {
 public:
  using value_type = double;

 public:
  /**
   * @brief Creates pointy orientation
   * @remark Pointy top starts at 30°
   * @return Newly created pointy orientation
   */
  [[nodiscard]] static Orientation pointy() noexcept {
    return Orientation{.f0 = std::sqrt(3),
                       .f1 = std::sqrt(3) / 2,
                       .f2 = 0.0,
                       .f3 = 3.0 / 2,
                       .b0 = std::sqrt(3) / 3,
                       .b1 = -1.0 / 3,
                       .b2 = 0,
                       .b3 = 2.0 / 3.0,
                       .start_angle = 0.5};
  }

  /**
   * @brief Creates flat orientation
   * @remark Pointy top starts at 0°
   * @return Newly created flat orientation
   */
  [[nodiscard]] static Orientation flat() noexcept {
    return Orientation{.f0 = 3.0 / 2.0,
                       .f1 = 0.0,
                       .f2 = std::sqrt(3) / 2.0,
                       .f3 = std::sqrt(3),
                       .b0 = 2.0 / 3.0,
                       .b1 = 0.0,
                       .b2 = -1.0 / 3.0,
                       .b3 = std::sqrt(3) / 3.0,
                       .start_angle = 0.0};
  }

 public:
  const value_type f0, f1, f2, f3; /**< 2x2 forward matrix (hex -> screen) */
  const value_type b0, b1, b2, b3; /**< 2x2 backward matrix (screen -> hex) */
  const value_type start_angle;    /**< Corner drawing starting angle */
};
}  // namespace rev

#endif  // ORIENTATION_H

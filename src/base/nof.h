#ifndef NOF_H
#define NOF_H

#include <limits>
#include <optional>
#include <type_traits>

namespace rev {
/**
 * @brief A class template based on optional monad designed to protect a value
 * from overflowing in some basic arithmetic operations
 * @remark Nof = NoOverFlow
 * @pre Works for arithmetic types
 */
template <class T>
requires std::is_arithmetic_v<T>
class Nof {
 public:
  using value_type = T;

 public:
  /**
   * @brief Pass-by-value ctor
   * @param v Value to be initialized with
   */
  constexpr explicit Nof(T v) noexcept : m_opt{v} {}

  /**
   * @brief Retrieve value
   * @return Contained value
   */
  [[nodiscard]] T value() const noexcept { return m_opt.value(); }

  /**
   * @brief Checks if a valid value is contained by the instance
   * @return true if there is a value, false otherwise
   */
  [[nodiscard]] bool has_value() const noexcept { return m_opt.has_value(); }

  /**
   * @brief Addition operator
   * @param rhs A value to be added
   * @return New instance holding a addition result
   */
  [[nodiscard]] constexpr Nof<T> operator+(const T& rhs) noexcept {
    using limit = std::numeric_limits<T>;
    if (!m_opt.has_value() ||
        (rhs > 0) && (m_opt.value() > limit::max() - rhs) ||
        (rhs < 0) && (m_opt.value() < limit::min() - rhs)) {
      m_opt.reset();
      return *this;
    }

    return Nof{m_opt.value() + rhs};
  }

  /**
   * @brief Substraction operator
   * @param rhs A value to be substracted
   * @return New instance holding a substraction result
   */
  [[nodiscard]] constexpr Nof<T> operator-(const T& rhs) noexcept {
    using limit = std::numeric_limits<T>;
    if (!m_opt.has_value() ||
        (rhs < 0) && (m_opt.value() > limit::max() + rhs) ||
        (rhs > 0) && (m_opt.value() < limit::min() + rhs)) {
      m_opt.reset();
      return *this;
    }
    return Nof{m_opt.value() - rhs};
  }

  /**
   * @brief Multiplication operator
   * @param rhs A value to be multiplied
   * @return New instance holding a multiplication result
   */
  [[nodiscard]] constexpr Nof<T> operator*(const T& rhs) noexcept {
    using limit = std::numeric_limits<T>;
    if (!m_opt.has_value() || (m_opt.value() > limit::max() / rhs) ||
        (m_opt.value() < limit::min() / rhs)) {
      m_opt.reset();
      return *this;
    }
    return Nof{m_opt.value() * rhs};
  }

 private:
  std::optional<T> m_opt;
};
}  // namespace rev

#endif  // NOF_H

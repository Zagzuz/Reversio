#ifndef POINT_H
#define POINT_H 

#include <SFML/System/Vector2.hpp>

namespace rev {
template <typename T>
using Point = sf::Vector2<T>;

using IntPt = sf::Vector2<int>;
using FracPt = sf::Vector2<double>;
}  // namespace rev

#endif  // POINT_H

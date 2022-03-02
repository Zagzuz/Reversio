#ifndef POINT_H
#define POINT_H

namespace rev
{
	template <typename T>
	struct Point
	{
		constexpr Point(T x, T y) noexcept : x(x), y(y) {}
		Point(const Point& p) noexcept : x(p.x), y(p.y) {}
		Point& operator=(Point p) noexcept { std::swap(*this, p); return *this; }
		const T x, y;
	};

	template <typename T>
	Point<T> operator+(const Point<T>& lhs, const Point<T>& rhs) noexcept
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}
} // namespace rev

#endif // POINT_H

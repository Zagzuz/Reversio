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
} // namespace rev

#endif // POINT_H
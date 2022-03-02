#ifndef LAYOUT_H
#define LAYOUT_H

#include "orientation.h"
#include "point.h"

namespace rev
{	
	template <typename T>
	struct Layout
	{
		Layout(Orientation orientation, Point<T> size, Point<T> origin = { 0, 0 }) noexcept :
			orientation(orientation), size(size), origin(origin) {}
		const Orientation orientation;
		const Point<T> size, origin;
	};
} // namespace rev

#endif // LAYOUT_H
#ifndef NOF_H
#define NOF_H

#include <optional>
#include <type_traits>
#include <limits>

namespace rev
{
	template <class T>
	class Nof
	{
	public:
		Nof(T value) : opt_v(value) {}
		T get() const;
		bool has_value() const;
		Nof<T> operator+(const T&);
		Nof<T> operator-(const T&);
		Nof<T> operator*(const T&);
	private:
		std::optional<T> opt_v;
	};

	template <class T>
	T Nof<T>::get() const
	{
		return opt_v.value();
	}

	template <class T>
	bool rev::Nof<T>::has_value() const
	{
		return opt_v.has_value();
	}

	template <class T>
	Nof<T> Nof<T>::operator+(const T& rhs)
	{
		if (!opt_v.has_value() ||
			(rhs > 0) && (opt_v.value() > std::numeric_limits<T>::max() - rhs) ||
			(rhs < 0) && (opt_v.value() < std::numeric_limits<T>::min() - rhs))
		{
			opt_v.reset();
			return *this;
		}
		return opt_v.value() + rhs;
	}

	template <class T>
	Nof<T> Nof<T>::operator-(const T& rhs)
	{
		if (!opt_v.has_value() ||
			(rhs < 0) && (opt_v.value() > std::numeric_limits<T>::max() + rhs) ||
			(rhs > 0) && (opt_v.value() < std::numeric_limits<T>::min() + rhs))
		{
			opt_v.reset();
			return *this;
		}
		return opt_v.value() - rhs;
	}

	template <class T>
	Nof<T> Nof<T>::operator*(const T& rhs)
	{
		if (!opt_v.has_value() ||
			(opt_v.value() > std::numeric_limits<T>::max() / rhs) ||
			(opt_v.value() < std::numeric_limits<T>::min() / rhs))
		{
			opt_v.reset();
			return *this;
		}
		return opt_v.value() * rhs;
	}
} // namespace rev

#endif // NOF_H

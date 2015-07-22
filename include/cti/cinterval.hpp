#pragma once


namespace cti{
	template <typename T>
	struct trait{
		static constexpr T add_up(const T &x, const T &y)
		{
			return x + y;
		}

		static constexpr T add_down(const T &x, const T &y)
		{
			return x + y;
		}

		static constexpr T sub_up(const T & x, const T &y)
		{
			return x - y;
		}

		static constexpr T sub_down(const T &x, const T &y)
		{
			return x - y;
		}

		static constexpr T mul_up(const T &x, const T &y)
		{
			return x * y;
		}

		static constexpr T mul_down(const T &x, const T &y)
		{
			return x * y;
		}

		static constexpr T div_up(const T &x, const T &y)
		{
			return x / y;
		}

		static constexpr T div_down(const T &x, const T &y)
		{
			return x / y;
		}

		static constexpr void print_up(const T &x, ::std::ostream &os)
		{
			os << x;
		}

		static constexpr void print_down(const T &x, ::std::ostream &os)
		{
			os << x;
		}
	};
}

namespace cti{
	// constexpr interval
	template <typename T>
	struct cinterval{
		T inf, sup;

		constexpr cinterval() = default;

		constexpr cinterval(T x, T y)
			: inf(x), sup(y)
		{
		}

		constexpr cinterval(T x)
			: inf(x), sup(x)
		{
		}

		
	};
}


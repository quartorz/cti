#pragma once

#include <ostream>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <bcl/double.hpp>

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

		static constexpr T sub_up(const T &x, const T &y)
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
	template <typename T>
	struct is_interval : ::std::false_type{
	};

	template <typename Inf, typename Sup>
	struct interval{
		static_assert(::std::is_same<typename Inf::value_type, typename Sup::value_type>{},
		              "Inf and Sup must contain the same type");

		static_assert(::std::is_same<typename Inf::value_type, double>{},
		              "currently, cti::interval supports only double as internal types of Inf and Sup");

		using value_type = typename Inf::value_type;

		operator ::kv::interval<typename Inf::value_type>() const
		{
			double inf = Inf::value, sup = Sup::value;
			return {inf, sup};
		}

		::kv::interval<typename Inf::value_type> to_kv() const
		{
			return *this;
		}

		static constexpr auto whole()
		{
			constexpr auto inf = ::bcl::encode(::std::numeric_limits<double>::infinity());
			constexpr auto sup = ::bcl::encode(-::std::numeric_limits<double>::infinity());
			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		friend ::std::ostream &operator<<(::std::ostream &os, interval)
		{
			constexpr auto inf = Inf::value;
			constexpr auto sup = Sup::value;

			os << '[';
			trait<value_type>::print_down(inf, os);
			os << ',';
			trait<value_type>::print_up(sup, os);
			os<< ']';
			return os;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr auto operator+(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			using common_t = ::std::common_type_t<typename Inf1::value_type, typename Inf2::value_type>;

			constexpr auto inf = ::bcl::encode(trait<common_t>::add_down(Inf1::value, Inf2::value));
			constexpr auto sup = ::bcl::encode(trait<common_t>::add_up(Sup1::value, Sup2::value));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator+(interval, T)
		{
			using common_t = ::std::common_type_t<typename T::value_type, value_type>;

			constexpr auto inf = ::bcl::encode(trait<common_t>::add_down(Inf::value, common_t(T::value)));
			constexpr auto sup = ::bcl::encode(trait<common_t>::add_up(Sup::value, common_t(T::value)));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator+(T x, interval y)
		{
			return y + x;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr auto operator-(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			using common_t = ::std::common_type_t<typename Inf1::value_type, typename Inf2::value_type>;

			constexpr auto inf = ::bcl::encode(trait<common_t>::sub_down(Inf1::value, Inf2::value));
			constexpr auto sup = ::bcl::encode(trait<common_t>::sub_up(Sup1::value, Sup2::value));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator-(interval, T)
		{
			using common_t = ::std::common_type_t<typename T::value_type, value_type>;

			constexpr auto inf = ::bcl::encode(trait<common_t>::sub_down(Inf::value, common_t(T::value)));
			constexpr auto sup = ::bcl::encode(trait<common_t>::sub_up(Sup::value, common_t(T::value)));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator-(T x, interval y)
		{
			return -(y - x);
		}

		friend constexpr auto operator-(interval)
		{
			constexpr auto inf = ::bcl::encode(-Sup::value);
			constexpr auto sup = ::bcl::encode(-Inf::value);
			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <typename T>
		static constexpr ::std::tuple<T, T>
		operator_mul_impl(T inf1, T sup1, T inf2, T sup2)
		{
			return ::std::make_tuple(inf1, sup1);
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr auto operator*(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			using common_t = ::std::common_type_t<typename Inf1::value_type, typename Inf2::value_type>;

			constexpr auto result = operator_mul_impl(
				static_cast<common_t>(Inf1::value), static_cast<common_t>(Sup1::value),
				static_cast<common_t>(Inf2::value), static_cast<common_t>(Sup2::value));

			return result;
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator*(interval, T)
		{
			using common_t = ::std::common_type_t<typename T::value_type, value_type>;

			constexpr auto inf = ::bcl::encode(trait<common_t>::mul_down(Inf::value, common_t(T::value)));
			constexpr auto sup = ::bcl::encode(trait<common_t>::mul_up(Sup::value, common_t(T::value)));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}
	};

	template <typename Inf, typename Sup>
	struct is_interval<interval<Inf, Sup>> : ::std::true_type{
	};
}


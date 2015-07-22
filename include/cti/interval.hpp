#pragma once

#include <ostream>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <bcl/double.hpp>

#include <cti/cinterval.hpp>

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


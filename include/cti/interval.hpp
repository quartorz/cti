#pragma once

#include <ostream>
#include <utility>
#include <stdexcept>

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

	namespace detail{
		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_mul_impl1(const T &inf1, const T &sup1, const T &inf2, const T &sup2);

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_mul_impl2(const T &inf1, const T &sup1, const T &x);

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_div_impl1(const T &inf1, const T &sup1, const T &inf2, const T &sup2);

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_div_impl2(const T &inf1, const T &sup1, const T &y);

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_div_impl3(const T &x, const T &inf2, const T &sup2);
	}

	template <typename Inf, typename Sup>
	struct interval;

	template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
	constexpr bool overlap(interval<Inf1, Sup1>, interval<Inf2, Sup2>);

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

		constexpr value_type lower() const
		{
			return Inf::value;
		}

		constexpr value_type upper() const
		{
			return Sup::value;
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
			using common_t = ::std::common_type_t<typename T::value_type, value_type>;

			constexpr auto inf = ::bcl::encode(trait<common_t>::sub_down(common_t(T::value), Inf::value));
			constexpr auto sup = ::bcl::encode(trait<common_t>::sub_up(common_t(T::value), Sup::value));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
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

			constexpr auto result = detail::interval_operator_mul_impl1(
				static_cast<common_t>(Inf1::value), static_cast<common_t>(Sup1::value),
				static_cast<common_t>(Inf2::value), static_cast<common_t>(Sup2::value));

			constexpr auto inf = ::bcl::encode(::std::get<0>(result));
			constexpr auto sup = ::bcl::encode(::std::get<1>(result));

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

			constexpr auto result = detail::interval_operator_mul_impl2(
				static_cast<common_t>(Inf::value), static_cast<common_t>(Sup::value),
				static_cast<common_t>(T::value));

			constexpr auto inf = ::bcl::encode(::std::get<0>(result));
			constexpr auto sup = ::bcl::encode(::std::get<1>(result));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator*(T x, interval y)
		{
			return y * x;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr auto operator/(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			using common_t = ::std::common_type_t<typename Inf1::value_type, typename Inf2::value_type>;

			constexpr auto result = detail::interval_operator_div_impl1(
				static_cast<common_t>(Inf1::value), static_cast<common_t>(Sup1::value),
				static_cast<common_t>(Inf2::value), static_cast<common_t>(Sup2::value));

			constexpr auto inf = ::bcl::encode(::std::get<0>(result));
			constexpr auto sup = ::bcl::encode(::std::get<1>(result));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator/(interval, T)
		{
			using common_t = ::std::common_type_t<typename T::value_type, value_type>;

			constexpr auto result = detail::interval_operator_div_impl2(
				static_cast<common_t>(Inf::value), static_cast<common_t>(Sup::value),
				static_cast<common_t>(T::value));

			constexpr auto inf = ::bcl::encode(::std::get<0>(result));
			constexpr auto sup = ::bcl::encode(::std::get<1>(result));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr auto operator/(interval, T)
		{
			using common_t = ::std::common_type_t<typename T::value_type, value_type>;

			constexpr auto result = detail::interval_operator_div_impl3(
				static_cast<common_t>(T::value),
				static_cast<common_t>(Inf::value), static_cast<common_t>(Sup::value));

			constexpr auto inf = ::bcl::encode(::std::get<0>(result));
			constexpr auto sup = ::bcl::encode(::std::get<1>(result));

			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr bool operator<(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			return Sup1::value < Inf2::value;
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr bool operator<(interval, T)
		{
			return Sup::value < static_cast<value_type>(T::value);
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr bool operator<(interval, T)
		{
			return static_cast<value_type>(T::value) < Inf::value;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr bool operator<=(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			return Sup1::value <= Inf2::value;
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr bool operator<=(interval, T)
		{
			return Sup::value <= static_cast<value_type>(T::value);
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
				&& !is_interval<T>{}
			>* = nullptr
		>
		friend constexpr bool operator<=(interval, T)
		{
			return static_cast<value_type>(T::value) <= Inf::value;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr bool operator>(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			return Inf1::value > Sup2::value;
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator>(interval, T)
		{
			return Inf::value > static_cast<value_type>(T::value);
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator>(T, interval)
		{
			return static_cast<value_type>(T::value) > Sup::value;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr bool operator>=(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			return Inf1::value >= Sup2::value;
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator>=(interval, T)
		{
			return Inf::value >= static_cast<value_type>(T::value);
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator>=(T, interval)
		{
			return static_cast<value_type>(T::value) >= Sup::value;
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr bool operator==(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
		{
			return Inf1::value == Sup1::value
				&& Sup1::value == Inf2::value
				&& Inf2::value == Sup2::value;
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator==(interval, T)
		{
			return Inf::value == Sup::value
				&& Sup::value == static_cast<value_type>(T::value);
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator==(T, interval)
		{
			return Inf::value == Sup::value
				&& Sup::value == static_cast<value_type>(T::value);
		}

		template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
		friend constexpr bool operator!=(interval<Inf1, Sup1> x, interval<Inf2, Sup2> y)
		{
			return !overlap(x, y);
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator!=(interval x, T)
		{
			constexpr auto value = ::bcl::encode(T::value);
			using y = BCL_DOUBLE(value);

			return !overlap(x, y{});
		}

		template <
			typename T,
			::std::enable_if_t<
				::std::is_convertible<typename T::value_type, value_type>{}
			>* = nullptr
		>
		friend constexpr bool operator!=(T, interval y)
		{
			constexpr auto value = ::bcl::encode(T::value);
			using x = BCL_DOUBLE(value);

			return !overlap(y, x{});
		}

	};

	namespace detail{
		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_mul_impl1(const T &inf1, const T &sup1, const T &inf2, const T &sup2)
		{
			constexpr double infinity = ::std::numeric_limits<T>::infinity();

			using ::sprout::fabs;

			double inf = 0.0, sup = 0.0;

			if(inf1 >= 0.0){
				if(sup1 == 0.0){
					if(fabs(inf2) == infinity || fabs(sup2) == infinity){
						inf = trait<T>::whole().lower();
						sup = trait<T>::whole().upper();
					}
				}else{
					if(inf2 >= 0.0){
						if(sup2 == 0.0){
							if(fabs(inf1) == infinity || fabs(sup1) == infinity){
								inf = trait<T>::whole().lower();
								sup = trait<T>::whole().upper();
							}
						}else{
							inf = trait<T>::mul_down(inf1, inf2);
							sup = trait<T>::mul_up(sup1, sup2);
						}
					}else if(sup2 <= 0.0){
						inf = trait<T>::mul_down(sup1, inf2);
						sup = trait<T>::mul_up(inf1, sup2);
					}else{
						inf = trait<T>::mul_down(sup1, inf2);
						sup = trait<T>::mul_up(sup1, sup2);
					}
				}
			}else if(sup1 <= 0.0){
				if(inf2 >= 0.0){
					if(sup2 == 0.0){
						if(fabs(inf1) == infinity || fabs(sup1) == infinity){
							inf = trait<T>::whole().lower();
							sup = trait<T>::whole().upper();
						}
					}else{
						inf = trait<T>::mul_down(inf1, sup2);
						sup = trait<T>::mul_up(sup1, inf2);
					}
				}else if(sup2 <= 0.0){
					inf = trait<T>::mul_down(sup1, sup2);
					sup = trait<T>::mul_up(inf1, inf2);
				}else{
					inf = trait<T>::mul_down(inf1, sup2);
					sup = trait<T>::mul_up(inf1, inf2);
				}
			}else{
				if(inf2 >= 0.0){
					if(sup2 == 0.0){
						if(fabs(inf1) == infinity || fabs(sup1) == infinity){
							inf = trait<T>::whole().lower();
							sup = trait<T>::whole().upper();
						}
					}else{
						inf = trait<T>::mul_down(inf1, sup2);
						sup = trait<T>::mul_up(sup1, sup2);
					}
				}else if(sup2 <= 0.0){
					inf = trait<T>::mul_down(sup1, inf2);
					sup = trait<T>::mul_up(inf1, inf2);
				}else{
					inf = trait<T>::mul_down(inf1, sup2);
					double tmp = trait<T>::mul_down(sup1, inf2);
					if(tmp < inf)
						inf = tmp;
					sup = trait<T>::mul_up(inf1, inf2);
					tmp = trait<T>::mul_up(sup1, sup2);
					if(tmp > sup)
						sup = tmp;
				}
			}

			return ::std::make_pair(inf, sup);
		}

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_mul_impl2(const T &inf1, const T &sup1, const T &x)
		{
			constexpr auto infinity = ::std::numeric_limits<T>::infinity();

			using ::sprout::fabs;

			double inf = 0.0, sup = 0.0;

			if(x > 0.0){
				inf = trait<T>::mul_down(x, inf1);
				sup = trait<T>::mul_up(x, sup1);
			}else if(x < 0.0){
				inf = trait<T>::mul_down(x, sup1);
				sup = trait<T>::mul_up(x, inf1);
			}else{
				if(fabs(inf1) == infinity || fabs(sup1) == infinity){
					inf = trait<T>::whole().lower();
					sup = trait<T>::whole().upper();
				}
			}

			return ::std::make_pair(inf, sup);
		}

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_div_impl1(const T &inf1, const T &sup1, const T &inf2, const T &sup2)
		{
			double inf = 0.0, sup = 0.0;

			if(inf2 > 0.0){
				if(inf1 >= 0.0){
					inf = trait<T>::div_down(inf1, sup2);
					sup = trait<T>::div_up(sup1, inf2);
				}else if(sup1 <= 0.0){
					inf = trait<T>::div_down(inf1, inf2);
					sup = trait<T>::div_up(sup1, sup2);
				}else{
					inf = trait<T>::div_down(inf1, inf2);
					sup = trait<T>::div_up(sup1, inf2);
				}
			}else if(sup2 < 0.0){
				if(inf1 >= 0.0){
					inf = trait<T>::div_down(sup1, sup2);
					sup = trait<T>::div_up(inf1, inf2);
				}else if(sup1 <= 0.0){
					inf = trait<T>::div_down(sup1, inf2);
					sup = trait<T>::div_up(inf1, sup2);
				}else{
					inf = trait<T>::div_down(sup1, sup2);
					sup = trait<T>::div_up(inf1, sup2);
				}
			}else{
				throw ::std::domain_error("cti::interval: division by 0");
			}

			return ::std::make_pair(inf, sup);
		}

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_div_impl2(const T &inf1, const T &sup1, const T &y)
		{
			double inf = 0.0, sup = 0.0;

			if(y > 0.0){
				inf = trait<T>::div_down(inf1, y);
				sup = trait<T>::div_up(sup1, y);
			}else if(y < 0.0){
				inf = trait<T>::div_down(sup1, y);
				sup = trait<T>::div_up(inf1, y);
			}else{
				throw ::std::domain_error("cti::interval: division by 0");
			}

			return ::std::make_pair(inf, sup);
		}

		template <typename T>
		constexpr ::std::pair<T, T>
		interval_operator_div_impl3(const T &x, const T &inf2, const T &sup2)
		{
			double inf = 0.0, sup = 0.0;

			if(inf2 > 0.0 || sup2 < 0.0){
				if(x >= 0.0){
					inf = trait<T>::div_down(x, sup2);
					sup = trait<T>::div_up(x, inf2);
				}else{
					inf = trait<T>::div_down(x, inf2);
					sup = trait<T>::div_up(x, sup2);
				}
			}else{
				throw ::std::domain_error("cti::interval: division by 0");
			}

			return ::std::make_pair(inf, sup);
		}
	}

	template <typename Inf, typename Sup>
	struct is_interval<interval<Inf, Sup>> : ::std::true_type{
	};

	template <typename Inf1, typename Sup1, typename Inf2, typename Sup2>
	constexpr bool overlap(interval<Inf1, Sup1>, interval<Inf2, Sup2>)
	{
		using common_t = ::std::common_type_t<typename Inf1::type, typename Inf2::type>;

		auto tmp1 = static_cast<common_t>(Inf1::value);

		if(Inf2::value > tmp1)
			tmp1 = Inf2::value;

		auto tmp2 = static_cast<common_t>(Sup1::value);

		if(Sup2::value < tmp2)
			tmp2 = Sup2::value;

		return tmp1 <= tmp2;
	}
}


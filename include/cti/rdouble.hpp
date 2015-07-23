#pragma once

#include <utility>
#include <tuple>
#include <ostream>

#include <sprout/math/fabs.hpp>
#include <sprout/math/ldexp.hpp>
#include <sprout/math/isinf.hpp>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <bcl/sqrt.hpp>

#include <cti/interval.hpp>

namespace cti{
	template <>
	struct trait<double>{
		static constexpr ::std::pair<double, double>
		fasttwosum(double a, double b)
		{
			double x = a + b;
			double tmp = x - a;
			double y = b - tmp;

			return {x, y};
		}

		static constexpr ::std::pair<double, double>
		twosum(double a, double b)
		{
			double x = a + b;
			if(::sprout::fabs(a) > ::sprout::fabs(b)){
				double tmp = x - a;
				double y = b - tmp;
				return {x, y};
			}else{
				double tmp = x - b;
				double y = a - tmp;
				return {x, y};
			}
		}

		static constexpr ::std::pair<double, double>
		split(double a)
		{
			constexpr double sigma = ::sprout::ldexp(1.0, 27) + 1.0;

			double tmp = a * sigma;
			double x = tmp - (tmp - a);
			double y = a - x;

			return {x, y};
		}

		static constexpr ::std::pair<double, double>
		twoproduct(double a, double b)
		{
			constexpr double th = ::sprout::ldexp(1.0, 996);
			constexpr double c1 = ::sprout::ldexp(1.0, -28);
			constexpr double c2 = ::sprout::ldexp(1.0, 28);
			constexpr double th2 = ::sprout::ldexp(1.0, 1023);

			double x = a * b;
			double na = 0.0, nb = 0.0;

			if(::sprout::fabs(a) > th){
				na = a * c1;
				nb = b * c2;
			}else if(::sprout::fabs(b) > th){
				na = a * c2;
				nb = b * c1;
			}else{
				na = a;
				nb = b;
			}

			auto sp1 = split(na);
			double a1 = ::std::get<0>(sp1), a2 = ::std::get<1>(sp1);

			auto sp2 = split(nb);
			double b1 = ::std::get<0>(sp2), b2 = ::std::get<1>(sp2);

			if(::sprout::fabs(x) > th2){
				double y = a2 * b2 - ((((x * 0.5) - (a1 * 0.5)  * b1) * 2.0 - a2 * b1) - a1 * b2);
				return {x, y};
			}else{
				double y = a2 * b2 - (((x - a1 * b1) - a2 * b1) - a1 * b2);
				return {x, y};
			}
		}

		static constexpr double succ(double x)
		{
			constexpr double th1 = ::sprout::ldexp(1.0, -969);
			constexpr double th2 = ::sprout::ldexp(1.0, -1021);
			constexpr double c1 = ::sprout::ldexp(1.0, -53) + ::sprout::ldexp(1.0, -105);
			constexpr double c2 = ::std::numeric_limits<double>::denorm_min();
			constexpr double c3 = ::sprout::ldexp(1.0, 53);
			constexpr double c4 = ::sprout::ldexp(1.0, -53);

			double a = ::sprout::fabs(x);

			if(a >= th1)
				return x + a * c1;
			if(a < th2)
				return x + c2;

			double c = c3 * x;
			double e = c1 * ::sprout::fabs(c);

			return (c + e) * c4;
		}

		static constexpr double pred(double x)
		{
			constexpr double th1 = ::sprout::ldexp(1.0, -969);
			constexpr double th2 = ::sprout::ldexp(1.0, -1021);
			constexpr double c1 = ::sprout::ldexp(1.0, -53) + ::sprout::ldexp(1.0, -105);
			constexpr double c2 = ::std::numeric_limits<double>::denorm_min();
			constexpr double c3 = ::sprout::ldexp(1.0, 53);
			constexpr double c4 = ::sprout::ldexp(1.0, -53);

			double a = ::sprout::fabs(x);
			
			if(a >= th1)
				return x - a * c1;
			if(a < th2)
				return x - c2;

			double c = c3 * x;
			double e = c1 * ::sprout::fabs(c);

			return (c - e) * c4;
		}

		static constexpr double add_up(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();

			auto sum = twosum(x, y);
			double r = ::std::get<0>(sum), r2 = ::std::get<1>(sum);

			if(r == inf){
				return r;
			}else if(r == -inf){
				if(x == -inf || y == -inf)
					return r;
				else
					return -::std::numeric_limits<double>::max();
			}

			if(r2 > 0.0)
				return succ(r);

			return r;
		}

		static constexpr double add_down(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();

			auto sum = twosum(x, y);
			double r = ::std::get<0>(sum), r2 = ::std::get<1>(sum);

			if(r == inf){
				if(x == inf || y == inf)
					return r;
				else
					return ::std::numeric_limits<double>::max();
			}else if(r == -inf)
				return r;

			if(r2 < 0.0)
				return pred(r);

			return r;
		}

		static constexpr double sub_up(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();

			auto sum = twosum(x, -y);
			double r = ::std::get<0>(sum), r2 = ::std::get<1>(sum);

			if(r == inf){
				return r;
			}else if(r == -inf){
				if(x == -inf || y == inf)
					return r;
				else
					return -::std::numeric_limits<double>::max();
			}

			if(r2 > 0.0)
				return succ(r);

			return r;
		}

		static constexpr double sub_down(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();

			auto sum = twosum(x, -y);
			double r = ::std::get<0>(sum), r2 = ::std::get<1>(sum);

			if(r == inf){
				if(x == inf || y == -inf)
					return r;
				else
					return ::std::numeric_limits<double>::max();
			}else if(r == -inf){
				return r;
			}

			if(r2 < 0.0)
				return pred(r);

			return r;
		}

		static constexpr double mul_up(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();
			constexpr double th = ::sprout::ldexp(1.0, -969);
			constexpr double c = ::sprout::ldexp(1.0, 537);

			auto prod = twoproduct(x, y);
			double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

			if(r == inf){
				return r;
			}else if(r == -inf){
				if(::sprout::isinf(x) || ::sprout::isinf(y))
					return r;
				else
					return -::std::numeric_limits<double>::max();
			}

			if(::sprout::fabs(r) >= th){
				if(r2 > 0.0)
					return succ(r);
				return r;
			}else{
				auto rod = twoproduct(x * c, y * c);

				double s = ::std::get<0>(prod), s2 = ::std::get<1>(prod);
				double t = (r * c) * c;

				if(t < s || (t == s && s2 > 0.0)){
					return succ(r);
				}

				return r;
			}
		}

		static constexpr double mul_down(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();
			constexpr double th = ::sprout::ldexp(1.0, -969);
			constexpr double c = ::sprout::ldexp(1.0, 537);

			auto prod = twoproduct(x, y);
			double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

			if(r == inf){
				if(::sprout::fabs(x) == inf || ::sprout::fabs(y) == inf)
					return r;
				else
					return ::std::numeric_limits<double>::max();
			}else if(r == -inf){
				return r;
			}

			if(::sprout::fabs(r) >= th){
				if(r2 < 0.0)
					return pred(r);
				return r;
			}else{
				auto prod = twoproduct(x * c, y * c);

				double s = ::std::get<0>(prod), s2 = ::std::get<1>(prod);
				double t = (r * c) * c;

				if(t > s || (t == s && s2 < 0.0)){
					return pred(r);
				}

				return r;
			}
		}

		static constexpr double div_up(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();
			constexpr double th1 = ::sprout::ldexp(1.0, -969);
			constexpr double th2 = ::sprout::ldexp(1.0, 918);
			constexpr double c1 = ::sprout::ldexp(1.0, 105);
			constexpr double c2 = ::std::numeric_limits<double>::denorm_min();

			if(x == 0.0 || y == 0.0 || ::sprout::fabs(x) == inf || ::sprout::fabs(y) == inf || x != x || y != y)
				return x / y;

			double xn = (y < 0.0 ? -x : x);
			double yn = (y < 0.0 ? -y : y);

			if(::sprout::fabs(xn) < th1){
				if(::sprout::fabs(yn) < th2){
					xn *= c1;
					yn *= c1;
				}else{
					if(xn < 0.0)
						return 0.0;
					else
						return c2;
				}
			}

			double d = xn / yn;

			if(d == inf)
				return d;
			else if(d == -inf)
				return -::std::numeric_limits<double>::max();

			auto prod = twoproduct(d, yn);
			double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

			if(r < xn || ((r == xn) && r2 < 0.0))
				return succ(d);

			return d;
		}

		static constexpr double div_down(double x, double y)
		{
			constexpr double inf = ::std::numeric_limits<double>::infinity();
			constexpr double th1 = ::sprout::ldexp(1.0, -969);
			constexpr double th2 = ::sprout::ldexp(1.0, 918);
			constexpr double c1 = ::sprout::ldexp(1.0, 105);
			constexpr double c2 = ::std::numeric_limits<double>::denorm_min();

			if(x == 0.0 || y == 0.0 || ::sprout::fabs(x) == inf || ::sprout::fabs(y) == inf || x != x || y != y)
				return x / y;

			double xn = (y < 0.0 ? -x : x);
			double yn = (y < 0.0 ? -y : y);

			if(::sprout::fabs(xn) < th1){
				if(::sprout::fabs(yn) < th2){
					xn *= c1;
					yn *= c1;
				}else{
					if(x < 0.0)
						return -c2;
					else
						return 0.0;
				}
			}

			double d = xn / yn;

			if(d == inf)
				return ::std::numeric_limits<double>::max();
			else if(d == -inf)
				return d;

			auto prod = twoproduct(d, yn);
			double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

			if(r > xn || ((r == xn) && r2 > 0.0))
				return pred(d);

			return d;
		}

		static constexpr double sqrt_up(double x)
		{
			constexpr double th1 = ::sprout::ldexp(1.0, -969);
			constexpr double c1 = ::sprout::ldexp(1.0, 106);
			constexpr double c2 = ::sprout::ldexp(1.0, 53);

			double d = ::bcl::sqrt(x);

			if(x < th1){
				double x2 = x * c1;
				double d2 = d * c2;

				auto prod = twoproduct(d2, d2);
				double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

				if(r < x2 || (r == x2 && r2 < 0.0))
					return succ(d);

				return d;
			}

			auto prod = twoproduct(d, d);
			double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

			if(r < x || (r == x && r2 < 0.0))
				return succ(d);

			return d;
		}

		static constexpr double sqrt_down(double x)
		{
			constexpr double th1 = ::sprout::ldexp(1.0, -969);
			constexpr double c1 = ::sprout::ldexp(1.0, 106);
			constexpr double c2 = ::sprout::ldexp(1.0, 53);

			double d = ::bcl::sqrt(x);

			if(x < th1){
				double x2 = x * c1;
				double d2 = d * c2;

				auto prod = twoproduct(d2, d2);
				double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

				if(r > x2 || (r == x2 && r2 > 0.0))
					return pred(d);

				return d;
			}

			auto prod = twoproduct(d, d);
			double r = ::std::get<0>(prod), r2 = ::std::get<1>(prod);

			if(r > x || (r == x && r2 > 0.0))
				return pred(d);

			return d;
		}

		static void print_up(double x, ::std::ostream &os)
		{
			::kv::rop<double>::print_up(x, os);
		}

		static void print_down(double x, ::std::ostream &os)
		{
			::kv::rop<double>::print_down(x, os);
		}

		static constexpr auto whole()
		{
			constexpr auto infinity = ::std::numeric_limits<double>::infinity();
			constexpr auto inf = ::bcl::encode(-infinity);
			constexpr auto sup = ::bcl::encode(infinity);
			return interval<BCL_DOUBLE(inf), BCL_DOUBLE(sup)>{};
		}
	};
}


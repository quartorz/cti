#pragma once

#include <sprout/string.hpp>

#include <bcl/double.hpp>
#include <bcl/string.hpp>

#include <cti/interval.hpp>

namespace cti{
	namespace literals{
		namespace double_interval{
			template <char ... Chars>
			constexpr auto operator "" _di()
			{
				constexpr auto str = ::sprout::make_string(Chars...);
				constexpr double d1 = ::bcl::stod(str, nullptr, -1);
				constexpr double d2 = ::bcl::stod(str, nullptr, 1);
				constexpr auto e1 = ::bcl::encode(d1);
				constexpr auto e2 = ::bcl::encode(d2);

				using inf_type = BCL_DOUBLE(e1);
				using sup_type = BCL_DOUBLE(e2);

				return ::cti::interval<inf_type, sup_type>{};
			}
		}

		using double_interval::operator "" _di;
	}
}


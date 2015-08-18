#pragma once

#include <type_traits>

#include <bcl/double.hpp>

#include <cti/interval.hpp>

namespace cti{
	namespace operators{
		template <
			typename Inf, typename Sup,
			::std::enable_if_t<
				::bcl::is_encoded_double_v<Inf>
				&& ::bcl::is_encoded_double_v<Sup>
			>* = nullptr
		>
		constexpr auto operator,(Inf, Sup)
		{
			return ::cti::interval<Inf, Sup>{};
		}
	}
}


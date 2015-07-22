#include <iostream>

#include <cti/interval.hpp>
#include <cti/rdouble.hpp>

#include <typeinfo>
#include <cxxabi.h>

template <typename T>
const char *name()
{
	return abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr);
}

template <typename T>
const char *name(T)
{
	return name<T>();
}

int main()
{
	std::cout.setf(std::ios::scientific);
	std::cout.precision(16);

	cti::interval<D_T(0.1), D_T(0.9)> x;
	cti::interval<D_T(-0.4), D_T(3.0)> y;

	std::cout << x << std::endl;
	std::cout << x << " + " << y << " = " << x + y << std::endl << std::endl;

	std::cout << name(x) << std::endl;
	std::cout << name(x) << " + " << name(y) << " = " << name(x + y) << std::endl << std::endl;

	std::cout << x.to_kv() << std::endl;
	std::cout << x.to_kv() << " + " << y.to_kv() << " = " << x.to_kv() + y.to_kv() << std::endl << std::endl;

	std::cout << std::get<0>(x * y);
}


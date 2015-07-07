// Description: Testing for SFINAE
// DocumentationOf: sfinae.h

#include "test/test_init.h"

#include <pastel/sys/sfinae.h>

namespace
{

	// SFINAE in the default template parameter value.
	//
	// Disadvantages:
	// * extraneous  = 0. 

	template <
		integer N,
		RequiresC<N == 1> = 0>
	std::true_type f();

	template <
		integer N,
		RequiresC<N != 1> = 0>
	std::false_type f();

}

TEST_CASE("DefaultTemplateParameterValue (sfinae)")
{
	PASTEL_STATIC_ASSERT(!decltype(f<0>())::value);
	PASTEL_STATIC_ASSERT(decltype(f<1>())::value);
}

namespace
{

	// SFINAE in the default template parameter type.

	template <
		integer N,
		typename = RequiresC<N == 1>>
	std::true_type g();

	template <
		integer N,
		typename = RequiresC<N != 1>>
	std::false_type g();

}

TEST_CASE("DefaultTemplateParameterType (sfinae)")
{
	PASTEL_STATIC_ASSERT(!decltype(g<0>())::value);
	PASTEL_STATIC_ASSERT(decltype(g<1>())::value);
}

namespace
{
		
	// SFINAE in the return type.
	//
	// Disadvantages:
	// * garbles the return type.

	template <integer N>
	EnableIfC<N == 1, std::true_type> h();

	template <integer N>
	EnableIfC<N != 1, std::false_type> h();

}

TEST_CASE("ReturnType (sfinae)")
{
	// Visual Studio 2013: pass
	// Visual Studio 2013 Compiler Nov 2013 CTP: fail
	// Visual Studio 14 CTP4: pass
	PASTEL_STATIC_ASSERT(!decltype(h<0>())::value);
	PASTEL_STATIC_ASSERT(decltype(h<1>())::value);
}

namespace
{

	// SFINAE in the parameter
	//
	// Disadvantages:
	// * garbles the parameter list

	template <integer N>
	std::true_type k(RequiresC<N == 1> = 0);

	template <integer N>
	std::false_type k(RequiresC<N != 1> = 0);

}

TEST_CASE("Parameter (sfinae)")
{
	PASTEL_STATIC_ASSERT(!decltype(k<0>())::value);
	PASTEL_STATIC_ASSERT(decltype(k<1>())::value);
}

namespace
{

	// Complex SFINAE constraint

	template <
		typename Type,
		Requires<
			Or<
				And<
					std::is_same<float, float>,
					std::is_same<Type, float>,
					std::is_same<float, Type>,
					std::is_same<Type, Type>,
					std::is_fundamental<Type>
				>,
				And<
					std::is_same<int, int>,
					std::is_same<Type, int>,
					std::is_same<int, Type>,
					std::is_same<Type, Type>,
					std::is_integral<Type>
				>
			>,
			std::is_same<Type, Type>
		> = 0
	>
	std::true_type m(Type);

	std::false_type m(...);
}

TEST_CASE("Complex (sfinae)")
{
	PASTEL_STATIC_ASSERT(decltype(m(1.0f))::value);
	PASTEL_STATIC_ASSERT(decltype(m(1))::value);
	PASTEL_STATIC_ASSERT(!decltype(m('a'))::value);
}

namespace
{

	class A
	{
	public:
		// SFINAE in the default template parameter value.

		template <
			integer N,
			RequiresC<N == 1> = 0>
		std::true_type f();

		template <
			integer N,
			RequiresC<N != 1> = 0>
		std::false_type f();

		// SFINAE in the default template parameter type.

		template <
			integer N,
			typename = RequiresC<N == 1>>
		std::true_type g();

		template <
			integer N,
			typename = RequiresC<N != 1 >>
		std::false_type g();

		// SFINAE in the return type.

		template <integer N>
		EnableIfC<N == 1, std::true_type> h();

		template <integer N>
		EnableIfC<N != 1, std::false_type> h();

		// SFINAE in the parameter

		template <integer N>
		std::true_type k(RequiresC<N == 1> = 0);

		template <integer N>
		std::false_type k(RequiresC<N != 1> = 0);
	};

}

TEST_CASE("Class (sfinae)")
{
	A a;

	static_assert(std::is_same<decltype(a.f<0>()), std::false_type>::value, "fail");
	static_assert(std::is_same<decltype(a.f<1>()), std::true_type>::value, "fail");

	static_assert(std::is_same<decltype(a.g<0>()), std::false_type>::value, "fail");
	static_assert(std::is_same<decltype(a.g<1>()), std::true_type>::value, "fail");

	// Visual Studio 2013: pass
	// Visual Studio 2013 Compiler Nov 2013 CTP: fail
	// Visual Studio 14 CTP4: pass
	static_assert(std::is_same<decltype(a.h<0>()), std::false_type>::value, "fail");
	static_assert(std::is_same<decltype(a.h<1>()), std::true_type>::value, "fail");

	PASTEL_STATIC_ASSERT((std::is_same<decltype(a.k<0>()), std::false_type>::value));
	PASTEL_STATIC_ASSERT((std::is_same<decltype(a.k<1>()), std::true_type>::value));

	a;
}


// Description: Testing for SFINAE
// DocumentationOf: sfinae.h

#include "test_pastelsys.h"

#include <pastel/sys/sfinae.h>

using namespace Pastel;

namespace
{

	class A
	{
	public:
		// SFINAE in the default template parameter value.

		template <
			int N,
			RequiresC<N == 1> = 0>
		std::true_type f();

		template <
			int N,
			RequiresC<N != 1> = 0>
		std::false_type f();

		// SFINAE in the default template parameter type.

		template <
			int N,
			typename = RequiresC<N == 1>>
		std::true_type g();

		template <
			int N,
			typename = RequiresC<N != 1 >>
		std::false_type g();

		// SFINAE in the return type.

		template <int N>
		EnableIfC<N == 1, std::true_type> h();

		template <int N>
		EnableIfC<N != 1, std::false_type> h();

		// SFINAE in the parameter
	
		template <int N>
		std::true_type k(RequiresC<N == 1> = 0);

		template <int N>
		std::false_type k(RequiresC<N != 1> = 0);
	};

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testDefaultTemplateParameterValue();
			testDefaultTemplateParameterType();
			testReturnType();
			testParameter();
			testComplex();
			testClass();
		}

		// SFINAE in the default template parameter value.
		//
		// Disadvantages:
		// * extraneuous  = 0. 
		//
		// The nullptr cannot be changed to 0; this is a 
		// special rule for template parameters.

		template <
			int N,
			RequiresC<N == 1> = 0>
		std::true_type f();

		template <
			int N,
			RequiresC<N != 1> = 0>
		std::false_type f();

		void testDefaultTemplateParameterValue()
		{
			PASTEL_STATIC_ASSERT(!decltype(f<0>())::value);
			PASTEL_STATIC_ASSERT(decltype(f<1>())::value);
		}

		// SFINAE in the default template parameter type.

		template <
			int N,
			typename = RequiresC<N == 1>>
		std::true_type g();

		template <
			int N,
			typename = RequiresC<N != 1>>
		std::false_type g();
		
		void testDefaultTemplateParameterType()
		{
			PASTEL_STATIC_ASSERT(!decltype(g<0>())::value);
			PASTEL_STATIC_ASSERT(decltype(g<1>())::value);
		}

		// SFINAE in the return type.
		//
		// Disadvantages:
		// * garbles the return type.

		template <int N>
		EnableIfC<N == 1, std::true_type> h();

		template <int N>
		EnableIfC<N != 1, std::false_type> h();
		
		void testReturnType()
		{
			// Visual Studio 2013: pass
			// Visual Studio 2013 Compiler Nov 2013 CTP: fail
			// Visual Studio 14 CTP4: pass
			PASTEL_STATIC_ASSERT(!decltype(h<0>())::value);
			PASTEL_STATIC_ASSERT(decltype(h<1>())::value);
		}

		// SFINAE in the parameter
		//
		// Disadvantages:
		// * garbles the parameter list

		template <int N>
		std::true_type k(RequiresC<N == 1> = 0);

		template <int N>
		std::false_type k(RequiresC<N != 1> = 0);

		void testParameter()
		{
			PASTEL_STATIC_ASSERT(!decltype(k<0>())::value);
			PASTEL_STATIC_ASSERT(decltype(k<1>())::value);
		}

		// Complex SFINAE constraint

		template <
			typename Type,
			Requires<
				Or<
					And<
						std::is_same<void, void>,
						std::is_same<Type, void>,
						std::is_same<void, Type>,
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
		std::true_type m();

		template <typename Type>
		std::false_type m(...);

		void testComplex()
		{
			PASTEL_STATIC_ASSERT(decltype(m<void>())::value);
			PASTEL_STATIC_ASSERT(decltype(m<int>())::value);
		}

		void testClass()
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
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("sfinae", test);
	}

	CallFunction run(addTest);

}

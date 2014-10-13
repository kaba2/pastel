// Description: Testing for SFINAE
// DocumentationOf: sfinae_macros.h

#include "test_pastelsys.h"

#include <pastel/sys/sfinae_macros.h>

using namespace Pastel;

namespace
{

	struct True;
	struct False;

	// SFINAE in the default template parameter value.
	//
	// Disadvantages:
	// * extraneuous  = 0. 
	//
	// The nullptr cannot be changed to 0; this is a 
	// special rule for template parameters.

	template <
		int N,
		EnableIfC<N == 1> = 0>
	True f();

	template <
		int N,
		EnableIfC<N != 1> = 0>
	False f();

	// SFINAE in the default template parameter type.
	// 
	// Disadvantages:
	// * If the function declarations are the same,
	// then this style cannot be used for overloading
	// (e.g if the return type of g was void instead).

	template <
		int N,
		typename = EnableIfC<N == 1>>
	True g();

	template <
		int N,
		typename = EnableIfC<N != 1>>
	False g();

	// SFINAE in the return type.
	//
	// Disadvantages:
	// * garbles the return type.

	template <int N>
	EnableIfC<N == 1, True> h();

	template <int N>
	EnableIfC<N != 1, False> h();

	// SFINAE in the parameter
	//
	// Disadvantages:
	// * garbles the parameter list

	template <int N>
	True k(EnableIfC<N == 1> = 0);

	template <int N>
	False k(EnableIfC<N != 1> = 0);

	class A
	{
	public:
		// SFINAE in the default template parameter value.

		template <
			int N,
			EnableIfC<N == 1> = 0>
		True f();

		template <
			int N,
			EnableIfC<N != 1> = 0>
		False f();

		// SFINAE in the default template parameter type.

		template <
			int N,
			typename = EnableIfC<N == 1>>
		True g();

		template <
			int N,
			typename = EnableIfC<N != 1 >>
		False g();

		// SFINAE in the return type.

		template <int N>
		EnableIfC<N == 1, True> h();

		template <int N>
		EnableIfC<N != 1, False> h();

		// SFINAE in the parameter
	
		template <int N>
		True k(EnableIfC<N == 1> = 0);

		template <int N>
		False k(EnableIfC<N != 1> = 0);
	};

	/*
	template <int N_>
	class F
	{
	public:
		template <
			int N = N_,
			typename = EnableIfC<N == 1>>
		F();

		template <
			int N = N_,
			typename = EnableIfC<N != 1>>
		F();
	};

	template <int N_>
	class G
	{
	public:
		template <
			int N = N_,
			typename = EnableIfC<N == 1>>
		G();

		template <
			int N = N_,
			typename = EnableIfC<N != 1>>
		G();
	};

	template <int N_>
	class H
	{
	public:
		template <int N = N_>
		H(typename = EnableIfC<N == 1>);

		template <int N = N_>
		H(typename = EnableIfC<N != 1>);
	};
	*/

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
			test();
			testConstructor();
			testClass();
		}

		void test()
		{
			PASTEL_STATIC_ASSERT((std::is_same<decltype(f<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(f<1>()), True>::value));

			PASTEL_STATIC_ASSERT((std::is_same<decltype(g<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(g<1>()), True>::value));

			// Visual Studio 2013: pass
			// Visual Studio 2013 Compiler Nov 2013 CTP: fail
			// Visual Studio 14 CTP4: pass
			PASTEL_STATIC_ASSERT((std::is_same<decltype(h<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(h<1>()), True>::value));

			PASTEL_STATIC_ASSERT((std::is_same<decltype(k<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(k<1>()), True>::value));
		}

		void testConstructor()
		{
			/*
			F<0>();
			F<1>();

			G<0>();
			G<1>();

			H<0>();
			H<1>();
			*/
		}

		void testClass()
		{
			A a;

			static_assert(std::is_same<decltype(a.f<0>()), False>::value, "fail");
			static_assert(std::is_same<decltype(a.f<1>()), True>::value, "fail");

			static_assert(std::is_same<decltype(a.g<0>()), False>::value, "fail");
			static_assert(std::is_same<decltype(a.g<1>()), True>::value, "fail");

			// Visual Studio 2013: pass
			// Visual Studio 2013 Compiler Nov 2013 CTP: fail
			// Visual Studio 14 CTP4: pass
			static_assert(std::is_same<decltype(a.h<0>()), False>::value, "fail");
			static_assert(std::is_same<decltype(a.h<1>()), True>::value, "fail");

			PASTEL_STATIC_ASSERT((std::is_same<decltype(a.k<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(a.k<1>()), True>::value));

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

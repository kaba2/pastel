// Description: Testing for SFINAE
// DocumentationOf: sfinae_macros.h

#include "test_pastelsys.h"

using namespace Pastel;

namespace
{

	struct True;
	struct False;

	// SFINAE in the default template parameter value.

	template <
		int N,
		EnableIfC<N == 1>* = 0>
	True f();

	template <
		int N,
		EnableIfC<N != 1>* = 0>
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

	template <int N_>
	class F
	{
	public:
		template <
			int N = N_,
			EnableIfC<N == 1>* = 0>
		F();

		template <
			int N = N_,
			EnableIfC<N != 1>* = 0>
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
		H(EnableIfC<N == 1>* = 0);

		template <int N = N_>
		H(EnableIfC<N != 1>* = 0);
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
			test();
		}

		void test()
		{
			PASTEL_STATIC_ASSERT((std::is_same<decltype(f<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(f<1>()), True>::value));

			PASTEL_STATIC_ASSERT((std::is_same<decltype(g<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(g<1>()), True>::value));

			PASTEL_STATIC_ASSERT((std::is_same<decltype(h<0>()), False>::value));
			PASTEL_STATIC_ASSERT((std::is_same<decltype(h<1>()), True>::value));

			F<0>();
			F<1>();

			G<0>();
			G<1>();

			H<0>();
			H<1>();
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

// Description: Testing for named parameters
// Documentation: named_parameter.txt

#include "test_pastelsys.h"

#include <pastel/sys/function/identity_function.h>

#include <string>

using namespace Pastel;

namespace
{

	struct Euclidean_Metric
	{
		template <typename Type>
		decltype(auto) operator()(Type x, Type y) const
		{
			return ((x - y) * (x - y));
		}
	};

	struct Manhattan_Metric
	{
		template <typename Type>
		decltype(auto) operator()(Type x, Type y) const
		{
			return std::abs(x - y);
		}
	};

	template <
		typename Point,
		typename... ArgumentSet,
		typename Metric = PASTEL_ARG_T(metric, Euclidean_Metric),
		Requires<
			std::is_integral<Point>
		> = 0
	>
	float distance(const Point& a, const Point& b, ArgumentSet&&... argumentSet)
	{
		float scaling = PASTEL_ARG_S(scaling, 1);
		auto&& metric = PASTEL_ARG(metric, ([&](){return Euclidean_Metric();}));
		bool negate = PASTEL_ARG_S(negate, false);

		return metric(a, b) * scaling * (negate ? -1 : 1);
	}

}

namespace
{

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
			TEST_ENSURE_OP(distance(1, 6), ==, 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 2), ==, 2 * 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 3.5), ==, 3.5 * 5 * 5);
			//TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(negate)), ==, -5 * 5);

			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(metric), Manhattan_Metric()), ==, 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 2, PASTEL_TAG(negate), PASTEL_TAG(metric), Manhattan_Metric()), ==, 2 * (-1) * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 3.5, PASTEL_TAG(metric), Manhattan_Metric()), ==, 3.5 * 5);

			// These should give errors at compile-time.
			//TEST_ENSURE_OP(distance(1, 6, 1), ==, 5 * 5);
			//TEST_ENSURE_OP(distance(1, 6, 1, 2), ==, 5 * 5);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("named_parameter", test);
	}

	CallFunction run(addTest);

}
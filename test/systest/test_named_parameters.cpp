
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
		Requires<
			std::is_integral<Point>
		> = 0
	>
	float distance(
		const Point& a, const Point& b, 
		ArgumentSet&&... argumentSet)
	{
		float scaling = 
			PASTEL_ARG(
				scaling,
				[](){return 1.0;},
				[](auto input) {return std::is_floating_point<decltype(input)>();}
			);
		
		auto&& metric = 
			PASTEL_ARG(
				metric, 
				[](){return Euclidean_Metric();}, 
				[](auto input) {return
					Or<
						std::is_same<decltype(input), Euclidean_Metric>,
						std::is_same<decltype(input), Manhattan_Metric>
					>();} 
			);
		
		bool negate = 
			PASTEL_ARG(
				negate, 
				[](){return false;},
				[](auto input) {return std::is_same<decltype(input), bool>();} 
			);

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

		struct A {};

		virtual void run()
		{
			TEST_ENSURE_OP(distance(1, 6), ==, 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 2.0), ==, 2 * 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 3.5), ==, 3.5 * 5 * 5);

			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(negate)), ==, (-1) * 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, Manhattan_Metric()), ==, 5);
			TEST_ENSURE_OP(distance(1, 6, Manhattan_Metric(), true), ==, -5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(metric), Manhattan_Metric()), ==, 5);
			//TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 2.0, PASTEL_TAG(negate), PASTEL_TAG(metric), Manhattan_Metric()), ==, 2 * (-1) * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 3.5, PASTEL_TAG(metric), Manhattan_Metric()), ==, 3.5 * 5);

			// These should give errors at compile-time.
			
			// Metric is required to be either Manhattan_Metric or Euclidean_Metric.
			//TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(metric), A()), ==, 5 * 5);
			//TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(metric)), ==, 5 * 5);
			// Negate is required to be of type bool.
			//TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(negate), 4.0f), ==, 5 * 5);
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
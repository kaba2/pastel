// Description: Testing for named parameters
// Documentation: named_parameter.txt

#include "test_pastelsys.h"

#include <pastel/sys/function/identity_function.h>

#include <string>

using namespace Pastel;

namespace
{

	struct Offset
	{
		explicit Offset(integer id_ = 0)
			: amount(id_)
		{
		}

		Offset(const Offset& that)
			: amount(0)
		{
		}

		Offset(Offset&& that)
			: amount(that.amount)
		{
			that.amount = 0;
		}
		
		integer amount;
	};

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
				[](auto input) 
				{
					return std::is_convertible<decltype(input), float>();
				}
			);
		
		auto&& metric = 
			PASTEL_ARG(
				metric, 
				[](){return Euclidean_Metric();}, 
				[](auto input) {return
					implicitArgument(Or<
						std::is_same<decltype(input), Euclidean_Metric>,
						std::is_same<decltype(input), Manhattan_Metric>
					>());} 
			);
		
		bool negate = 
			PASTEL_ARG(
				negate, 
				[](){return false;},
				[](auto input) {return std::is_same<decltype(input), bool>();} 
			);

		auto offset =
			PASTEL_ARG_S(offset, Offset());
				
		return metric(a, b) * scaling * (negate ? -1 : 1) + offset.amount;
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
			// The tag hashing uses the Fowler-Noll-Vo 1a hash-function 
			// (FNV-1a) for 32-bit integers. Check the hashes of some
			// known strings. Initially I had a bug in the implementation 
			// which I noticed only because I had a hash collision between
			// these strings.
			PASTEL_STATIC_ASSERT("translation"_tag == 3419592236UL);
			PASTEL_STATIC_ASSERT("orientation"_tag == 3309681697UL);

			TEST_ENSURE_OP(distance(1, 6), ==, 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 2.0), ==, 2 * 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 3.5), ==, 3.5 * 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling)), ==, 1 * 5 * 5);

			{
				// Test that arguments are perfectly forwarded; moved
				// objects should be moved.
				Offset offset(543);
				TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(offset), std::move(offset)), ==, 5 * 5 + 543);
				TEST_ENSURE_OP(offset.amount, ==, 0);
			}

			{
				// Test that arguments are perfectly forwarded; non-moved
				// objects should not be moved.
				Offset offset(543);
				TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(offset), offset), ==, 5 * 5 + 0);
				TEST_ENSURE_OP(offset.amount, ==, 543);
			}

			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(negate)), ==, (-1) * 5 * 5);
			TEST_ENSURE_OP(distance(1, 6, Manhattan_Metric()), ==, 5);
			// Since 'negate' is not an implicit parameter,
			// the 'true' will not bind to it.
			TEST_ENSURE_OP(distance(1, 6, Manhattan_Metric(), true), ==, 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(metric), Manhattan_Metric()), ==, 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 2.0, PASTEL_TAG(negate), PASTEL_TAG(metric), Manhattan_Metric()), ==, 2 * (-1) * 5);
			TEST_ENSURE_OP(distance(1, 6, PASTEL_TAG(scaling), 3.5, PASTEL_TAG(metric), Manhattan_Metric()), ==, 3.5 * 5);

			// These should give errors at compile-time.

			// Error: Multiple arguments for 'negate'.
			//distance(1, 6, PASTEL_TAG(negate), PASTEL_TAG(negate));
			//distance(1, 6, PASTEL_TAG(scaling));
			//distance(1, 6, true, true);

			// Error: 'metric' is required to be either Manhattan_Metric or Euclidean_Metric.
			//distance(1, 6, PASTEL_TAG(metric), A());
			//distance(1, 6, PASTEL_TAG(metric));

			// Error: 'negate' is required to be of type bool.
			//distance(1, 6, PASTEL_TAG(negate), 4.0f);
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
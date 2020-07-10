// Description: Testing for named parameters
// Documentation: named_parameter.txt

#include "test/test_init.h"

#include <pastel/sys/function/identity_function.h>

#include <string>

namespace
{

	enum class Enum
	{
		Off,
		On
	};

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
		typename... ArgumentSet
	>
	requires std::is_integral_v<Point>
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

		auto offset =
			PASTEL_ARG_S(offset, Offset());

		Enum enumValue =
			PASTEL_ARG_ENUM(enumValue, Enum::Off);

		return metric(a, b) * scaling * (negate ? -1 : 1) + offset.amount + (integer)enumValue;
	}

	struct A {};

}

TEST_CASE("Tag (named_parameter)")
{
	// The tag hashing uses the Fowler-Noll-Vo 1a hash-function 
	// (FNV-1a) for 32-bit integers. Check the hashes of some
	// known strings. Initially I had a bug in the implementation 
	// which I noticed only because I had a hash collision between
	// these strings.
	PASTEL_STATIC_ASSERT(tagHash("translation") == 3419592236UL);
	PASTEL_STATIC_ASSERT(tagHash("orientation") == 3309681697UL);

	PASTEL_STATIC_ASSERT("translation"_tag == 3419592236UL);
	PASTEL_STATIC_ASSERT("orientation"_tag == 3309681697UL);

	// These are the only english words which collide under the
	// FNV1a hash function.
	PASTEL_STATIC_ASSERT(tagHash("liquid") == tagHash("costarring"));
	PASTEL_STATIC_ASSERT("liquid"_tag == "costarring"_tag);
}

TEST_CASE("Empty (Empty)")
{
	REQUIRE(distance(1, 6) == 5 * 5);
}

TEST_CASE("Explicit (named_parameter)")
{
	REQUIRE(distance(1, 6, PASTEL_TAG(scaling), 2.0) == 2 * 5 * 5);
	REQUIRE(distance(1, 6, PASTEL_TAG(scaling), 3.5) == 3.5 * 5 * 5);
	REQUIRE(distance(1, 6, PASTEL_TAG(metric), Manhattan_Metric()) == 5);
	REQUIRE(distance(1, 6, PASTEL_TAG(scaling), 3.5, PASTEL_TAG(metric), Manhattan_Metric()) == 3.5 * 5);
}

TEST_CASE("Enum (named_parameter)")
{
	REQUIRE(distance(1, 6, PASTEL_TAG(enumValue), Enum::Off) == 5 * 5 + 0);
	REQUIRE(distance(1, 6, PASTEL_TAG(enumValue), Enum::On) == 5 * 5 + 1);
}

TEST_CASE("Forwarding (named_parameter)")
{
	// Test that arguments are perfectly forwarded
	{
		Offset offset(543);
		REQUIRE(distance(1, 6, PASTEL_TAG(offset), std::move(offset)) == 5 * 5 + 543);
		REQUIRE(offset.amount == 0);
	}

	{
		Offset offset(543);
		REQUIRE(distance(1, 6, PASTEL_TAG(offset), offset) == 5 * 5 + 0);
		REQUIRE(offset.amount == 543);
	}
}

TEST_CASE("Flag (named_parameter)")
{
	REQUIRE(distance(1, 6, PASTEL_TAG(negate)) == (-1) * 5 * 5);
	REQUIRE(distance(1, 6, PASTEL_TAG(scaling)) == 1 * 5 * 5);
	REQUIRE(distance(1, 6, PASTEL_TAG(scaling), 2.0, PASTEL_TAG(negate), PASTEL_TAG(metric), Manhattan_Metric()) == 2 * (-1) * 5);
}

TEST_CASE("Erroneous (named_parameter)")
{
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


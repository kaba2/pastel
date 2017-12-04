// Description: Testing for distances
// DocumentationOf: pastel/math/distance.h

#include "test/test_init.h"

#include "pastel/math/distance.h"

template <typename Distance>
void testBasic(Distance distance)
{
	auto copy = distance;
	REQUIRE(copy == distance);
	REQUIRE((real)copy == (real)distance);
}

TEST_CASE("Distance (Manhattan)")
{
	auto distance = Manhattan_Distance<real>();
	testBasic(distance);

	distance.set(0, 2);
	REQUIRE((real)distance == 2);

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == 4);
}

TEST_CASE("Distance (Euclidean)")
{
	auto distance = Euclidean_Distance<real>();
	testBasic(distance);

	distance.set(0, 2);
	REQUIRE((real)distance == std::sqrt(2*2));

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == std::sqrt(4*4));
}

TEST_CASE("Distance (Maximum)")
{
	auto distance = Maximum_Distance<real>();
	testBasic(distance);

	distance.set(0, 2);
	REQUIRE((real)distance == 2);

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == 4);
}

TEST_CASE("Distance (Product)")
{
	auto distance = Product_Distance<real, Euclidean_Distance<real>, Manhattan_Distance<real>>(2);
	testBasic(distance);

	distance.set(0, 2);
	REQUIRE((real)distance == std::sqrt(2*2));

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == std::sqrt(4*4));

	distance.set(1, 2);
	REQUIRE((real)distance == std::sqrt(4*4 + 2*2));

	distance.set(2, 100);
	REQUIRE((real)distance == std::max(std::sqrt(4*4 + 2*2), (real)100));
}


// Description: Testing for distances
// Documentation: unit_testing.txt

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
	auto distance = ManhattanDistance<real>(2);
	testBasic(distance);
	REQUIRE(distance.n() == 2);

	distance.set(0, 2);
	REQUIRE((real)distance == 2);

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == 4);
}

TEST_CASE("Distance (Euclidean)")
{
	auto distance = EuclideanDistance<real>(2);
	testBasic(distance);
	REQUIRE(distance.n() == 2);

	distance.set(0, 2);
	REQUIRE((real)distance == std::sqrt(2*2));

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == std::sqrt(4*4));
}

TEST_CASE("Distance (Maximum)")
{
	auto distance = MaximumDistance<real>(2);
	testBasic(distance);
	REQUIRE(distance.n() == 2);

	distance.set(0, 2);
	REQUIRE((real)distance == 2);

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == 4);
}

TEST_CASE("Distance (Product)")
{
	auto distance = productDistance<real>(
		EuclideanDistance<real, 2>(),
		ManhattanDistance<real, 2>());
	testBasic(distance);
	REQUIRE(distance.n() == 4);

	distance.set(0, 2);
	REQUIRE((real)distance == std::sqrt(2*2));

	distance.replace(0, 2, 4);
	REQUIRE((real)distance == std::sqrt(4*4));

	distance.set(1, 2);
	REQUIRE((real)distance == std::sqrt(4*4 + 2*2));

	distance.set(2, 100);
	REQUIRE((real)distance == std::max(std::sqrt(4*4 + 2*2), (real)100));
}


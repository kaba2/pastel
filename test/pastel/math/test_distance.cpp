// Description: Testing for distances
// DocumentationOf: pastel/math/distance.h

#include "test/test_init.h"

#include "pastel/math/distance.h"

template <typename Distance>
void testBasic()
{
	auto distance = Distance((real)2);
	auto copy = distance;
	REQUIRE(copy == distance);
	REQUIRE(copy <= distance);
	REQUIRE(copy >= distance);
	REQUIRE(!(copy != distance));
	REQUIRE(!(copy < distance));
	REQUIRE(!(copy > distance));
	
	REQUIRE(copy == 2);
	REQUIRE(2 == copy);
}

TEST_CASE("Distance (Manhattan)")
{
	using Distance = Manhattan_Distance<real>;
	PASTEL_CONCEPT_CHECK(Distance, Distance_Concept);

	auto distance = Distance();
	testBasic<Distance>();

	distance.set(0, 2);
	REQUIRE(~distance == 2);

	distance.replace(0, 2, 4);
	REQUIRE(~distance == 4);
}

TEST_CASE("Distance (Euclidean)")
{
	using Distance = Euclidean_Distance<real>;
	PASTEL_CONCEPT_CHECK(Distance, Distance_Concept);

	auto distance = Distance();
	testBasic<Distance>();

	distance.set(0, 2);
	REQUIRE(~distance == 2*2);

	distance.replace(0, 2, 4);
	REQUIRE(~distance == 4*4);
}

TEST_CASE("Distance (Maximum)")
{
	using Distance = Maximum_Distance<real>;
	PASTEL_CONCEPT_CHECK(Distance, Distance_Concept);

	auto distance = Distance();
	testBasic<Distance>();

	distance.set(0, 2);
	REQUIRE(~distance == 2);

	distance.replace(0, 2, 4);
	REQUIRE(~distance == 4);
}

TEST_CASE("Distance (Minkowski)")
{
	using Distance = Minkowski_Distance<real>;
	PASTEL_CONCEPT_CHECK(Distance, Distance_Concept);

	auto distance = Distance();
	testBasic<Distance>();

	distance.set(0, 2);
	REQUIRE(~distance == 2*2);

	distance.replace(0, 2, 4);
	REQUIRE(~distance == 4*4);
}

TEST_CASE("Distance (Product)")
{
	using Distance = Product_Distance<real, Euclidean_Distance<real>, Manhattan_Distance<real>>;

	auto distance = Distance((integer)2);
	testBasic<Distance>();

	distance.set(0, 2);
	REQUIRE(~distance == 2*2);

	distance.replace(0, 2, 4);
	REQUIRE(~distance == 4*4);

	distance.set(1, 2);
	REQUIRE(~distance == 4*4 + 2*2);

	distance.set(2, 100);
	REQUIRE(~distance == std::max(4*4 + 2*2, 100));

	distance.set(3, 10);
	REQUIRE(~distance == std::max(4*4 + 2*2, 100 + 10));
}


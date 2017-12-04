// Description: Testing for norms
// DocumentationOf: pastel/math/norm.h

#include "test/test_init.h"

#include "pastel/math/norm.h"

template <typename Norm>
void testBasic(Norm norm)
{
	auto copy = norm;
	REQUIRE(copy == norm);
	REQUIRE((real)copy == (real)norm);
}

TEST_CASE("Norm (Manhattan)")
{
	auto norm = Manhattan_Norm<real>();
	testBasic(norm);

	norm.set(0, 2);
	REQUIRE((real)norm == 2);

	norm.replace(0, 2, 4);
	REQUIRE((real)norm == 4);
}

TEST_CASE("Norm (Euclidean)")
{
	auto norm = Euclidean_Norm<real>();
	testBasic(norm);

	norm.set(0, 2);
	REQUIRE((real)norm == std::sqrt(2*2));

	norm.replace(0, 2, 4);
	REQUIRE((real)norm == std::sqrt(4*4));
}

TEST_CASE("Norm (Maximum)")
{
	auto norm = Maximum_Norm<real>();
	testBasic(norm);

	norm.set(0, 2);
	REQUIRE((real)norm == 2);

	norm.replace(0, 2, 4);
	REQUIRE((real)norm == 4);
}

TEST_CASE("Norm (Product)")
{
	auto norm = Product_Norm<real, Euclidean_Norm<real>, Manhattan_Norm<real>>(2);
	testBasic(norm);

	norm.set(0, 2);
	REQUIRE((real)norm == std::sqrt(2*2));

	norm.replace(0, 2, 4);
	REQUIRE((real)norm == std::sqrt(4*4));

	norm.set(1, 2);
	REQUIRE((real)norm == std::sqrt(4*4 + 2*2));

	norm.set(2, 100);
	REQUIRE((real)norm == std::max(std::sqrt(4*4 + 2*2), (real)100));
}


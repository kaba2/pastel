// Description: Testing for locators
// DocumentationOf: locator.h

#include "test/test_init.h"

#include <pastel/sys/locator.h>
#include <pastel/sys/function/dereference_function.h>

TEST_CASE("Archetype (Locator)")
{
	struct A {};
	using Locator = Locator_Archetype<real, A, 3>;
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept());
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept(A));
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept(Locator_Point<Locator>, real));
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept(A, real));
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept(A, Locator_Real<Locator>));
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept(Locator_Point<Locator>, real));

	PASTEL_STATIC_ASSERT(
		Locator_N<Locator>::value == 3);
	PASTEL_STATIC_ASSERT(
		std::is_same<Locator_Real<Locator>, real>::value);
	PASTEL_STATIC_ASSERT(
		std::is_same<Locator_Point<Locator>, A>::value);
}

TEST_CASE("Various (Locator)")
{
	enum : integer
	{
		D = 3
	};

	using Locator = Pointer_Locator<real, D>;
	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

	Locator locator(D);
	real dataSet[] = {0, 1, 2, 3, 4, 5, 6};
	integer n = sizeof(dataSet) / sizeof(real);

	for (integer i = 0;i < n - D;++i)
	{
		for (integer j = 0;j < D;++j)
		{
			REQUIRE(locator(dataSet + i, j) == i + j);
		}
	}
}

TEST_CASE("Scalar (Locator)")
{
	using DataSet = std::vector<real>;
	using Data_ConstIterator = 
		DataSet::const_iterator;

	DataSet dataSet = {0, 7, 3, 5, 11, 13};

	auto locator = scalarLocator<real, 1>();

	for (integer i = 0;i < dataSet.size();++i)
	{
		REQUIRE(locator(dataSet[i], 0) == dataSet[i]);
	}
}

TEST_CASE("Transform (Locator)")
{
	using DataSet = std::vector<real>;
	using Data_ConstIterator = 
		DataSet::const_iterator;

	DataSet dataSet = {0, 1, 2, 3, 4, 5};

	auto indirectLocator = 
		transformLocator<Data_ConstIterator>(
			scalarLocator<real>(1),
			Dereference_Function()
		);

	for (integer i = 0;i < dataSet.size();++i)
	{
		REQUIRE(indirectLocator(dataSet.begin() + i, 0) == i);
	}
}

TEST_CASE("Array (Locator)")
{
	{
		using Locator = Array_Locator<real, 1>;
		PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
	}
	{
		using Point = std::array<real, 2>;
		using Locator = Array_Locator<real, 2>;
		PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

		PASTEL_STATIC_ASSERT(
			(std::is_same<Locator_Real<Locator>, real>::value));

		PASTEL_STATIC_ASSERT(
			(std::is_same<Locator_Point<Locator>, Point>::value));

		PASTEL_STATIC_ASSERT(
			Locator_N<Locator>::value == 2);
	}
}

TEST_CASE("Sub (Locator)")
{
	{
		using Point = std::array<real, 2>;
		using Locator = Sub_Locator<Array_Locator<real, 2>>;
		PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

		PASTEL_STATIC_ASSERT(
			(std::is_same<Locator_Real<Locator>, real>::value));

		PASTEL_STATIC_ASSERT(
			(std::is_same<Locator_Point<Locator>, Point>::value));

		PASTEL_STATIC_ASSERT(
			Locator_N<Locator>::value == Dynamic);
	}
	{
		auto locator = subLocator(arrayLocator<real, 2>(), 0, 2);
		std::array<real, 2> a = {1, 2};
		REQUIRE(locator.n() == 2);
		REQUIRE(locator(a, 0) == 1);
		REQUIRE(locator(a, 1) == 2);
	}
	{
		auto locator = subLocator(arrayLocator<real, 2>(), 0, 0);
		REQUIRE(locator.n() == 0);
	}
	{
		auto locator = subLocator(arrayLocator<real, 2>(), 1, 2);
		std::array<real, 2> a = {1, 2};
		REQUIRE(locator.n() == 1);
		REQUIRE(locator(a, 0) == 2);
	}
	{
		auto locator = subLocator(arrayLocator<real, 2>(), 0, 1);
		std::array<real, 2> a = {1, 2};
		REQUIRE(locator.n() == 1);
		REQUIRE(locator(a, 0) == 1);
	}
}

TEST_CASE("Types (Locator)")
{
	PASTEL_STATIC_ASSERT(
		(std::is_same<Locator_Real<Pointer_Locator<real>>, real>::value));

	PASTEL_STATIC_ASSERT(
		(std::is_same<Locator_Real<Pointer_Locator<real>, Pointer_Locator<integer>>, real>::value));

	PASTEL_STATIC_ASSERT(
		(std::is_same<Locator_Real<Pointer_Locator<integer>, Pointer_Locator<real>>, real>::value));

	PASTEL_STATIC_ASSERT(
		(std::is_same<Locator_Point<Pointer_Locator<integer>>, const integer*>::value));

	PASTEL_STATIC_ASSERT(
		Locator_N<Pointer_Locator<real, 0>>::value == 0);
	PASTEL_STATIC_ASSERT(
		Locator_N<Pointer_Locator<real, 1>>::value == 1);
	PASTEL_STATIC_ASSERT(
		Locator_N<Pointer_Locator<real, 2>>::value == 2);
}


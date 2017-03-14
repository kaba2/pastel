// Description: Testing for point-sets
// DocumentationOf: pointset_concept.h

#include "test/test_init.h"

#include <pastel/sys/pointset/pointset_concept.h>
#include <pastel/sys/set.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/vector.h>

TEST_CASE("Homogeneous (pointset_concept)")
{
	using Point = Vector<real, 2>;

	std::vector<Point> inputSet;
	auto pointSet = inputSet;
	using PointSet = decltype(pointSet);

	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	REQUIRE(pointSetDimension(pointSet) == 2);
	PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 2);
}

TEST_CASE("Real (pointset_concept)")
{
	struct A {};

	using Real = real;
	using PointId = A;
	using Locator = Locator_Archetype<Real, A, 1>;

	std::vector<PointId> inputSet =
	{
		A(), A(), A()
	};

	auto pointIdSet = addConst(inputSet);
	auto pointSet = locationSet(pointIdSet, Locator());

	using PointIdSet = decltype(pointIdSet);

	using PointSet = decltype(pointSet);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	{
		using Set_ = PointSet_Set<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Set_, PointIdSet>::value));

		using PointId_ = PointSet_PointId<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<PointId_, PointId>::value));

		using Point_ = PointSet_Point<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Point_, Location<PointId, Locator>>::value));

		using Real_ = PointSet_Real<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Real_, Real>::value));

		using Locator_ = PointSet_Locator<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Locator_, Locator>::value));

		REQUIRE(pointSetDimension(pointSet) == 1);
		PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 1);

		REQUIRE(pointSetN(pointSet) == 3);
	}
}

TEST_CASE("Array (pointset_concept)")
{
	using Real = real;
	using Point = std::array<Real, 2>;
	using Locator = Array_Locator<Real, 2>;

	std::vector<Point> pointSet =
	{
		{{1, 2}},
		{{3, 4}},
		{{5, 6}}
	};

	using PointSet = decltype(pointSet);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	{
		using Set_ = PointSet_Set<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Set_, PointSet>::value));

		using PointId_ = PointSet_PointId<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<PointId_, Point>::value));

		using Point_ = PointSet_Point<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Point_, Point>::value));

		using Real_ = PointSet_Real<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Real_, Real>::value));

		using Locator_ = PointSet_Locator<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Locator_, Locator>::value));

		REQUIRE(pointSetDimension(pointSet) == 2);
		PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 2);

		REQUIRE(pointSetN(pointSet) == 3);
	}
}

TEST_CASE("Functions (pointset_concept)")
{
	using Real = real;
	using Point = std::array<Real, 2>;
	using Locator = Array_Locator<Real, 2>;

	std::vector<Point> pointSet_;

	{
		auto pointSet = pointSet_;
		REQUIRE(ranges::size(pointSet) == 0);
	}

	Point a = {{1, 2}};
	pointSet_.emplace_back(a);

	{
		auto pointSet = pointSet_;
		REQUIRE(ranges::size(pointSet) == 1);
		REQUIRE(pointSetDimension(pointSet) == 2);

		auto index = pointSet.begin();

		REQUIRE(pointAxis(*index, 0) == 1);
		REQUIRE(pointAxis(*index, 1) == 2);

		++index;
		REQUIRE(index == pointSet.end());
	}
}

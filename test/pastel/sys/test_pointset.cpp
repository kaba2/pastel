// Description: Testing for point-sets
// DocumentationOf: pointset_concept.h

#include "test/test_init.h"

#include <pastel/sys/pointset/pointset_concept.h>
#include <pastel/sys/set.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/vector.h>

TEST_CASE("Homogeneous (pointset_concept)")
{
	using Point = Vector<dreal, 2>;
	PASTEL_CONCEPT_CHECK(Point, Point_Concept);

	using PointSet = std::vector<Point>;
	PointSet pointSet;
	pointSet.assign({{0, 2}, {2, 3}});

	REQUIRE(dimension(pointSet[0]) == 2);
	REQUIRE(dimension(pointSet[1]) == 2);

	PASTEL_CONCEPT_CHECK(PointSet, Range_Concept);
	PASTEL_STATIC_ASSERT(!HasMemberPointSetLocator<PointSet>::value);
	PASTEL_STATIC_ASSERT(BoolConstant<Point_N<Range_Value<PointSet>>::value == 2>::value);

	pointSetRange(pointSet);
	auto locator = pointSetLocator(pointSet);

	using Locator = decltype(locator);

	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
	
	PASTEL_CONCEPT_REJECT(PointSet, Point_Concept);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	REQUIRE(pointSetDimension(pointSet) == 2);
	PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 2);
}

TEST_CASE("Heterogeneous (pointset_concept)")
{
	using Point = std::vector<dreal>;
	PASTEL_CONCEPT_CHECK(Point, Point_Concept);

	using PointSet = std::vector<Point>;
	PointSet pointSet;
	pointSet.assign({{0, 2}, {2, 3, 4}});

	PASTEL_STATIC_ASSERT(Point_HasMemberSize<Point>::value);
	PASTEL_STATIC_ASSERT(!Point_HasMemberDimension<Point>::value);

	REQUIRE(pointSet[0].size() == 2);
	REQUIRE(dimension(pointSet[0]) == 2);
	REQUIRE(dimension(pointSet[1]) == 3);

	PASTEL_CONCEPT_CHECK(PointSet, Range_Concept);
	PASTEL_STATIC_ASSERT(!HasMemberPointSetLocator<PointSet>::value);
	PASTEL_STATIC_ASSERT(BoolConstant<Point_N<Range_Value<PointSet>>::value == Dynamic>::value);

	pointSetRange(pointSet);
	auto locator = pointSetLocator(pointSet);

	using Locator = decltype(locator);

	PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
	
	PASTEL_CONCEPT_REJECT(PointSet, Point_Concept);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	REQUIRE(pointSetDimension(pointSet) == Dynamic);
	PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == Dynamic);
}

TEST_CASE("Real (pointset_concept)")
{
	struct A {};

	using Real = dreal;
	using PointId = A;
	using Locator = Locator_Archetype<Real, A, 1>;

	std::vector<PointId> inputSet =
	{
		A(), A(), A()
	};

	auto pointIdSet = addConst(inputSet);
	auto pointSet = locationSet(pointIdSet, Locator());

	using PointIdSet = RemoveCvRef<decltype(pointIdSet)>;
	using PointSet = RemoveCvRef<decltype(pointSet)>;

	PASTEL_CONCEPT_CHECK(PointSet, Range_Concept);
	PASTEL_STATIC_ASSERT(HasMemberPointSet<PointSet>::value);

	PASTEL_CONCEPT_CHECK(decltype(pointSetRange(pointSet)), Range_Concept);

	PASTEL_STATIC_ASSERT(HasMemberPointSetLocator<PointSet>::value);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	{
		using Set_ = PointSet_Range<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Set_, PointIdSet>::value));

		using Point = PointSet_Point<PointSet>;
		using PointId_ = PointSet_PointId<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<PointId_, PointId>::value));

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
	using Real = dreal;
	using Point = std::array<Real, 2>;

	std::vector<Point> pointSet =
	{
		{{1, 2}},
		{{3, 4}},
		{{5, 6}}
	};

	REQUIRE(dimension(pointSet[0]) == 2);
	REQUIRE(dimension(pointSet[1]) == 2);
	REQUIRE(dimension(pointSet[2]) == 2);

	using PointSet = decltype(pointSet);
	PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

	{
		using Set_ = PointSet_Range<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Set_, PointSet>::value));

		using PointId_ = PointSet_PointId<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<PointId_, Point>::value));

		using Point_B = PointSet_Point<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Point_B, Point>::value));

		using Real_ = PointSet_Real<PointSet>;
		PASTEL_STATIC_ASSERT((std::is_same<Real_, Real>::value));

		REQUIRE(pointSetDimension(pointSet) == 2);
		PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 2);

		REQUIRE(pointSetN(pointSet) == 3);
	}
}

TEST_CASE("Functions (pointset_concept)")
{
	using Real = dreal;
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

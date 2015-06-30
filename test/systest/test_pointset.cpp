// Description: Testing for point-sets
// DocumentationOf: pointset_concept.h

#include "test_pastelsys.h"

#include <pastel/sys/pointset/pointset_concept.h>
#include <pastel/sys/input.h>
#include <pastel/sys/set.h>
#include <pastel/sys/locator.h>
#include <pastel/sys/vector.h>

namespace
{

	using namespace Pastel;

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testHomogeneous();
			testArray();
			testFunctions();
		}

		void testHomogeneous()
		{
			using Point = Vector<real, 2>;
			
			std::vector<Point> inputSet;
			auto pointSet = rangeSet(inputSet);
			using PointSet = decltype(pointSet);

			PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
			
			TEST_ENSURE_OP(pointSetDimension(pointSet), == , 2);
			PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 2);
		}

		void testArray()
		{
			using Real = real;
			using Point = std::array<Real, 2>;
			using Locator = Array_Locator<Real, 2>;

			std::vector<Point> inputSet;
			auto pointSet = rangeSet(inputSet);
			
			using PointSet = decltype(pointSet);
			PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

			{
				using Set_ = PointSet_Set<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Set_, PointSet>::value));

				using Point_ = PointSet_Point<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Point_, Point>::value));

				using Real_ = PointSet_Real<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Real_, Real>::value));

				using Locator_ = PointSet_Locator<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Locator_, Locator>::value));

				using Location_ = PointSet_Location<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Location_, Location<Point, Locator>>::value));

				TEST_ENSURE_OP(pointSetDimension(pointSet), ==, 2);
				PASTEL_STATIC_ASSERT(PointSet_Dimension<PointSet>::value == 2);
			}
		}

		void testFunctions()
		{
			using Real = real;
			using Point = std::array<Real, 2>;
			using Locator = Array_Locator<Real, 2>;

			std::vector<Point> pointSet_;

			{
				auto pointSet = rangeSet(pointSet_);
				TEST_ENSURE_OP(pointSet.n(), ==, 0);
			}

			Point a = {{1, 2}};
			pointSet_.emplace_back(a);

			{
				auto pointSet = rangeSet(pointSet_);
				TEST_ENSURE_OP(pointSet.n(), ==, 1);
				TEST_ENSURE_OP(pointSetDimension(pointSet), ==, 2);

				auto state = pointSet.state();

				TEST_ENSURE(pointAxis(pointSet.element(state), 0) == 1);
				TEST_ENSURE(pointAxis(pointSet.element(state), 1) == 2);

				pointSet.next(state);
				TEST_ENSURE(pointSet.empty(state));
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("pointset_concept", test);
	}

	CallFunction run(addTest);

}

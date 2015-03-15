// Description: Testing for point-sets
// DocumentationOf: pointset_concept.h

#include "test_pastelsys.h"

#include <pastel/sys/pointset/pointset_concept.h>
#include <pastel/sys/input.h>
#include <pastel/sys/locator.h>

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
			auto pointSet = rangeInput(range(inputSet.begin(), inputSet.end()));

			PASTEL_STATIC_ASSERT((Models<decltype(pointSet), PointSet_Concept>::value));
			
			//TEST_ENSURE_OP(pointSetDimension(pointSet), == , 0);
		}

		void testArray()
		{
			using Real = real;
			using Point = std::array<Real, 2>;
			using Locator = Array_Locator<Real, 2>;

			std::vector<Point> inputSet;
			auto pointSet = rangeInput(range(inputSet.begin(), inputSet.end()));
			PASTEL_STATIC_ASSERT((Models<decltype(pointSet), PointSet_Concept>::value));
			{
				using PointSet = decltype(pointSet);

				using Input_ = PointSet_Input<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Input_, PointSet>::value));

				using Point_ = PointSet_Point<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Point_, Point>::value));

				using Real_ = PointSet_Real<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Real_, Real>::value));

				using Locator_ = PointSet_Locator<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Locator_, Locator>::value));

				using Location_ = PointSet_Location<PointSet>;
				PASTEL_STATIC_ASSERT((std::is_same<Location_, Location<Point, Locator>>::value));

				TEST_ENSURE_OP(pointSetDimension(pointSet), ==, 2);
			}
		}

		void testFunctions()
		{
			using Real = real;
			using Point = std::array<Real, 2>;
			using Locator = Array_Locator<Real, 2>;

			std::vector<Point> pointSet;

			{
				auto pointInput = rangeInput(pointSet);
				TEST_ENSURE(pointSetEmpty(pointInput));
			}

			Point a = {{1, 2}};
			pointSet.emplace_back(a);

			{
				auto pointInput = rangeInput(pointSet);
				TEST_ENSURE(!pointSetEmpty(pointInput));
				TEST_ENSURE_OP(pointSetDimension(pointInput), ==, 2);

				TEST_ENSURE(axis(pointSetGet(pointInput), 0) == 1);
				TEST_ENSURE(axis(pointSetGet(pointInput), 1) == 2);

				pointSetPop(pointInput);
				TEST_ENSURE(pointSetEmpty(pointInput));
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

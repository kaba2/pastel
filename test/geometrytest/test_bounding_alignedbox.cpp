// Description: Testing for bounding aligned box
// DocumentationOf: bounding_alignedbox.h

#include "test_pastelgeometry.h"

#include <pastel/sys/locators.h>
#include <pastel/sys/vector.h>
#include <pastel/sys/input.h>

#include <pastel/geometry/bounding/bounding_alignedbox.h>

using namespace Pastel;

namespace
{

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
			test();
		}

		void test()
		{
			using Point = Vector2i;
			using Locator = Vector_Locator<integer, 2>;

			{
				std::vector<Point> pointSet;

				AlignedBox2i bound =
					boundingAlignedBox(
					rangeInput(pointSet),
					Locator());

				TEST_ENSURE(bound.min() == Vector2i(infinity<integer>()));
				TEST_ENSURE(bound.max() == Vector2i(-infinity<integer>()));
			}
			{
				std::vector<Point> pointSet =
				{
					{3, 5}
				};

				AlignedBox2i bound =
					boundingAlignedBox(
					rangeInput(pointSet),
					Locator());

				TEST_ENSURE(bound.min() == Vector2i(3, 5));
				TEST_ENSURE(bound.max() == Vector2i(3, 5));
			}
			{
				// 5  
				// 4  *
				// 3   * *
				// 2  *
				// 1    *
				// 0*
				//  0123456

				std::vector<Point> pointSet =
				{
					{0, 0},
					{2, 2},
					{2, 4},
					{3, 3},
					{4, 1},
					{5, 3}
				};

				AlignedBox2i bound =
					boundingAlignedBox(
						rangeInput(pointSet),
						Locator());

				TEST_ENSURE(bound.min() == Vector2i(0, 0));
				TEST_ENSURE(bound.max() == Vector2i(5, 4));
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
		testRunner().add("bounding_alignedbox", test);
	}

	CallFunction run(addTest);

}

// Description: Testing for grid traversal
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

#include <pastel/sys/for_each_point.h>

using namespace Pastel;
using namespace std;

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
			using Box = AlignedBox2i;

			std::vector<Point> pointSet;

			auto report = [&](const Point& point)
			{
				pointSet.emplace_back(point);
			};

			{
				forEachPoint(
					Box(Point(3, 3), Point(3, 3)),
					report);

				TEST_ENSURE(pointSet.empty());
			}

			{
				forEachPoint(
					Box(Point(3, 3), Point(0, 2)),
					report);

				TEST_ENSURE(pointSet.empty());
			}

			{
				std::vector<Point> correctSet =
				{
					{1, 2},
					{2, 2},
					{1, 3},
					{2, 3},
					{1, 4},
					{2, 4},
				};

				pointSet.clear();
				forEachPoint(
					Box(Point(1, 2), Point(3, 5)),
					report);

				TEST_ENSURE(boost::equal(pointSet, correctSet));
			}

			{
				std::vector<Point> correctSet =
				{
					{1, 2},
					{1, 3},
					{1, 4},
					{2, 2},
					{2, 3},
					{2, 4},
				};

				pointSet.clear();
				forEachPoint<false>(
					Box(Point(1, 2), Point(3, 5)),
					report);

				TEST_ENSURE(boost::equal(pointSet, correctSet));
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
		testRunner().add("for_each_point", test);
	}

	CallFunction run(addTest);

}

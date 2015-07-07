// Description: Testing for grid traversal
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include <pastel/sys/for_each_point.h>

namespace
{

	class Test
	{
	public:
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

				REQUIRE(pointSet.empty());
			}

			{
				forEachPoint(
					Box(Point(3, 3), Point(0, 2)),
					report);

				REQUIRE(pointSet.empty());
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

				REQUIRE(boost::equal(pointSet, correctSet));
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

				REQUIRE(boost::equal(pointSet, correctSet));
			}
		}
	};

	TEST_CASE("for_each_point", "[for_each_point]")
	{
	}

}

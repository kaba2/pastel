// Description: Testing for range tree
// DocumentationOf: rangetree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/rangetree.h"

#include "pastel/sys/locators.h"
#include "pastel/sys/vector_locator.h"

#include <boost/range/adaptor/indirected.hpp>

using namespace Pastel;

namespace
{

	class Settings
	{
	public:
		using Real = real;
		using Point = Vector<Real, 2>;
	};

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
			using Point = Vector<real, 2>;
			std::vector<Point> pointSet = 
			{ 
				{ 1, 2 }, { 3, 4 }, { 4, 5 } 
			};

			auto xLess = [](
				const Point& left, 
				const Point& right)
			{
				return left[0] < right[0];
			};

			auto yLess = [](
				const Point& left,
				const Point& right)
			{
				return left[1] < right[1];
			};

			RangeTree<Settings> tree(
				pointSet, xLess, yLess);

			rangeSearch(tree, Point(0, 0), Point(3, 5));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("RangeTree", test);
	}

	CallFunction run(addTest);

}

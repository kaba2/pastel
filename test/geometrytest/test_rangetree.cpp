// Description: Testing for range tree
// DocumentationOf: rangetree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/rangetree.h"

#include <iostream>

using namespace Pastel;

namespace
{

	class Settings
	{
	public:
		using Point = Vector<real, 2>;
		class MultiLess
		{
		public:
			bool operator()(
				const Point& left,
				const Point& right,
				integer i)
			{
				return left[i] < right[i];
			}
		};
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

			using Tree = RangeTree<Settings>;
			using Point_ConstIterator = Tree::Point_ConstIterator;

			Tree tree(pointSet, 2);

			auto report = [&](
				const Point_ConstIterator& point)
			{
				std::cout << "(";
				for (integer i = 0; i < point->n(); ++i)
				{
					std::cout << (*point)[i];
					if (i < point->n() - 1)
					{
						std::cout << ", ";
					}
				}
				std::cout << ")" << std::endl;
			};

			rangeSearch(tree, Point(0, 0), Point(3, 5), report);
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

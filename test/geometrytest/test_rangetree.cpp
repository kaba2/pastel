// Description: Testing for range tree
// DocumentationOf: rangetree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/rangetree.h"
#include "pastel/sys/eps.h"
#include "pastel/sys/for_each_point.h"

#include <iostream>
#include <queue>

using namespace Pastel;

namespace
{

	template <integer N>
	using Point = Vector<real, N>;

	class MultiLess
	{
	public:
		template <typename Point>
		bool operator()(
			const Point& left,
			const Point& right,
			integer i)
		{
			return left[i] < right[i];
		}
	};

	template <integer N>
	class Settings
	{
	public:
		using Point = Point<N>;
		using MultiLess = MultiLess;
	};

	using Tree = RangeTree<Settings<2>>;
	using Point_ConstIterator = Tree::Point_ConstIterator;
	using Node_ConstIterator = Tree::Node_ConstIterator;

	std::ostream& operator<<(
		std::ostream& stream,
		const Point_ConstIterator& point)
	{
		stream << "(";
		if (point == Point_ConstIterator())
		{
			stream << "  ";
		}
		else
		{
			stream << *point;
		}
		stream << ")";

		return stream;
	}

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
			testSingular<2>();
			testSingular<3>();
			testSingular<4>();
		}

		void print(const Tree& tree)
		{
			Node_ConstIterator node = tree.root();

			integer prevLevel = -1;
			
			std::list<std::pair<Node_ConstIterator, integer>> nodeSet;
			nodeSet.push_back(std::make_pair(node, 0));
			while (!nodeSet.empty())
			{
				std::pair<Node_ConstIterator, integer> nodeLevel = nodeSet.front();

				Node_ConstIterator node = nodeLevel.first;
				integer level = nodeLevel.second;

				nodeSet.pop_front();

				if (!node->isLeaf())
				{
					nodeSet.push_back(std::make_pair(node->child(false), level + 1));
					nodeSet.push_back(std::make_pair(node->child(true), level + 1));
				}
				
				if (prevLevel != level)
				{
					std::cout << std::endl;
					prevLevel = level;
				}

				if (!node->isLeaf() && node->isBottom())
				{
					integer n = node->entries();
					for (integer i = 0; i < n; ++i)
					{
						const auto& entry = node->entryRange()[i];
						
						Node_ConstIterator left = node->child(false);
						const auto& leftEntry = left->entryRange()[entry.cascade(false)];

						Node_ConstIterator right = node->child(true);
						const auto& rightEntry = right->entryRange()[entry.cascade(true)];

						std::cout
							<< "["
							<< leftEntry.point()
							<< entry.point()
							<< rightEntry.point()
							<< "] ";
					}
				}
			}
		}

		template <integer N>
		void testSingular()
		{
			using Tree = RangeTree<Settings<N>>;
			using Point_ConstIterator = Tree::Point_ConstIterator;
			using Node_ConstIterator = Tree::Node_ConstIterator;

			integer width = 5;

			using Point = typename Tree::Point;
			using Box = AlignedBox<integer, N>;

			Box grid(Point(0), Point(width));
									
			std::vector<Point> pointSet;
			pointSet.reserve(product(grid.extent()));
			forEachPoint(
				grid,
				[&](const Point& point)
			{
				pointSet.emplace_back(point);
			});

			Tree tree(pointSet, N);
			TEST_ENSURE(testInvariants(tree));

			forEachPoint(
				grid,
				[&](const Point& point)
			{
				std::vector<Point> resultSet;

				auto report = [&](
					const Point_ConstIterator& point)
				{
					resultSet.emplace_back(*point);
				};

				rangeSearch(tree, point, point, report);

				std::vector<Point> correctSet;
				correctSet.emplace_back(point);

				TEST_ENSURE(boost::equal(resultSet, correctSet));
				
				Point a = point;
				a[0] = nextGreater(a[0]);

				Point b = point + 1;
				b[0] = nextSmaller(b[0]);

				resultSet.clear();
				rangeSearch(tree, a, b, report);

				TEST_ENSURE(resultSet.empty());
			});
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

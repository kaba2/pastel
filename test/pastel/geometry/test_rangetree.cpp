// Description: Testing for range tree
// DocumentationOf: rangetree.h

#include "test/test_init.h"

#include <pastel/geometry/rangetree/rangetree.h>
#include <pastel/sys/math/eps.h>
#include <pastel/sys/for_each_point.h>
#include <pastel/sys/input.h>

#include <iostream>
#include <queue>
#include <list>

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

	using MultiLess_ = MultiLess;

	template <integer N>
	using Point_ = Point<N>;

	template <integer N>
	class Settings
	{
	public:
		using Point = Point_<N>;
		using MultiLess = MultiLess_;
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
	{
	public:
		virtual void run()
		{
			testSimple();
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

		void testSimple()
		{
			// 4  *
			// 3* *  *
			// 2 *      *
			// 1  * * 
			// 0      *
			//  012345678

			std::vector<Point<2>> pointSet;

			pointSet.emplace_back(0, 3);
			pointSet.emplace_back(1, 2);
			pointSet.emplace_back(2, 1);
			pointSet.emplace_back(2, 3);
			pointSet.emplace_back(2, 4);
			pointSet.emplace_back(4, 1);
			pointSet.emplace_back(5, 3);
			pointSet.emplace_back(6, 0);
			pointSet.emplace_back(8, 2);

			Tree tree(rangeInput(pointSet), 2);
			REQUIRE(testInvariants(tree));

			std::vector<Point<2>> resultSet;

			auto report = [&](
				const Point_ConstIterator& point)
			{
				resultSet.emplace_back(*point);
			};

			auto order = [&](
				const Point<2>& left,
				const Point<2>& right)
			{
				if (left[0] == right[0])
				{
					return left[1] < right[1];
				}
				return left[0] < right[0];
			};

			{
				auto test = [&](
					const Point<2>& min,
					const Point<2>& max,
					integer correct)
				{
					return searchRange(tree, min, max) == correct;
				};

				REQUIRE(test(Point<2>(0, 0), Point<2>(0, 4), 1));
				REQUIRE(test(Point<2>(0, 0), Point<2>(1, 4), 2));
				REQUIRE(test(Point<2>(0, 0), Point<2>(2, 4), 5));
				REQUIRE(test(Point<2>(0, 0), Point<2>(3, 4), 5));
				REQUIRE(test(Point<2>(0, 0), Point<2>(4, 4), 6));
				REQUIRE(test(Point<2>(0, 0), Point<2>(5, 4), 7));
				REQUIRE(test(Point<2>(0, 0), Point<2>(6, 4), 8));
				REQUIRE(test(Point<2>(0, 0), Point<2>(7, 4), 8));
				REQUIRE(test(Point<2>(0, 0), Point<2>(8, 4), 9));

				REQUIRE(test(Point<2>(0, 0), Point<2>(8, 0), 1));
				REQUIRE(test(Point<2>(0, 0), Point<2>(8, 1), 3));
				REQUIRE(test(Point<2>(0, 0), Point<2>(8, 2), 5));
				REQUIRE(test(Point<2>(0, 0), Point<2>(8, 3), 8));
				REQUIRE(test(Point<2>(0, 0), Point<2>(8, 4), 9));
			}
			{
				integer count = 
					searchRange(tree, Point<2>(2, 1), Point<2>(4, 3), report);
				boost::sort(resultSet, order);
				REQUIRE(count == resultSet.size());

				std::vector<Point<2>> correctSet;
				correctSet.emplace_back(2, 1);
				correctSet.emplace_back(2, 3);
				correctSet.emplace_back(4, 1);

				REQUIRE(boost::equal(resultSet, correctSet));
			}

			{
				resultSet.clear();

				integer count = 
					searchRange(tree, Point<2>(0), Point<2>(8, 4), report);
				REQUIRE(count == resultSet.size());

				boost::sort(resultSet, order);
				REQUIRE(boost::equal(resultSet, pointSet));
			}
		}

		template <integer N>
		void testSingular()
		{
			using Tree = RangeTree<Settings<N>>;
			using Point_ConstIterator = typename Tree::Point_ConstIterator;
			using Node_ConstIterator = typename Tree::Node_ConstIterator;

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

			Tree tree(rangeInput(pointSet), N);
			REQUIRE(testInvariants(tree));

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

				integer count = searchRange(tree, point, point, report);
				REQUIRE(count == resultSet.size());

				std::vector<Point> correctSet;
				correctSet.emplace_back(point);

				REQUIRE(boost::equal(resultSet, correctSet));

				Point a = point;
				a[0] = nextGreater(a[0]);

				Point b = point + 1;
				b[0] = nextSmaller(b[0]);

				resultSet.clear();
				count = searchRange(tree, a, b, report);
				REQUIRE(count == resultSet.size());

				REQUIRE(resultSet.empty());
			});
		}
	};

	TEST_CASE("RangeTree", "[RangeTree]")
	{
	}

}

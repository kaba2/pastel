// Description: Testing for range tree
// DocumentationOf: rangetree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/rangetree.h"

#include <iostream>
#include <queue>

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

	using Tree = RangeTree<Settings>;
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
			test();
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

		void test()
		{
			using Point = Vector<real, 2>;
			std::vector<Point> pointSet = 
			{ 
				{ 1, 2 }, { 3, 4 }, { 4, 5 } 
			};

			Tree tree(pointSet, 2);
			TEST_ENSURE(testInvariants(tree));

			print(tree);

			auto report = [&](
				const Point_ConstIterator& point)
			{
				std::cout << *point << std::endl;
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

// Description: Testing for temporal kd-tree
// DocumentationOf: tdtree.h

#include "test/test_init.h"

#include "pastel/geometry/tdtree/tdtree.h"

#include "pastel/geometry/search_nearest_kdtree.h"
#include "pastel/geometry/distance/distance_point_point.h"
#include "pastel/geometry/search_nearest_bruteforce.h"

#include "pastel/sys/locator.h"
#include "pastel/sys/set.h"
#include "pastel/sys/output.h"
#include "pastel/sys/for_each_point.h"
#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/iterator/counting_iterator.h"
#include "pastel/sys/iterator/constant_iterator.h"

namespace
{

	class Test
	{
	public:
		using Point = Vector2;
		using Locator = Vector_Locator<real, 2>;
		using Tree = TdTree<TdTree_Settings<Locator>>;
		using ConstIterator = Tree::ConstIterator;

		PASTEL_CONCEPT_CHECK(Tree, NearestSet_Concept);

		virtual void run()
		{
			testConstruction();
			testGrid();
			testLinear();
			testGaussian();
		}

		void testConstruction()
		{
			Tree tree;
			REQUIRE(tree.simple());

			tree.clear();
			REQUIRE(tree.simple());

			tree.swap(Tree());
		}

		void testGrid()
		{
			Array<Point> pointSet(Vector2i(5, 5));

			forEachPoint(
				AlignedBox2i(Point(0), Point(5)),
				[&](const Point& point)
			{
				pointSet(point[0], point[1]) = point;
			});

			Tree tree(rangeSet(pointSet));
			REQUIRE(tree.simple());

			REQUIRE(tree.bound() == 
				AlignedBox2(Vector2(0, 0), Vector2(4, 4)));

			REQUIRE(tree.timeToIndex(-infinity<real>()) == 0);
			REQUIRE(tree.timeToIndex(-2) == 0);
			REQUIRE(tree.timeToIndex(-1) == 0);
			REQUIRE(tree.timeToIndex(-0.5) == 0);
			REQUIRE(tree.timeToIndex(0) == 0);
			REQUIRE(tree.timeToIndex(0.5) == 1);
			REQUIRE(tree.timeToIndex(1) == 1);
			REQUIRE(tree.timeToIndex(24) == 24);
			REQUIRE(tree.timeToIndex(24.5) == 25);
			REQUIRE(tree.timeToIndex(25.5) == 25);
			REQUIRE(tree.timeToIndex(26) == 25);
			REQUIRE(tree.timeToIndex(infinity<real>()) == 25);

			std::unordered_set<ConstIterator, IteratorAddress_Hash> neighborSet;

			auto report = [&](
				real distance,
				const ConstIterator& point)
			{
				neighborSet.insert(point);
			};

			searchNearest(
				tree, 
				Point(1, 3), 
				PASTEL_TAG(report), report,
				PASTEL_TAG(kNearest), 5);

			auto pointSet_ = tree.pointSet();

			REQUIRE(neighborSet.size() == 5);
			//REQUIRE(neighborSet.count(pointSet(1, 3)) > 0);
		}

		void testGaussian()
		{
			using Point = Vector3;
			using Locator = Vector_Locator<real, 3>;
			using Tree = TdTree<TdTree_Settings<Locator>>;
			using ConstIterator = Tree::ConstIterator;

			using PointSet = std::vector<Point>;

			integer n = 1000;

			PointSet pointSet;
			pointSet.reserve(n);

			using Point_Iterator = PointSet::iterator;

			for (integer i = 0; i < n; ++i)
			{
				pointSet.emplace_back(
					randomGaussianVector<real, 3>());
			}

			Tree tree(rangeSet(pointSet));

			integer k = 7;

			for (integer i = 0; i < n; ++i)
			{
				std::vector<std::pair<real, Point>> bruteSet;
				bruteSet.reserve(k);

				real kDistanceBrute = searchNearest(
					bruteForceNearestSet(rangeSet(pointSet)),
					pointSet[i],
					PASTEL_TAG(report), emplaceBackOutput(bruteSet),
					PASTEL_TAG(kNearest), k
					).first;

				std::vector<std::pair<real, ConstIterator>> treeSet;
				treeSet.reserve(k);

				real kDistanceTree = searchNearest(
						tree,
						pointSet[i],
						PASTEL_TAG(report), emplaceBackOutput(treeSet),
						PASTEL_TAG(kNearest), k
					).first;

				REQUIRE(kDistanceBrute == kDistanceTree);

				for (integer j = 0; j < k; ++j)
				{
					REQUIRE(bruteSet[j].first == treeSet[j].first);
					if (bruteSet[j].first != treeSet[j].first)
					{
						std::cout << bruteSet[j].first << " != " << treeSet[j].first << std::endl;
					}
				}
			}
		}

		void testLinear()
		{
			std::vector<Point> pointSet;
			integer n = 1000;
			for (integer i = 0; i < n; ++i)
			{
				pointSet.emplace_back(i, 0);
			}

			Tree tree(rangeSet(pointSet));
			REQUIRE(tree.simple());

			for (integer i = 0; i < n; ++i)
			{
				{
					Vector2 timeInterval = { (real)i, (real)n };
					integer distance =
						searchNearest(
							tree, 
							Point(0, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					REQUIRE(distance == square(i));
				}
				{
					Vector2 timeInterval = { (real)0, (real)i + 1 };
					integer distance =
						searchNearest(
							tree, 
							Point(0, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					REQUIRE(distance == 0);
				}
				{
					Vector2 timeInterval = { (real)i, (real)i + 1 };
					integer distance =
						searchNearest(
							tree, 
							Point(0, 0),
							PASTEL_TAG(intervalSequence), timeInterval).first;
					REQUIRE(distance == square(i));
				}
			}

			for (integer i = 0; i < n; ++i)
			{
				Vector4 timeInterval = { (real)i, (real)i + 1, (real)i + 5, (real)i + 6 };
				{
					real distance =
						searchNearest(
							tree, 
							Point(i + 2, 0),
							PASTEL_TAG(intervalSequence), timeInterval).first;

					REQUIRE((integer)distance == square(2));
				}

				{
					real distance =
						searchNearest(
							tree, 
							Point(i - 3, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;

					REQUIRE((integer)distance == square(3));
				}

				{
					real distance =
						searchNearest(
							tree, 
							Point(i + 4, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					integer correct = i < (n - 5) ? square(1) : square(4);

					REQUIRE((integer)distance == correct);
				}

				{
					real distance =
						searchNearest(
							tree, 
							Point(i + 7, 0), 
							PASTEL_TAG(intervalSequence), timeInterval).first;
					integer correct = i < (n - 5) ? square(2) : square(7);

					REQUIRE((integer)distance == correct);
				}
			}
		}
	};

	TEST_CASE("TdTree", "[TdTree]")
	{
	}

}
// Description: Testing for PointKdTree
// DocumentationOf: pointkdtree.h

#include "pastelgeometrytest.h"

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/slidingmidpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/bestfirst_searchalgorithm_pointkdtree.h"

#include "pastel/device/timer.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/sys/iterators.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		typedef PointKdTree<real, 2> Tree;
		typedef Tree::Point_ConstIterator Point_ConstIterator;

		Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			//testTiming<2>();
			testEmpty();
			test();
			testCircular<2>();
			testCircular<3>();
			testSearch(
				DepthFirst_SearchAlgorithm_PointKdTree());
			testSearch(
				BestFirst_SearchAlgorithm_PointKdTree());
		}

		template <typename SearchAlgorithm_PointKdTree>
		void testSearch(
			SearchAlgorithm_PointKdTree searchAlgorithm)
		{
			Euclidean_NormBijection<real> normBijection;

			/*
			 0   |
			     |2  3
			  1  | 4
				 |5
			--6--+--7---
			     |  8
			9ABC D
			     |    E
				 |
			*/

			std::vector<Vector2> pointSet;
			// 0
			pointSet.push_back(
				Vector2(-4, 4));
			// 1
			pointSet.push_back(
				Vector2(-3, 2));
			// 2
			pointSet.push_back(
				Vector2(1, 3));
			// 3
			pointSet.push_back(
				Vector2(4, 3));
			// 4
			pointSet.push_back(
				Vector2(2, 2));
			// 5
			pointSet.push_back(
				Vector2(1, 1));
			// 6
			pointSet.push_back(
				Vector2(-3, 0));
			// 7
			pointSet.push_back(
				Vector2(3, 0));
			// 8
			pointSet.push_back(
				Vector2(3, -1));
			// 9
			pointSet.push_back(
				Vector2(-5, -2));
			// A
			pointSet.push_back(
				Vector2(-4, -2));
			// B
			pointSet.push_back(
				Vector2(-3, -2));
			// C
			pointSet.push_back(
				Vector2(-2, -2));
			// D
			pointSet.push_back(
				Vector2(0, -2));
			// E
			pointSet.push_back(
				Vector2(5, -3));

			std::vector<Point_ConstIterator> iteratorSet;

			Tree tree;
			tree.insert(
				range(pointSet.begin(), pointSet.end()),
				std::back_inserter(iteratorSet));
			TEST_ENSURE(check(tree));

			tree.refine(SlidingMidpoint_SplitRule_PointKdTree(), 1);
			TEST_ENSURE(check(tree));
			
			/*
			 0   |
			     |2  3
			  1  | 4
				 |5
			--6--+--7---
			     |  8
			9ABC D
			     |    E
				 |
			*/
			
			/*
			Nearest neighbors:
			0 -> 1 (1^2 + 2^2 = 5)
			1 -> 6 (0^2 + 2^2 = 4)
			2 -> 4 (1^2 + 1^2 = 2)
			3 -> 4 (2^2 + 1^2 = 5)
			4 -> 2 or 5 (1^2 + 1^2 = 2)
			5 -> 4 (1^2 + 1^2 = 2)
			6 -> 1 or B (0^2 + 2^2 = 4)
			7 -> 8 (0^2 + 1^2 = 1)
			8 -> 7 (0^2 + 1^2 = 1)
			9 -> A (1^2 + 0^2 = 1)
			A -> 9 or B (1^2 + 0^2 = 1)
			B -> A or C (1^2 + 0^2 = 1)
			C -> B (1^2 + 0^2 = 1)
			D -> C (2^2 + 0^2 = 4)
			E -> 8 (2^2 + 2^2 = 8)
			*/

			std::vector<Point_ConstIterator> correctSet;
			correctSet.push_back(iteratorSet[1]);
			correctSet.push_back(iteratorSet[6]);
			correctSet.push_back(iteratorSet[4]);
			correctSet.push_back(iteratorSet[4]);
			correctSet.push_back(iteratorSet[2]);
			correctSet.push_back(iteratorSet[4]);
			correctSet.push_back(iteratorSet[1]);
			correctSet.push_back(iteratorSet[8]);
			correctSet.push_back(iteratorSet[7]);
			correctSet.push_back(iteratorSet[10]);
			correctSet.push_back(iteratorSet[9]);
			correctSet.push_back(iteratorSet[10]);
			correctSet.push_back(iteratorSet[11]);
			correctSet.push_back(iteratorSet[12]);
			correctSet.push_back(iteratorSet[8]);

			std::vector<real> distanceSet;
			distanceSet.push_back(5);
			distanceSet.push_back(4);
			distanceSet.push_back(2);
			distanceSet.push_back(5);
			distanceSet.push_back(2);
			distanceSet.push_back(2);
			distanceSet.push_back(4);
			distanceSet.push_back(1);
			distanceSet.push_back(1);
			distanceSet.push_back(1);
			distanceSet.push_back(1);
			distanceSet.push_back(1);
			distanceSet.push_back(1);
			distanceSet.push_back(4);
			distanceSet.push_back(8);

			for (integer i = 0;i < iteratorSet.size();++i)
			{
				{
					const KeyValue<real, Point_ConstIterator> result = 
						searchNearestOne(tree, iteratorSet[i], infinity<real>(),
						0, alwaysAcceptPoint(tree),
						1, normBijection, searchAlgorithm);

					const real distance2 = result.key();
					const Point_ConstIterator iter = result.value();

					TEST_ENSURE(iter == iteratorSet[i]);
					TEST_ENSURE(distance2 == 0);
				}
				
				{
					const KeyValue<real, Point_ConstIterator> result = 
						searchNearestOne(tree, iteratorSet[i], infinity<real>(),
						0, dontAcceptPoint(iteratorSet[i]),
						1, normBijection, searchAlgorithm);
					
					const real distance2 = result.key();
					const Point_ConstIterator iter = result.value();

					TEST_ENSURE(iter == correctSet[i]);
					TEST_ENSURE(distance2 == distanceSet[i]);
				}
			}
		}

		void test()
		{
			const integer m = 10000;

			std::vector<Vector2> pointSet;
			pointSet.reserve(m);
			for (integer i = 0;i < m;++i)
			{
				pointSet.push_back(2 * randomVectorBall<real, 2>());
			}

			std::vector<Point_ConstIterator> iteratorSet;

			Tree tree;
			tree.insert(
				range(pointSet.begin(), pointSet.end()),
				std::back_inserter(iteratorSet));
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.points(), ==, m);
			TEST_ENSURE_OP(tree.leaves(), ==, 1);
			TEST_ENSURE_OP(tree.nodes(), ==, 1);

			tree.refine(SlidingMidpoint_SplitRule_PointKdTree());
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.points(), ==, m);

			{
				Tree bTree(tree);
				TEST_ENSURE(check(bTree));
				TEST_ENSURE(equivalentKdTree(tree, bTree));

				tree.swap(bTree);
				TEST_ENSURE(check(bTree));
				TEST_ENSURE(equivalentKdTree(tree, bTree));
			}

			tree.merge();
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.leaves(), ==, 1);
			TEST_ENSURE_OP(tree.nodes(), ==, 1);
			TEST_ENSURE_OP(tree.points(), ==, m);

			tree.refine(SlidingMidpoint_SplitRule_PointKdTree());
			TEST_ENSURE(check(tree));

			tree.merge(tree.root());
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.leaves(), ==, 1);
			TEST_ENSURE_OP(tree.nodes(), ==, 1);
			TEST_ENSURE_OP(tree.points(), ==, m);

			tree.refine(SlidingMidpoint_SplitRule_PointKdTree());
			TEST_ENSURE(check(tree));

			tree.hide();
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.points(), ==, 0);

			tree.show();
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.points(), ==, m);

			tree.erase();
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.points(), ==, 0);

			tree.insert(
				range(pointSet.begin(), pointSet.end()),
				std::back_inserter(iteratorSet));
			TEST_ENSURE(check(tree));

			tree.clear();
			TEST_ENSURE(check(tree));
			TEST_ENSURE_OP(tree.points(), ==, 0);
			TEST_ENSURE_OP(tree.leaves(), ==, 1);
			TEST_ENSURE_OP(tree.nodes(), ==, 1);
			{
				Tree emptyTree;
				TEST_ENSURE(equivalentKdTree(tree, emptyTree));
			}
		}

		void testEmpty()
		{
			Tree tree;
			TEST_ENSURE(check(tree));
			TEST_ENSURE(tree.empty());
			TEST_ENSURE_OP(tree.points(), ==, 0);
			TEST_ENSURE(tree.bound().empty());
			TEST_ENSURE_OP(tree.leaves(), ==, 1);
			TEST_ENSURE_OP(tree.nodes(), ==, 1);
						
			tree.clear();
			TEST_ENSURE(check(tree));
			TEST_ENSURE(tree.empty());
			TEST_ENSURE_OP(tree.points(), ==, 0);
			TEST_ENSURE(tree.bound().empty());
			TEST_ENSURE_OP(tree.leaves(), ==, 1);
			TEST_ENSURE_OP(tree.nodes(), ==, 1);

			{
				Tree bTree(tree);
				TEST_ENSURE(check(tree));
				TEST_ENSURE(equivalentKdTree(tree, bTree));

				tree.swap(bTree);
				TEST_ENSURE(check(tree));
				TEST_ENSURE(equivalentKdTree(tree, bTree));
			}
		}

		template <int N>
		void testCircular()
		{
			typedef PointKdTree<real, N> Tree;
			typedef typename Tree::Point_ConstIterator Point_ConstIterator;

			const integer m = 10000;

			std::vector<Vector<real, N> > pointSet;
			pointSet.reserve(m);
			for (integer i = 0;i < m;++i)
			{
				pointSet.push_back(2 * randomVectorSphere<real, N>());
			}

			Tree tree;

			tree.insert(
				range(pointSet.begin(), pointSet.end()));

			tree.refine(SlidingMidpoint_SplitRule_PointKdTree());
			
			Euclidean_NormBijection<real> normBijection;

			{
				std::vector<Point_ConstIterator> neighborSet;
				std::vector<real> distanceSet;

				const integer count = searchNearest(
					tree, Vector<real, N>(0), 
					m, 
					std::back_inserter(neighborSet), 
					std::back_inserter(distanceSet));
				
				TEST_ENSURE_OP(count, ==, m);
				TEST_ENSURE_OP(neighborSet.size(), ==, m);
				TEST_ENSURE_OP(distanceSet.size(), ==, m);
		
				for (integer i = 0;i < count;++i)
				{
					TEST_ENSURE_OP(relativeError<real>(distanceSet[i], 
						normBijection.toBijection(2)), <, 0.001);
				}
			}
			{
				const integer outerCount = countNearest(
					tree, Vector<real, N>(0), normBijection.toBijection(2.001));
				TEST_ENSURE_OP(outerCount, ==, m);

				const integer innerCount = countNearest(
					tree, Vector<real, N>(0), normBijection.toBijection(1.999));
				TEST_ENSURE_OP(innerCount, ==, 0);
			}
		}

		template <int N>
		void testTiming()
		{
			typedef PointKdTree<real, N> Tree;
			typedef typename Tree::Point_ConstIterator Point_ConstIterator;

			integer m = 190000;
			const integer k = 20;

			for (integer i = 0;i < 3;++i)
			{
				std::vector<Vector<real, N> > pointSet;
				pointSet.reserve(m);
				for (integer i = 0;i < m;++i)
				{
					pointSet.push_back(randomGaussianVector<real, N>());
				}

				Tree tree(Vector_PointPolicy<real, N>(), true);

				tree.insert(
					range(pointSet.begin(), pointSet.end()));

				tree.refine(SlidingMidpoint_SplitRule_PointKdTree());
				
				Array<Point_ConstIterator> nearestSet(k, m);

				Timer timer;

				timer.setStart();

				std::vector<Point_ConstIterator> querySet(
					countingIterator(tree.begin()), countingIterator(tree.end()));
				
				searchAllNeighbors(
					tree,
					range(querySet.begin(), querySet.end()),
					0, k,
					&nearestSet);

				timer.store();

				log() << "Searching " << k << " neighbors for " 
					<< m << " points took " << timer.seconds() << " seconds."
					<< logNewLine;

				m *= 2;
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
		geometryTestList().add("PointKdTree", test);
	}

	CallFunction run(addTest);

}

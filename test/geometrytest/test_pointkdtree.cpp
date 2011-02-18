#include "pastelgeometrytest.h"

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/slidingmidpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/device/timer.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/sys/iterators.h"

using namespace Pastel;

namespace Pastel
{

	integer searched();

}

namespace
{

	class PointKdTree_Test
		: public TestSuite
	{
	public:
		typedef PointKdTree<real, 2> Tree;
		typedef Tree::Point_ConstIterator Point_ConstIterator;

		PointKdTree_Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			testTiming<2>();
			testEmpty();
			test();
			testCircular<2>();
			testCircular<3>();
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

	void testBegin()
	{
		PointKdTree_Test test;
		test.run();
	}

	void testAdd()
	{
		geometryTestList().add("PointKdTree", testBegin);
	}

	CallFunction run(testAdd);

}

#include "pastelgeometrytest.h"

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/slidingmidpoint_splitrule_pointkdtree.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/sys/nulliterator.h"

using namespace Pastel;

namespace
{

	class PointKdTree_Test
		: public TestSuite
	{
	public:
		PointKdTree_Test()
			: TestSuite(&geometryTestReport())
		{
		}

		virtual void run()
		{
			testCircular<2>();
			testCircular<3>();
		}

		template <int N>
		void testCircular()
		{
			const integer m = 10000;

			std::vector<Vector<real, N> > pointSet;
			pointSet.reserve(m);
			for (integer i = 0;i < m;++i)
			{
				pointSet.push_back(2 * randomVectorSphere<real, N>());
			}

			typedef PointKdTree<real, N> Tree;
			typedef typename Tree::Point_ConstIterator Point_ConstIterator;

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

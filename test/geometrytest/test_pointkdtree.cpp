#include "pastelgeometrytest.h"

#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"

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
			const integer n = N;

			const integer m = 10;

			std::vector<Vector<real, N> > pointSet;
			pointSet.reserve(m);
			for (integer i = 0;i < m;++i)
			{
				pointSet.push_back(randomVectorSphere<real, N>());
			}

			typedef PointKdTree<real, N> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

			Tree tree;

			tree.insert(
				pointSet.begin(), pointSet.end());
			
			const integer count = searchNearest(
				tree, Vector<real, N>(0), 
				m, 
				NullIterator(), 
				NullIterator(),
				infinity<real>(), 0,
				Always_AcceptPoint<ConstObjectIterator>());
			TEST_ENSURE_OP(count, ==, m);
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

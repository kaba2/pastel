// Description: Testing for matchPointsKr
// DocumentationOf: match_points_kr.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/pattern_matching/match_points_kr.h>
#include <pastel/sys/random.h>
#include <pastel/sys/range.h>
#include <pastel/sys/rational.h>

#include <pastel/geometry/pointkdtree.h>

using namespace Pastel;

namespace
{

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
			//using Real = Rational<integer>;
			using Real = real;
			using Point = Vector<Real, 2>;
			using Tree_Settings = PointKdTree_Settings<Vector_Locator<Real, 2>>;
			using Tree = PointKdTree<Tree_Settings>;

			Point translation(1, 5);

			// Generate a point-set.
			
			integer n = 1;
			std::vector<Point> aSet;
			aSet.reserve(n);

			for (integer i = 0;i < n;++i)
			{
				Point p = 2 * randomVector<Real, 2>() - 1;
				aSet.push_back(p);
			}
			
			Tree sceneTree;
			sceneTree.insertRange(range(aSet.begin(), aSet.end()));
			sceneTree.refine();

			// Generate a translated point-set.

			int m = n;
			std::vector<Point> bSet;
			bSet.reserve(m);

			for (integer i = 0; i < m; ++i)
			{
				bSet.push_back(aSet[i] + translation);
			}

			// Randomize the order of the translated point-set.
			std::random_shuffle(bSet.begin(), bSet.end());

			Tree modelTree;
			modelTree.insertRange(range(bSet.begin(), bSet.end()));
			modelTree.refine();
			
			auto result = matchPointsKr(
				modelTree, sceneTree, 
				Euclidean_NormBijection<Real>(), 
				PASTEL_TAG(report), nullOutput(),
				PASTEL_TAG(kNearest), 16,
				PASTEL_TAG(minMatchRatio), 0.7,
				PASTEL_TAG(matchingDistance), 0.01,
				PASTEL_TAG(maxBias), 0.1);

			TEST_ENSURE(result.success);
			TEST_ENSURE(allEqual(translation, result.translation));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("matchPointsKr", test);
	}

	CallFunction run(addTest);

}

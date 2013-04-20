// Description: Testing for RangeTree
// DocumentationOf: rangetree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/rangetree.h"

#include "pastel/sys/random_uniform.h"
#include "pastel/sys/pointreps.h"
#include "pastel/sys/inputs.h"
#include "pastel/sys/iterators.h"

#include <boost/range/adaptor/indirected.hpp>

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
			struct RangeTree_Types
			{
				typedef Pointer_PointRep<real> PointRep;
			};

			integer d = 3;
			integer n = 100;
			RangeTree_Types::PointRep pointRep(d);

			RangeTree<RangeTree_Types> rangeTree(pointRep);

			std::vector<real> pointSet;
			for (integer i = 0;i < 2 * n;++i)
			{
				pointSet.push_back(random<real>());
			}

			{
				auto pointRange = 
					sparseRange(
					countingIterator(&pointSet.front()), 
					countingIterator(&pointSet.front() + 2 * n), 2);

				rangeTree.construct(rangeInput(pointRange));
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
		testRunner().add("RangeTree", test);
	}

	CallFunction run(addTest);

}

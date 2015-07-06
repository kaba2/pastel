// Description: Testing for fair stable partitioning
// DocumentationOf: fair_stable_partition.h

#include "test_pastelsys.h"

#include <pastel/sys/sequence/fair_stable_partition.h>
#include <pastel/sys/trindicator.h>
#include <pastel/sys/indicator.h>

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
			{
				std::vector<integer> elementSet =
				{
					1, 6, 3, 4, 6, 8, 9, 0, 4
				};

				auto trindicator = [](integer)
				{
					return 0;
				};

				auto rightBegin = 
					fairStablePartition(
						range(elementSet.begin(), elementSet.end()), trindicator);

				std::vector<integer> leftSet =
				{
					1, 3, 6, 9, 4
				};

				std::vector<integer> rightSet =
				{
					6, 4, 8, 0
				};

				TEST_ENSURE(boost::equal(leftSet, 
					range(elementSet.begin(), rightBegin)));

				TEST_ENSURE(boost::equal(rightSet, 
					range(rightBegin, elementSet.end())));
			}

			{
				std::vector<integer> elementSet =
				{
					1, 6, 3, 4, 6, 8, 9, 0, 4
				};

				auto rightBegin =
					fairStablePartition(
					range(elementSet.begin(), elementSet.end()), 
					indicatorTrindicator(predicateIndicator(5, LessThan())));

				std::vector<integer> leftSet =
				{
					1, 3, 4, 0, 4
				};

				std::vector<integer> rightSet =
				{
					6, 6, 8, 9
				};

				TEST_ENSURE(boost::equal(leftSet,
					range(elementSet.begin(), rightBegin)));

				TEST_ENSURE(boost::equal(rightSet,
					range(rightBegin, elementSet.end())));
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
		testRunner().add("fair_stable_partition", test);
	}

	CallFunction run(addTest);

}

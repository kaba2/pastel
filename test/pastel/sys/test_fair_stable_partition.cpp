// Description: Testing for fair stable partitioning
// DocumentationOf: fair_stable_partition.h

#include "test/test_init.h"

#include <pastel/sys/sequence/fair_stable_partition.h>
#include <pastel/sys/trindicator.h>
#include <pastel/sys/indicator.h>

namespace
{

	class Test
	{
	public:
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

				REQUIRE(boost::equal(leftSet, 
					range(elementSet.begin(), rightBegin)));

				REQUIRE(boost::equal(rightSet, 
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

				REQUIRE(boost::equal(leftSet,
					range(elementSet.begin(), rightBegin)));

				REQUIRE(boost::equal(rightSet,
					range(rightBegin, elementSet.end())));
			}
		}
	};

	TEST_CASE("fair_stable_partition", "[fair_stable_partition]")
	{
	}

}

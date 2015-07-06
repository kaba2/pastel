// Description: Testing for interval sequences
// DocumentationOf: interval_sequence.h

#include "test_pastelsys.h"

#include "pastel/sys/interval_sequence.h"
#include "pastel/sys/range.h"

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
			testSimple();
		}

		void testSimple()
		{
			integer aSet[] = 
			{
				0, 2,
				2, 2,
				3, 6,
				6, 8,
				10, 11
			};

			integer bSet[] = 
			{
				-1, -1,
				0, 1,
				2, 2,
				4, 5,
				6, 9,
				11, 12
			};

			{
				std::vector<integer> cSet;

				auto report = [&](const std::pair<integer, integer>& that)
				{
					cSet.push_back(that.first);
					cSet.push_back(that.second);
				};
				difference(aSet, bSet, report);

				integer correctSet[] = 
				{
					1, 2,
					3, 4,
					5, 6,
					10, 11
				};

				TEST_ENSURE(boost::equal(cSet, correctSet));
			}
			
			std::vector<integer> emptySet;
			{
				std::vector<integer> cSet;
				auto report = [&](const std::pair<integer, integer>& that)
				{
					cSet.push_back(that.first);
					cSet.push_back(that.second);
				};
				difference(emptySet, bSet, report);
				TEST_ENSURE(cSet.empty());
			}
			{
				std::vector<integer> cSet;
				auto report = [&](const std::pair<integer, integer>& that)
				{
					cSet.push_back(that.first);
					cSet.push_back(that.second);
				};
				difference(aSet, emptySet, report);

				integer correctSet[] = 
				{
					0, 2,
					3, 6,
					6, 8,
					10, 11
				};

				TEST_ENSURE(boost::equal(cSet, correctSet));
			}
		}
	};

	void test()
	{
		Test test;
		test.run();		
	}

	void addTests()
	{
		testRunner().add("IntervalSequence", test);
	}

	CallFunction run(addTests);

}

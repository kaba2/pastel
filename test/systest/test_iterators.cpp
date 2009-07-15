#include "pastelsystest.h"

#include "pastel/sys/countingiterator.h"

using namespace Pastel;

namespace
{

	class Iterator_Test
		: public TestSuite
	{
	public:
		Iterator_Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			testCounting();
		}

		void testCounting()
		{
			std::vector<integer> a;
			a.push_back(1);
			a.push_back(2);
			a.push_back(3);
			a.push_back(4);
			a.push_back(5);
			a.push_back(6);

			TEST_ENSURE(
				std::equal(a.begin(), a.end(), 
				CountingIterator<integer>(1)));
		}
	};

	void testIterators()
	{
		Iterator_Test test;
		test.run();		
	}

	void addTests()
	{
		sysTestList().add("iterators", testIterators);
	}

	CallFunction run(addTests);

}

#include "pastelsystest.h"

#include "pastel/sys/iteratorrange.h"

using namespace Pastel;

namespace
{

	class IteratorRange_Test
		: public TestSuite
	{
	public:
		IteratorRange_Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
		}

		void testForward()
		{
			typedef std::vector<integer> Container;
			typedef Container::iterator Iterator;

			Container a;
			for (integer i = 0;i < 20;++i)
			{
				a.push_back(i);
			}

			ForwardRange<Iterator> r(
				a.begin(), a.end());

			ForwardRange<Iterator> s(
				a.begin(), a.end(), a.size());

			TEST_ENSURE(r == s);
		}
	};

	void testIteratorRange()
	{
		IteratorRange_Test test;
		test.run();		
	}

	void addTests()
	{
		sysTestList().add("IteratorRange", testIteratorRange);
	}

	CallFunction run(addTests);

}

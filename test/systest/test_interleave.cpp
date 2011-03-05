// Description: Testing for interleaving
// DocumentationOf: interleave.h

#include "pastelsystest.h"

#include "pastel/sys/interleave.h"
#include "pastel/sys/array_range.h"
#include "pastel/sys/range_for_all.h"

#include <iostream>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			test();
		}

		void test()
		{
			{
				integer aSet[] = {0, 1, 2, 3, 4, 5};
				const integer bSet[] = {0, 2, 4, 1, 3, 5};

				interleave(arrayRange(aSet), 3);
				TEST_ENSURE(rangeForAll(
					arrayRange(aSet), arrayRange(bSet), EqualTo()));
			}
			{
				integer aSet[] = {0, 1, 2, 3, 4, 5};
				const integer bSet[] = {0, 3, 1, 4, 2, 5};

				interleave(arrayRange(aSet), 2);
				TEST_ENSURE(rangeForAll(
					arrayRange(aSet), arrayRange(bSet), EqualTo()));
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
		sysTestList().add("interleave", test);
	}

	CallFunction run(addTest);

}

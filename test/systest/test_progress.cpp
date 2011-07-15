// Description: Testing for Progress class
// DocumentationOf: progress.h

#include "pastelsystest.h"

#include "pastel/sys/progress.h"

#include <iostream>
#include <list>

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
			int b = 0;
			Progress progress(10, "progress");
			for (integer i = 0;i < 10;++i)
			{
				int a = 0;
				for (integer j = 0;j < (1 << 30);++j)
				{
					a += j * j;
				}
				b += a;
				progress.report();
			}
			std::cout << b << std::endl;
		}
	};

	void test()
	{
		Test theTest;
		theTest.run();
	}

	void addTest()
	{
		testRunner().add("Progress", test);
	}

	CallFunction run(addTest);

}

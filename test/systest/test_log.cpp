// Description: Testing for logging
// DocumentationOf: log.h

#include "pastelsystest.h"

#include "pastel/sys/log.h"
#include "pastel/sys/loggers.h"

#include <iostream>

using namespace Pastel;
using namespace std;

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
			Stream_Logger firstLogger(&std::cout);

			{
				Log log;

				log.addLogger(&firstLogger);

				{
					Stream_Logger streamLogger(&std::cout);
					log.addLogger(&streamLogger);
					
					log << "Should see twice." << logNewLine;
				}

				log << "Should see only once." << logNewLine;
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
		testRunner().add("Log", test);
	}

	CallFunction run(addTest);

}

// Description: Testing for Optional
// DocumentationOf: optional.h

#include "pastelsystest.h"

#include "pastel/sys/optional.h"

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
			Optional<integer> a;
			TEST_ENSURE(a.empty());
			TEST_ENSURE(a == 0);
			
			a = 2;
			TEST_ENSURE(!a.empty());
			TEST_ENSURE(a == 2);

			a.clear();
			TEST_ENSURE(a.empty());
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Optional", test);
	}

	CallFunction run(addTest);

}

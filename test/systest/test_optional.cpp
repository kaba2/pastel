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

		class B
		{
		public:
			B()
				: b(0)
			{
			}

			explicit B(int b_)
				: b(b_)
			{
			}

			int b;
		};

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

			Optional<B> b;
			TEST_ENSURE(b.empty());
			TEST_ENSURE(b.b == 0);
			
			b.b = 3;
			TEST_ENSURE(b.empty());
			TEST_ENSURE(b.b == 3);
			b = B(2);
			TEST_ENSURE(!b.empty());
			TEST_ENSURE(b.b == 2);
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

// Description: Testing for optional objects
// DocumentationOf: optional.h

#include "test_pastelsys.h"

#include "pastel/sys/optional.h"

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

		void f(integer x)
		{
		}

		void g(Optional<integer> x)
		{
		}

		void test()
		{
			Optional<integer> a;
			TEST_ENSURE(a.empty());
			// The zero is treated as a non-empty
			// optional: therefore it should not
			// match.
			TEST_ENSURE(a != 0);
			TEST_ENSURE(a != 0.0);

			a = 2;
			TEST_ENSURE(!a.empty());
			TEST_ENSURE(a == 2);
			TEST_ENSURE(a == 2.0);

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

			// Optional<Type> is implicitly convertible 
			// to type.
			f(a);

			// Type is implicitly convertible to Optional<Type>.
			g(3);
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
